/**
 *
 * IFJ Projekt 2018
 *
 * xdacer00: Ondřej Dacer
 * xzitny01: Roland Žitný
 * xnovot1l: Jakub Novotný
 * xsvehl09: Lubomír Švehla
 *
 *
 */

#include <math.h>
#include "symtable.h"
#include "scanner.h"
#include "expression_parser.h"
#include "errors.h"
#include "code_generator.h"
#include "token_list.h"

/** declaration of functions defined in parser.c*/
extern symtab_t *LST;                    // globalní proměnná uchovávající globální tabulku symbolů
extern int token;
extern string attr;
extern tTokenList TokenList;

extern stack_t stack;                          // globalní proměnná uchovávající zásobník príchodzích terminálov a simulovnaých neterminálov

int precTable[PT_SIZE][PT_SIZE] =
        {
                //*,/   +,-   rel   eq    (     )     op    $
                {PT_L, PT_L, PT_L, PT_L, PT_R, PT_L, PT_R, PT_L},   // *, /
                {PT_R, PT_L, PT_L, PT_L, PT_R, PT_L, PT_R, PT_L},   // +, -
                {PT_R, PT_R, PT_N, PT_L, PT_R, PT_L, PT_R, PT_L},   // <, <=, >, >=
                {PT_R, PT_R, PT_R, PT_N, PT_R, PT_L, PT_R, PT_L},   // ==, !=
                {PT_R, PT_R, PT_R, PT_R, PT_R, PT_E, PT_R, PT_N},   // (
                {PT_L, PT_L, PT_L, PT_L, PT_N, PT_L, PT_N, PT_L},   // )
                {PT_L, PT_L, PT_L, PT_L, PT_N, PT_L, PT_N, PT_L},   // operand (id, int, double, string, nil)
                {PT_R, PT_R, PT_R, PT_R, PT_R, PT_N, PT_R, PT_N}    // $
        };                                                          // riadky predstavujú vrch zásobníku, stĺpce príchodzí token

int mapToken(int token)
{
  switch (token) {
    case MUL:
    case DIV:
      return 0;
    case PLUS:
    case MINUS:
      return 1;
    case LESS:
    case LESS_EQUAL:
    case GREAT:
    case GREAT_EQUAL:
      return 2;
    case EQUAL:
    case NOT_EQUAL:
      return 3;
    case LEFT_BRACKET:
      return 4;
    case RIGHT_BRACKET:
      return 5;
    case ID:
    case INT_LIT:
    case FLOAT_LIT:
    case STR_LIT:
    case NIL:
      return 6;
    default:
      return 7;
  }
}

int symbolsBeforeMark()
{
  stack_elem_t* tmp = ss_top(&stack);
  int count = 0;

  while (tmp != NULL && tmp->symbol != PT_R)                  //nájdi počet symbolov medzi vrcholom a zaráźkou
  {
    count++;
    tmp = tmp->next;
  }

  if ((tmp != NULL && tmp->symbol == PT_R) && (count == 1 || count == 3))   //nejaká zarážka musí byť nájdená
    return count;                                       //povolený počet symbolov je vzhľadom na pravidlá len 1, alebo 3
  else
    return ERR_SYNTAX;
}

int usedRule(int count, stack_elem_t* op1, stack_elem_t* op2, stack_elem_t* op3)
{
  // E -> i
  if (count == 1 && (op3->symbol == ID || op3->symbol == INT_LIT || op3->symbol == FLOAT_LIT || op3->symbol == STR_LIT || op3->symbol == NIL))
    return EP_OPERAND;

    //E -> (E)
  else if (count == 3 && op1->symbol == LEFT_BRACKET && op2->symbol == NON_TERMINAL && op3->symbol == RIGHT_BRACKET)
    return EP_BRACKETS;

  else if (count == 3 && op1->symbol == NON_TERMINAL && op3->symbol == NON_TERMINAL)
    switch (op2->symbol)
    {
      //E -> E*E  ||  E -> E/E  ||  E -> E+E  ||  E -> E-E
      case MUL:
      case DIV:
      case PLUS:
      case MINUS:
        return EP_ARITMETIC;
        //E -> E<E  ||  E -> E<=E  ||  E -> E>E  ||  E -> E>=E  ||   E => E==E  ||  E => E!=E
      case LESS:
      case LESS_EQUAL:
      case GREAT:
      case GREAT_EQUAL:
      case EQUAL:
      case NOT_EQUAL:
        return EP_REL;
      default:
        return ERR_SYNTAX;
    }
  return ERR_SYNTAX;
}

