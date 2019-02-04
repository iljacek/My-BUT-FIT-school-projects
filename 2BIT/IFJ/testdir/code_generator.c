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

#include <stdio.h>
#include "scanner.h"
#include "code_generator.h"


void CodelistInit(tCodeList *L) {
  L->First = NULL;
  L->Last = NULL;
  L->Active = NULL;
  L->IfRepair = NULL;
  L->WhileRepair = NULL;
}

void codeGenStart() {
    CodelistInit(&List);
}

void CodelistFree(tCodeList *L) {
  tCodeElem *ptr;
  while (L->First != NULL) {
    ptr = L->First;
    L->First = L->First->nextItem;
    free(ptr->Instruction);
    free(ptr);
  }
}

void CodeListInsertLast(tCodeList *L, char *instr) {
  tCodeElem *newItem;
  newItem = malloc(sizeof(tCodeElem));
  newItem->Instruction = malloc((strlen(instr)+1) * sizeof(char));

  if (newItem == NULL)
  {
    return;
  }

  strcpy(newItem->Instruction,instr);
  newItem->nextItem = NULL;
  newItem->prevItem = L->Last;

  if (L->First == NULL)
    L->First = newItem;
  else
    L->Last->nextItem = newItem;
  L->Last = newItem;
}

void CodelistFirst(tCodeList *L) {
  L->Active = L->First;
}

void CodelistNext(tCodeList *L) {
  if (L->Active != NULL)
    L->Active = L->Active->nextItem;
}

void listGoto(tCodeList *L, void *gotoInstr ) {
  L->Active = (tCodeElem*)gotoInstr;
}

void CodeListInsertBTW(tCodeElem *I, char instr[INSTRUCTION])
{
  tCodeElem *newElem;
  newElem = malloc(sizeof(tCodeElem));
  newElem->Instruction = malloc(INSTRUCTION * sizeof(char));

  strcpy(newElem->Instruction , instr);

  newElem->prevItem = I->prevItem;
  newElem->nextItem = I;
  I->prevItem->nextItem = newElem;
  I->prevItem = newElem;
}


/* Vestavěné funkce */
/* INPUTS */
void define_function_inputs()
{
  CodeListInsertLast(&List,"\n\n #Build-In function inputs");
  CodeListInsertLast(&List,"\n LABEL $inputs");
  CodeListInsertLast(&List,"\n PUSHFRAME");
  CodeListInsertLast(&List,"\n DEFVAR LF@%retval");
  CodeListInsertLast(&List,"\n READ LF@%retval string");
  CodeListInsertLast(&List,"\n DEFVAR LF@%tring_length");
  CodeListInsertLast(&List,"\n CREATEFRAME");
  CodeListInsertLast(&List,"\n DEFVAR TF@%1");
  CodeListInsertLast(&List,"\n MOVE TF@%1 LF@%retval");
  CodeListInsertLast(&List,"\n CALL $length");
  CodeListInsertLast(&List,"\n MOVE LF@%tring_length TF@%retval");
  CodeListInsertLast(&List,"\n DEFVAR LF@%jump_cond");
  CodeListInsertLast(&List,"\n EQ LF@%jump_cond LF@%tring_length int@0");
  CodeListInsertLast(&List,"\n JUMPIFEQ $length_return_int LF@%jump_cond bool@false");
  CodeListInsertLast(&List,"\n MOVE LF@%retval nil@nil");
  CodeListInsertLast(&List,"\n LABEL $length_return_int");
  CodeListInsertLast(&List,"\n POPFRAME");
  CodeListInsertLast(&List,"\n RETURN");
}

/* INPUTI */
void define_function_inputi()
{
  CodeListInsertLast(&List,"\n\n #Build-In function inputi");
  CodeListInsertLast(&List,"\n LABEL $inputi");
  CodeListInsertLast(&List,"\n PUSHFRAME");
  CodeListInsertLast(&List,"\n DEFVAR LF@%retval");
  CodeListInsertLast(&List,"\n READ LF@%retval int");
  CodeListInsertLast(&List,"\n POPFRAME");
  CodeListInsertLast(&List,"\n RETURN");
}

/* INPUTF */
void define_function_inputf()
{
  CodeListInsertLast(&List,"\n\n #Build-In function inputf");
  CodeListInsertLast(&List,"\n LABEL $inputf");
  CodeListInsertLast(&List,"\n PUSHFRAME");
  CodeListInsertLast(&List,"\n DEFVAR LF@%retval");
  CodeListInsertLast(&List,"\n READ LF@%retval float");
  CodeListInsertLast(&List,"\n POPFRAME");
  CodeListInsertLast(&List,"\n RETURN");
}

/* LENGTH */
void define_function_length()
{
  CodeListInsertLast(&List,"\n\n #Build-In function length");
  CodeListInsertLast(&List,"\n LABEL $length");
  CodeListInsertLast(&List,"\n PUSHFRAME");
  CodeListInsertLast(&List,"\n DEFVAR LF@%retval");
  CodeListInsertLast(&List,"\n STRLEN LF@%retval LF@%1");
  CodeListInsertLast(&List,"\n POPFRAME");
  CodeListInsertLast(&List,"\n RETURN");
}

/* SUBSTR */
void define_function_substr()
{
  CodeListInsertLast(&List,"\n\n #Build-In function substr");
  CodeListInsertLast(&List,"\n LABEL $substr");
  CodeListInsertLast(&List,"\n PUSHFRAME");
  CodeListInsertLast(&List,"\n DEFVAR LF@%retval");
  CodeListInsertLast(&List,"\n MOVE LF@%retval string@");
  /* Délka řetězce s */
  CodeListInsertLast(&List,"\n DEFVAR LF@str_len");
  CodeListInsertLast(&List,"\n CREATEFRAME");
  CodeListInsertLast(&List,"\n DEFVAR TF@%1");
  CodeListInsertLast(&List,"\n MOVE TF@%1 LF@%1");
  CodeListInsertLast(&List,"\n CALL $length");
  CodeListInsertLast(&List,"\n MOVE LF@str_len TF@%retval");
  CodeListInsertLast(&List,"\n SUB LF@str_len LF@str_len int@1"); /* odečtení o jedničku */
  /* Index menší než 0 - Spodní mez */
  CodeListInsertLast(&List,"\n DEFVAR LF@index");
  CodeListInsertLast(&List,"\n MOVE LF@index LF@%2");
  CodeListInsertLast(&List,"\n DEFVAR LF@index_cond");
  CodeListInsertLast(&List,"\n LT LF@index_cond LF@index int@0");
  CodeListInsertLast(&List,"\n JUMPIFEQ $substr$nil$return LF@index_cond bool@true");
  /* Horní mez */
  CodeListInsertLast(&List,"\n GT LF@index_cond LF@index LF@str_len");
  CodeListInsertLast(&List,"\n JUMPIFEQ $substr$nil$return LF@index_cond bool@true");
  /* Podmínka délky podřetězce */
  CodeListInsertLast(&List,"\n DEFVAR LF@substr_len");
  CodeListInsertLast(&List,"\n MOVE LF@substr_len LF@%3");
  CodeListInsertLast(&List,"\n DEFVAR LF@substr_len_cond");
  CodeListInsertLast(&List,"\n LT LF@substr_len_cond LF@substr_len int@0");
  CodeListInsertLast(&List,"\n JUMPIFEQ $substr$nil$return LF@substr_len_cond bool@true");
  CodeListInsertLast(&List,"\n EQ LF@substr_len_cond LF@substr_len int@0");
  CodeListInsertLast(&List,"\n JUMPIFEQ $substr$nil$return LF@substr_len_cond bool@true");
  /* Cyklus */
  CodeListInsertLast(&List,"\n DEFVAR LF@char");
  CodeListInsertLast(&List,"\n DEFVAR LF@loop_cond");
  CodeListInsertLast(&List,"\n LABEL $substr$loop");
  CodeListInsertLast(&List,"\n GETCHAR LF@char LF@%1 LF@index");
  CodeListInsertLast(&List,"\n CONCAT LF@%retval LF@%retval LF@char");
  CodeListInsertLast(&List,"\n ADD LF@index LF@index int@1"); /* zvýšení indexu o 1 */
  CodeListInsertLast(&List,"\n GT LF@loop_cond LF@index LF@str_len");
  CodeListInsertLast(&List,"\n JUMPIFEQ $substr$string$return LF@loop_cond bool@true");
  CodeListInsertLast(&List,"\n SUB LF@substr_len LF@substr_len int@1"); /* délka podřetězce - 1 */
  CodeListInsertLast(&List,"\n GT LF@loop_cond LF@substr_len int@0");
  CodeListInsertLast(&List,"\n JUMPIFEQ $substr$loop LF@loop_cond bool@true");
  CodeListInsertLast(&List,"\n JUMP $substr$string$return");
  /* Labely */
  CodeListInsertLast(&List,"\n LABEL $substr$nil$return");
  CodeListInsertLast(&List,"\n MOVE LF@%retval nil@nil");
  CodeListInsertLast(&List,"\n LABEL $substr$string$return");
  CodeListInsertLast(&List,"\n POPFRAME");
  CodeListInsertLast(&List,"\n RETURN");
}