int checkSemantics(int rule, stack_elem_t *op1, stack_elem_t *op2, stack_elem_t *op3, datatype_t *final_type, int generate_flag)
{
  switch (rule)                                         //podla známeho pravidla vyberie čo sa bude kontrolovať
  {
    case EP_OPERAND:
      *final_type = op3->type;                          //výsledný typ neterminálu (redukovaného výrazu) bude taký ako je typ operandu 1
      break;

    case EP_BRACKETS:
      *final_type = op2->type;
      break;

    case EP_ARITMETIC:
      if (op1->type == DATA_TYPE_NIL || op3->type == DATA_TYPE_NIL)
        return ERR_TYPE;
      if (op1->type == DATA_TYPE_BOOL || op3->type == DATA_TYPE_BOOL)
        return ERR_TYPE;

      if (op1->type == DATA_TYPE_STRING && op3->type == DATA_TYPE_STRING && op2->symbol == PLUS)  //ak je operácia +, možno sčítať špeciálnym spôsobom aj stringy
      {
        *final_type = DATA_TYPE_STRING;
        break;
      }

      if (op1->type == DATA_TYPE_INT && op3->type == DATA_TYPE_INT)   // aritmetická operácia s 2 integermi => vysledna hodnota tieź integer
      {
        *final_type = DATA_TYPE_INT;
        break;
      }

      if (op1->type == DATA_TYPE_STRING || op3->type == DATA_TYPE_STRING) // string nemôžem s iným typom sčíta, odčítať, násobiť, ani deliť
        return ERR_TYPE;

      if (op1->type == DATA_TYPE_UNKNOWN || op3->type == DATA_TYPE_UNKNOWN) // keď počítam s neznámym typom, bude neznámy typ mať aj výsledok
      {
        *final_type = DATA_TYPE_UNKNOWN;
        break;
      }

      *final_type = DATA_TYPE_DOUBLE;               // vo všetkých ostatných prípadoch je jasné, źe výsledný typ bude double

      if (generate_flag == 1)
      {
        if (op1->type == DATA_TYPE_INT)               //pretypovanie operandu 1 (ten je hlbšie v zásobníku)
          codeGen_stackToDouble_2();

        if (op3->type == DATA_TYPE_INT)               // pretypovanie operandu 2 (ten je na vrchu)
          codeGen_stackToDouble_1();
      }

      break;

    case EP_REL:
      if (op2->symbol != EQUAL && op2->symbol != NOT_EQUAL)
      {
        if (op1->type == DATA_TYPE_NIL || op3->type == DATA_TYPE_NIL)
          return ERR_TYPE;
      }
      *final_type = DATA_TYPE_BOOL;                                     //u relačných operácií je výsledný typ vždy bool

      if (generate_flag == 1)
      {
        if (op1->type == DATA_TYPE_INT && op3->type == DATA_TYPE_DOUBLE)  //pretypovanie operandu 1 ako u aritmetických operácií
          codeGen_stackToDouble_2();

        else if (op1->type == DATA_TYPE_DOUBLE && op3->type == DATA_TYPE_INT) //rovnako pre druhý operand
          codeGen_stackToDouble_1();
      }

      if (op1->type == DATA_TYPE_UNKNOWN || op3->type == DATA_TYPE_UNKNOWN) // keď počítam s neznámym typom, bude neznámy typ mať aj výsledok
      {
        break;
      }

      else if (op2->symbol != EQUAL && op2->symbol != NOT_EQUAL && (op1->type != op3->type))    // ak sa ani po pretypovaní hodnoty nerovnajú (napr bol použitý string) => chyba
        return ERR_TYPE;

      break;

    default:
      break;
  }

  return NO_ERR;
}