/* ORD */
void define_function_ord()
{
  CodeListInsertLast(&List,"\n\n #Build-In function ord");
  CodeListInsertLast(&List,"\n LABEL $ord");
  CodeListInsertLast(&List,"\n PUSHFRAME");
  CodeListInsertLast(&List,"\n DEFVAR LF@%retval");
  CodeListInsertLast(&List,"\n MOVE LF@%retval nil@nil");
  /* Index menší než 0 - Spodní mez */
  CodeListInsertLast(&List,"\n DEFVAR LF@index");
  CodeListInsertLast(&List,"\n LT LF@index LF@%2 int@0");
  CodeListInsertLast(&List,"\n JUMPIFEQ $ord$nil$return LF@index bool@true");
  /* Délka řetězce s */
  CodeListInsertLast(&List,"\n DEFVAR LF@str_len");
  CodeListInsertLast(&List,"\n CREATEFRAME");
  CodeListInsertLast(&List,"\n DEFVAR TF@%1");
  CodeListInsertLast(&List,"\n MOVE TF@%1 LF@%1");
  CodeListInsertLast(&List,"\n CALL $length");
  CodeListInsertLast(&List,"\n MOVE LF@str_len TF@%retval");
  CodeListInsertLast(&List,"\n SUB LF@str_len LF@str_len int@1"); /* odečtení o jedničku */
  /* Horní mez */
  CodeListInsertLast(&List,"\n GT LF@index LF@%2 LF@str_len");
  CodeListInsertLast(&List,"\n JUMPIFEQ $ord$nil$return LF@index bool@true");
  CodeListInsertLast(&List,"\n STRI2INT LF@%retval LF@%1 LF@%2");
  CodeListInsertLast(&List,"\n JUMP $ord$int$return");

  CodeListInsertLast(&List,"\n LABEL $ord$nil$return");
  CodeListInsertLast(&List,"\n MOVE LF@%retval nil@nil");
  CodeListInsertLast(&List,"\n LABEL $ord$int$return");
  CodeListInsertLast(&List,"\n POPFRAME");
  CodeListInsertLast(&List,"\n RETURN");
}

/* CHR */
void define_function_chr()
{
  CodeListInsertLast(&List,"\n\n #Build-In function chr");
  CodeListInsertLast(&List,"\n LABEL $chr");
  CodeListInsertLast(&List,"\n PUSHFRAME");
  CodeListInsertLast(&List,"\n DEFVAR LF@%retval");
  CodeListInsertLast(&List,"\n INT2CHAR LF@%retval LF@%1");
  CodeListInsertLast(&List,"\n POPFRAME");
  CodeListInsertLast(&List,"\n RETURN");
}

/* Definice všech vestavěných funkcí */
void define_buildInFunc()
{
  define_function_inputs();
  define_function_inputi();
  define_function_inputf();
  define_function_length();
  define_function_substr();
  define_function_ord();
  define_function_chr();
}

/* Hlavička souboru */
void codeGen_labelHeader()
{
  CodeListInsertLast(&List, " .IFJcode18");
}

/* Začátek programu - definice GF a JUMP na main */
void codeGen_header()
{
  CodeListInsertLast(&List,"\n\n # Start of program");
  CodeListInsertLast(&List,"\n DEFVAR GF@%input_prompt");
  CodeListInsertLast(&List,"\n MOVE GF@%input_prompt string@?\\032");
  CodeListInsertLast(&List,"\n DEFVAR GF@%tmp_op1");
  CodeListInsertLast(&List,"\n DEFVAR GF@%tmp_op2");
  CodeListInsertLast(&List,"\n DEFVAR GF@%tmp_op3");
  CodeListInsertLast(&List,"\n DEFVAR GF@%exp_result");
  CodeListInsertLast(&List,"\n\n JUMP $$main");
}

/* Uvolnění listu s instrukcemi */
void codeGenClear()
{
  CodelistFree(&List);
}

/* Začátek mainu */
void codeGen_Main_Start()
{
  CodeListInsertLast(&List, "\n\n #Main");
  CodeListInsertLast(&List, "\n LABEL $$main");
  CodeListInsertLast(&List, "\n CREATEFRAME");
  CodeListInsertLast(&List, "\n PUSHFRAME");
}

/* Konec mainu */
void codeGen_Main_End()
{
  CodeListInsertLast(&List, "\n\n POPFRAME");
  CodeListInsertLast(&List, "\n CLEARS");
  CodeListInsertLast(&List, " \n #End of main \n");
}

/* Začátek definice funkce */
void codeGen_func_Start(char *function_id)
{
  char str1[INSTRUCTION+strlen(function_id)];
  strcpy(str1,"\n\n JUMP $");
  strcat(str1,function_id);
  strcat(str1,"$end");
  CodeListInsertLast(&List, str1);

  char str2[INSTRUCTION+strlen(function_id)];
  strcpy(str2,"\n\n #Function Start - ");
  strcat(str2,function_id);
  CodeListInsertLast(&List, str2);

  char str3[INSTRUCTION+strlen(function_id)];
  strcpy(str3,"\n LABEL $");
  strcat(str3,function_id);
  CodeListInsertLast(&List, str3);
  CodeListInsertLast(&List, "\n PUSHFRAME");
}

/* Konec definice funkce */
void codeGen_func_End(char *function_id)
{
  char str1[INSTRUCTION] = "\n #Function End";
  CodeListInsertLast(&List, "\n POPFRAME");
  CodeListInsertLast(&List, "\n RETURN");
  CodeListInsertLast(&List, str1);

  char str2[INSTRUCTION+ strlen(function_id)];
  strcpy(str2,"\n\n LABEL $");
  strcat(str2,function_id);
  strcat(str2,"$end");
  strcat(str2,"\n");
  CodeListInsertLast(&List, str2);
}

/* Generování typu návratové hodnoty */
const char *codeGen_func_VarType(datatype_t type)
{
  switch (type)
  {
    case DATA_TYPE_INT:
      return "int@0";
      break;

    case DATA_TYPE_DOUBLE:
      return "float@0.0";
      break;

    case DATA_TYPE_STRING:
      return "string@";
      break;

    case DATA_TYPE_BOOL:
      return "bool@false";
      break;

    case DATA_TYPE_NIL:
      return "nil@nil";
      break;

    default:
      return "nil@nil";
      break;
  }
}

/* Definování návratové hodnoty funkce */
void codeGen_func_RetVal(datatype_t type)
{
  char str1[INSTRUCTION] = "\n MOVE LF@%retval ";
  strcat(str1,codeGen_func_VarType(type));
  CodeListInsertLast(&List, "\n DEFVAR LF@%retval");
  CodeListInsertLast(&List, str1);
}

/* Přiřazení návratové hodnoty volané funkce do proměnné */
void codeGen_retvalUse(char *id_name)
{
  char str1[INSTRUCTION+strlen(id_name)];
  strcpy(str1,"\n MOVE LF@");
  char str2[INSTRUCTION] = " TF@%retval";
  strcat(str1, id_name);
  strcat(str1, str2);
  CodeListInsertLast(&List, str1);
}

/* Volání funkce */
void codeGen_func_Call(char *function_id)
{
  char str1[INSTRUCTION+strlen(function_id)];
  strcpy(str1,"\n CALL $");
  strcat(str1,function_id);
  CodeListInsertLast(&List, str1);
}