int reduce(int generate_flag)
{
  int result = 0;
  stack_elem_t* op1 = NULL;
  stack_elem_t* op2 = NULL;
  stack_elem_t* op3 = NULL;
  datatype_t expression_type;
  int rule;

  int count = symbolsBeforeMark();        //kontrola počtu symbolov nad zaráźkou

  if (count == ERR_SYNTAX)
    return ERR_SYNTAX;

    /// mimo pripad E = i využívam 3 operandy
  else if (count == 3)
  {
    op1 = stack.top->next->next;
    op2 = stack.top->next;
  }

  /// jeden využijem vždy
  op3 = stack.top;
  rule = usedRule(count, op1, op2, op3);


  if (rule == ERR_SYNTAX)
    return ERR_SYNTAX;
  else
  {
    if ((result = checkSemantics(rule, op1, op2, op3, &expression_type, generate_flag)) != NO_ERR )
      return result;
    if (count == 3)
    {
      if (generate_flag == 1)                                             //ak sme redukovali s troma operandmi a chceme generovat kod
      {
        if(rule == EP_ARITMETIC && expression_type == DATA_TYPE_STRING)
          codeGen_concat_StackStrings();

        else
        {
          codeGen_stack_operation(op2->symbol, op1->type, op3->type);
        }
      }

    }
    ss_multiplePop(&stack, count + 1);                    //odstráň terminál(y) so zaráźkou a pushni nový neterminál
    ss_push(&stack, NON_TERMINAL, expression_type);
  }
return NO_ERR;
}

int isIdDefined(char* id_name)
{
  symtab_elem_t *symbol;

  symbol = st_search(LST, id_name);
  if (symbol == NULL)
    return ERR_SEM;
  else
    return symbol->d_type;
}

int getLiteralType(int token)
{
  switch (token)
  {
    case INT_LIT:
      return DATA_TYPE_INT;
    case FLOAT_LIT:
      return DATA_TYPE_DOUBLE;
    case STR_LIT:
      return DATA_TYPE_STRING;
    default:
      return DATA_TYPE_NIL;
  }

}



int computeForZero(double *value, int symbol, int *zero_flag)
{
  stack_elem_t *operations_before = ss_top(&stack);

  if (symbol == MUL || symbol == DIV || symbol == PLUS || symbol == MINUS)
  {
    if (token == INT_LIT || token == FLOAT_LIT)
    {
      switch (symbol)
      {
        case PLUS:
          *value = *value + strtod(TokenList.Active->attr, NULL);
          break;
        case MINUS:
          *value = *value - strtod(TokenList.Active->attr, NULL);
          break;
        case MUL:
          while (operations_before->symbol != LEFT_BRACKET)
          {
           if (operations_before->symbol == PLUS || operations_before->symbol == MINUS)
           {
             *zero_flag = 0;
             *value = 0;
             break;
           }
            operations_before = operations_before->next;
          }
          *value = *value * strtod(TokenList.Active->attr, NULL);
          break;
        case DIV:
          while (operations_before->symbol != LEFT_BRACKET)
          {
            if (operations_before->symbol == PLUS || operations_before->symbol == MINUS)
            {
              *zero_flag = 0;
              *value = 0;
              break;
            }
            operations_before = operations_before->next;
          }
          *value = *value / strtod(TokenList.Active->attr, NULL);
          break;
        default:
          break;
      }
    }
    else if (token != NON_TERMINAL)
    {
      *zero_flag = 0;
      *value = 0;
    }
  }
  else if (symbol == INT_LIT || symbol == FLOAT_LIT)
  {
    if (token == RIGHT_BRACKET)
    {
      *zero_flag = 0;
      if (fabs(*value) < 0.0000000001)
        return ERR_ZERO;
    }
  }
  return NO_ERR;
}