/* Deklarace parametru */
void codeGen_func_ParamDeclaration(char *param_id, int index)
{
  char str1[INSTRUCTION+strlen(param_id)];
  strcpy(str1,"\n DEFVAR LF@");
  strcat(str1,param_id);
  CodeListInsertLast(&List, str1);

  char str2[INSTRUCTION+strlen(param_id)];
  strcpy(str2,"\n MOVE LF@");
  strcat(str2,param_id);
  char str3[INSTRUCTION] = " LF@%";
  strcat(str2,str3);
  char str4[INSTRUCTION];
  sprintf(str4, "%d" , index);
  strcat(str2,str4);
  CodeListInsertLast(&List, str2);
}

/* Vytvoření framu před voláném funkce */
void codeGen_func_PrepPass()
{
  CodeListInsertLast(&List, "\n CREATEFRAME");
}

/*  */
void codeGen_func_TermmVal(symtab_elem_t *token, char* term_str, int is_id)
{
  char term_value[INSTRUCTION + strlen(term_str)];

  if(is_id == 0)
  {
    switch(token->d_type)
    {
      case DATA_TYPE_INT:
        strcpy(term_str,"int@");
        sprintf(term_value, "%d",token->param_num);
        strcat(term_str,term_value);
        break;

      case DATA_TYPE_DOUBLE:
        strcpy(term_str,"float@");
        sprintf(term_value, "%d",token->param_num);
        strcat(term_str , term_value);
        break;

      case DATA_TYPE_STRING:
        strcpy(term_str,"string@");
        sprintf(term_value,"%d",token->param_num);
        strcat(term_str,term_value);
        break;

      case DATA_TYPE_NIL:
        strcpy(term_str,"nil@nil");
        break;

      default:
        break;
    }
  }
  else
  {
    strcpy(term_str,"LF@");
    sprintf(term_value,"%s",token->key);
    strcat(term_str,term_value);
  }
}

void codeGen_func_PassParam(symtab_elem_t *token, int index,int is_id)
{
  char idx[INSTRUCTION];
  sprintf(idx, "%d" , index);

  char str1[INSTRUCTION] = "\n DEFVAR TF@%";
  strcat(str1,idx);
  CodeListInsertLast(&List, str1);

  char str2[INSTRUCTION] = "\n MOVE TF@%";
  strcat(str2,idx);
  strcat(str2," ");
  char str3[INSTRUCTION] = "";
  codeGen_func_TermmVal(token,str3, is_id);
  strcat(str2,str3);
  CodeListInsertLast(&List, str2);
}

void codeGen_func_PassParamVal(datatype_t type,char *value , int index)
{
  char idx[INSTRUCTION];
  sprintf(idx, "%d" , index);

  char str1[INSTRUCTION] = "\n DEFVAR TF@%";
  strcat(str1,idx);
  CodeListInsertLast(&List, str1);

  char str2[INSTRUCTION+strlen(value)];
  strcpy(str2, "\n MOVE TF@%");
  strcat(str2,idx);
  strcat(str2," ");
  char str3[INSTRUCTION+strlen(value)];

  char valNeed[INSTRUCTION + strlen(value)];
  double float_val;
  int int_val;
  switch(type)
  {
    case DATA_TYPE_INT:
      int_val = atoi(value);
      sprintf(valNeed,"%d",int_val);
      strcpy(str3,"int@");
      break;
    case DATA_TYPE_DOUBLE:
      float_val = atof(value);
      sprintf(valNeed,"%a",float_val);
      strcpy(str3,"float@");
      break;
    case DATA_TYPE_STRING:
      strcpy(valNeed,value);
      strcpy(str3 ,"string@");
      break;
    case DATA_TYPE_BOOL:
      strcpy(valNeed,value);
      strcpy(str3,"bool@");
      break;
    default:
      break;
  }

  strcat(str2,str3);
  strcat(str2,valNeed);
  CodeListInsertLast(&List, str2);
}

void codeGen_return(int last, char *name_or_buffer)
{
  char str1[INSTRUCTION+strlen(name_or_buffer)];
  strcpy(str1, "\n MOVE LF@%retval");

  if(last == ID)
  {
    strcat(str1," LF@");
    strcat(str1,name_or_buffer);
    CodeListInsertLast(&List, str1);
  }
  else if(last == STR_LIT || last == INT_LIT || last == FLOAT_LIT)
  {
    strcat(str1,name_or_buffer);
    CodeListInsertLast(&List, str1);
  }
  else if(last == INPUTS || last == INPUTI || last == INPUTF || last == LENGTH || last == SUBSTR || last == ORD || last == CHR || last == DEF)
  {
    strcat(str1," TF@%retval");
    CodeListInsertLast(&List, str1);
  }
  else if(last == PRINT)
  {
    strcat(str1," nil@nil");
    CodeListInsertLast(&List, str1);
  }
}

void codeGen_var_declare(char *var_id)
{
  char str1[INSTRUCTION+strlen(var_id)];
  strcpy(str1,"\n DEFVAR LF@");
  strcat(str1,var_id);
  CodeListInsertLast(&List, str1);
}

void codeGen_input(char *var_id, datatype_t type)
{
  char dataType[INSTRUCTION];

  switch(type)
  {
    case DATA_TYPE_INT:
      strcpy(dataType ,"int");
      break;
    case DATA_TYPE_DOUBLE:
      strcpy(dataType,"float");
      break;
    case DATA_TYPE_STRING:
      strcpy(dataType ,"string");
      break;
    case DATA_TYPE_BOOL:
      strcpy(dataType,"bool");
      break;
    case DATA_TYPE_NIL:
      strcpy(dataType,"nil");
      break;
    default:
      break;
  }

  char str1[INSTRUCTION+strlen(var_id)];
  strcpy(str1,"\n READ LF@");
  strcat(str1,var_id);
  strcat(str1," ");
  strcat(str1,dataType);
  CodeListInsertLast(&List, str1);
}

void codeGen_printID(char *id_name)
{
  char str1[INSTRUCTION+strlen(id_name)];
  strcpy(str1,"\n WRITE LF@");
  strcat(str1, id_name);
  CodeListInsertLast(&List, str1);
}

void codeGen_printLIT(char *content, datatype_t type)
{
  char dataType[INSTRUCTION];
  char dataValue[INSTRUCTION + strlen(content)];
  strcpy(dataValue,content);

  switch(type)
  {
    case DATA_TYPE_INT:
      strcpy(dataType ,"int");
      int atoi_int = atoi(content);
      sprintf(dataValue,"%d", atoi_int);
      break;
    case DATA_TYPE_DOUBLE:
      strcpy(dataType,"float");
      double float_num = atof(content);
      sprintf(dataValue,"%a",float_num);
      break;
    case DATA_TYPE_STRING:
      strcpy(dataType ,"string");
      break;
    case DATA_TYPE_BOOL:
      strcpy(dataType,"bool");
      break;
    case DATA_TYPE_NIL:
      strcpy(dataType,"nil");
      break;
    default:
      break;
  }

  char str1[INSTRUCTION+strlen(content)];
  strcpy(str1, "\n WRITE ");
  strcat(str1, dataType);
  strcat(str1, "@");
  strcat(str1, dataValue);
  CodeListInsertLast(&List, str1);
}

void codeGen_pushValue(int token, char *attr)
{
  char str1[INSTRUCTION+strlen(attr)];
  strcpy(str1,"\n PUSHS ");
  char attr_str[INSTRUCTION+strlen(attr)];
  int int_val;
  double float_val;
  
  switch(token)
  {
    case ID:
      strcat(str1,"LF@");
      strcat(str1,attr);
      CodeListInsertLast(&List,str1);
      break;
    case INT_LIT:
      strcat(str1,"int@");
      int_val = atoi(attr);
      sprintf(attr_str,"%d",int_val);
      strcat(str1,attr_str);
      CodeListInsertLast(&List,str1);
      break;
    case FLOAT_LIT:
      strcat(str1,"float@");
      float_val = atof(attr);
      sprintf(attr_str,"%a",float_val);
      strcat(str1,attr_str);
      CodeListInsertLast(&List,str1);
      break;
    case STR_LIT:
      strcat(str1,"string@");
      strcat(str1,attr);
      CodeListInsertLast(&List,str1);
      break;
    case NIL:
      CodeListInsertLast(&List,"\n PUSHS nil@nil");
      break;
    default:
      break;
  }
}