int expression(int generate_flag)
{

  /* ss_init(&stack); */  //inicializácia stacku, a pushnutie hodnoty $ na vrchol


  bool success = false;                                         //test správneho tvaru prečítaného expression
  stack_elem_t* top_terminal;                                   //vrchný terminál v zásobníku
  int id_data_type = DATA_TYPE_NIL;
  int result;
  double value = 0;
  int check_zero = 0;
  stack_elem_t *top_stack;

  do                                                                    //kým nenájde error, alebo nenájde $ na vrchu zásobníka aj v tokene s pravidlom N
  {
    top_terminal = ss_topTerminal(&stack);                              //ak pribudol, alebo ubudol terminál na vrchole stacku, aktualizuj
    if (top_terminal == NULL)
      return ERR_INTERN;

    top_stack = ss_top(&stack);

    if (check_zero == 1)                                                            //compute if flag is raised
      if (computeForZero( &value, top_stack->symbol, &check_zero) == ERR_ZERO)
        return ERR_ZERO;

    if (top_stack->symbol == DIV)                                              // check for zero after div
    {
      if (token == INT_LIT || token == FLOAT_LIT)
      {
        value = strtod(TokenList.Active->attr, NULL);
        if (fabs(value) < 0.0000000001)
          return ERR_ZERO;
      }
      else if (token == LEFT_BRACKET)                                               // if there is bracket, start calculating
      {
        check_zero = 1;
        value = strtod(TokenList.Active->nextItem->attr, NULL);
      }
    }

    switch (precTable[mapToken(top_terminal->symbol)][mapToken(token)])                 //vyhľadanie poloźky v precision tabuľke
    {
      case PT_R:                                                                        //PT_R (<)

        if (ss_insertBeforeTopTerminal(&stack, PT_R, DATA_TYPE_NIL) == ERR_INTERN)      //vlož < nad vrchný terminál v zásobníku
          return ERR_INTERN;
        if (token == ID)
        {
          id_data_type = isIdDefined(TokenList.Active->attr);
          if (id_data_type == ERR_SEM)
            return ERR_SEM;
          if (ss_push(&stack, token, id_data_type) == ERR_INTERN)                        //pushni čítaný token na vrchol
            return ERR_INTERN;
        }
        else
          if (ss_push(&stack, token, getLiteralType(token)) == ERR_INTERN)                        //pushni čítaný token na vrchol
            return ERR_INTERN;

        if (generate_flag == 1)
          if (token == ID || token == INT_LIT || token == FLOAT_LIT || token == STR_LIT || token == NIL)
            codeGen_pushValue(token, TokenList.Active->attr);

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;                                  //načítaj ďalší token

        break;

      case PT_L:                                                                        //PT_L (>)
        if ((result = reduce(generate_flag)) != NO_ERR )                                             //redukuj výraz od vrchu zásobníku po prvý <
          return result;
        break;

      case PT_E:                                                                        //PT_E (=)
        if (ss_push(&stack, token, DATA_TYPE_NIL) == ERR_INTERN)                        //pushni čítaný token na vrchol
          return ERR_INTERN;

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;
        break;

      case PT_N:                                                                        //PT_N ( )
        if (top_terminal->symbol==DOLLAR && ((token < COMMA || token > FLOAT_LIT) || token == RIGHT_BRACKET
                                             || token == DO || token == THEN))           //pokiaľ je $ v čítanom tokene aj vrchný terminál v zásobníku = success
        {
          if ((ss_top(&stack)->type == DATA_TYPE_BOOL || ss_top(&stack)->type == DATA_TYPE_UNKNOWN))
            success = true;
          else if (token != RIGHT_BRACKET && token != DO && token != THEN)
            success = true;
          else
            return ERR_SYNTAX;
        }
        else                                                                            //inak zadaný reťazec nezodpovedá pravidlám
          return ERR_SYNTAX;
        break;
      default:
        return ERR_SYNTAX;
    }
  }
  while (!success);

  return NO_ERR;
}