void codeGen_stack_operation(int rule , int op_1, int op_2)
{
  switch (rule)
  {
    case PLUS:
      CodeListInsertLast(&List, "\n ADDS");
      break;
    case MINUS:
      CodeListInsertLast(&List, "\n SUBS");
      break;
    case DIV:
      if(op_1 == DATA_TYPE_INT && op_2 == DATA_TYPE_INT)
      {
        CodeListInsertLast(&List, "\n IDIVS");
        break;
      }
      else
      {
        CodeListInsertLast(&List, "\n DIVS");
        break;
      }
    case MUL:
      CodeListInsertLast(&List, "\n MULS");
      break;
    case EQUAL:
      if(op_1 == op_2)
      {
        CodeListInsertLast(&List, "\n EQS");
        break;
      }
      else
      {
        CodeListInsertLast(&List, "\n POPS GF@%tmp_op1");
        CodeListInsertLast(&List, "\n POPS GF@%tmp_op2");
        CodeListInsertLast(&List, "\n PUSHS bool@false");
        break;
      }
    case NOT_EQUAL:
      if(op_1 == op_2)
      {
        CodeListInsertLast(&List, "\n EQS");
        CodeListInsertLast(&List, "\n NOTS");
        break;
      }
      else
      {
        CodeListInsertLast(&List, "\n POPS GF@%tmp_op1");
        CodeListInsertLast(&List, "\n POPS GF@%tmp_op2");
        CodeListInsertLast(&List, "\n PUSHS bool@false");
        break;
      }
    case LESS:
      CodeListInsertLast(&List, "\n LTS");
      break;
    case LESS_EQUAL:
      CodeListInsertLast(&List, "\n POPS GF@%tmp_op1");
      CodeListInsertLast(&List, "\n POPS GF@%tmp_op2");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op2");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op1");
      CodeListInsertLast(&List, "\n LTS");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op2");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op1");
      CodeListInsertLast(&List, "\n EQS");
      CodeListInsertLast(&List, "\n ORS");
      break;
    case GREAT:
      CodeListInsertLast(&List, "\n GTS");
      break;
    case GREAT_EQUAL:
      CodeListInsertLast(&List, "\n POPS GF@%tmp_op1");
      CodeListInsertLast(&List, "\n POPS GF@%tmp_op2");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op2");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op1");
      CodeListInsertLast(&List, "\n GTS");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op2");
      CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op1");
      CodeListInsertLast(&List, "\n EQS");
      CodeListInsertLast(&List, "\n ORS");
      break;
    default:
      break;
  }
}

void codeGen_concat_StackStrings()
{
  CodeListInsertLast(&List, "\n POPS GF@%tmp_op3");
  CodeListInsertLast(&List, "\n POPS GF@%tmp_op2");
  CodeListInsertLast(&List, "\n CONCAT GF@%tmp_op1 GF@%tmp_op2 GF@%tmp_op3");
  CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op1");
}

void codeGen_stackToDouble_1()
{
  CodeListInsertLast(&List, "\n INT2FLOATS");
}

void codeGen_stackToDouble_2()
{
  CodeListInsertLast(&List, "\n POPS GF@%tmp_op1");
  CodeListInsertLast(&List, "\n INT2FLOATS");
  CodeListInsertLast(&List, "\n PUSHS GF@%tmp_op1");
}

void codeGen_PopsBool()
{
  CodeListInsertLast(&List, "\n POPS GF@%tmp_op1");
}

void codeGen_PopsExpr()
{
  CodeListInsertLast(&List, "\n POPS GF@%tmp_op3");
}

void codeGen_MoveExprResult(char *id_name)
{
  char str1[INSTRUCTION+strlen(id_name)];
  strcpy(str1, "\n MOVE LF@");
  strcat(str1,id_name);
  strcat(str1, " GF@%tmp_op3");
  CodeListInsertLast(&List, str1);
  }

void codeGen_beforeIfRep(char *var_id)
{
  char str1[INSTRUCTION+strlen(var_id)];
  strcpy(str1, "\n DEFVAR LF@");
  strcat(str1,var_id);
  CodeListInsertBTW(List.IfRepair,str1);
}

void codeGen_if_start(char *function_id, int label_idx)
{
  char idx[INSTRUCTION];
  sprintf(idx, "%d" , label_idx);

  char str1[INSTRUCTION+strlen(function_id)];
  strcpy(str1,"\n JUMPIFEQ $");
  strcat(str1,function_id);
  strcat(str1,"%else%");
  strcat(str1,idx);
  char str2[INSTRUCTION] = " GF@%tmp_op1 bool@false";
  strcat(str1,str2);
  CodeListInsertLast(&List, "\n\n # If start");
  CodeListInsertLast(&List, str1);
  if(List.IfRepair == NULL)
    List.IfRepair = List.Last;
}

void codeGen_else_start(char *function_id, int label_idx)
{
  char idx[INSTRUCTION];
  sprintf(idx, "%d" , label_idx);

  char str1[INSTRUCTION+strlen(function_id)];
  strcpy(str1,"\n JUMP $");
  strcat(str1,function_id);
  strcat(str1,"%end%");
  strcat(str1,idx);
  CodeListInsertLast(&List, str1);

  CodeListInsertLast(&List, "\n\n # Else");

  char str10[INSTRUCTION+strlen(function_id)];
  strcpy(str10, "\n LABEL $");
  strcat(str10,function_id);
  strcat(str10,"%else%");
  strcat(str10,idx);
  CodeListInsertLast(&List, str10);
}

void codeGen_if_end(char *function_id, int label_idx)
{
  CodeListInsertLast(&List, "\n\n # If end");

  char idx2[INSTRUCTION];
  sprintf(idx2, "%d" , label_idx);

  char str10[INSTRUCTION+strlen(function_id)];
  strcpy(str10, "\n LABEl $");
  strcat(str10,function_id);
  strcat(str10,"%end%");
  strcat(str10,idx2);
  CodeListInsertLast(&List, str10);

  List.IfRepair = NULL;
}

void codeGen_while_start_label(char *function_id, int label_idx)
{
  char idx[INSTRUCTION];
  sprintf(idx, "%d" , label_idx);

  char str1[INSTRUCTION+strlen(function_id)];
  strcpy(str1,"\n LABEl $");
  strcat(str1,function_id);
  strcat(str1,"%while%start%");
  strcat(str1,idx);
  CodeListInsertLast(&List, "\n\n # While start");
  CodeListInsertLast(&List, str1);
  if(List.WhileRepair == NULL)
    List.WhileRepair = List.Last;

}

void codeGen_while_start(char *function_id, int label_idx)
{
  char idx[INSTRUCTION];
  sprintf(idx, "%d" , label_idx);

  char str2[INSTRUCTION+strlen(function_id)];
  strcpy(str2, "\n JUMPIFEQ $");
  strcat(str2,function_id);
  strcat(str2,"%while%end%");
  strcat(str2,idx);
  char str3[INSTRUCTION] = " GF@%tmp_op1 bool@false";
  strcat(str2,str3);
  CodeListInsertLast(&List, str2);

}

void codeGen_beforeWhileRep(char *var_id)
{
  char str1[INSTRUCTION+strlen(var_id)];
  strcpy(str1, "\n DEFVAR LF@");
  strcat(str1,var_id);
  CodeListInsertBTW(List.WhileRepair,str1);
}

void codeGen_while_end(char *function_id, int label_idx)
{
  char idx[INSTRUCTION];
  sprintf(idx, "%d" , label_idx);

  char str1[INSTRUCTION] = "\n JUMP $";
  strcat(str1,function_id);
  strcat(str1,"%while%start%");
  strcat(str1,idx);
  CodeListInsertLast(&List, str1);

  CodeListInsertLast(&List, "\n # While end \n");

  char str2[INSTRUCTION+strlen(function_id)];
  strcpy(str2, "\n LABEl $");
  strcat(str2,function_id);
  strcat(str2,"%while%end%");
  strcat(str2,idx);
  CodeListInsertLast(&List, str2);

  List.WhileRepair = NULL;
}