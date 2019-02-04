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
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "expression_parser.h"
#include "code_generator.h"
#include "errors.h"
#include "token_list.h"

int first_pass(symtab_t *globalSymbolTable);
int second_pass(symtab_t *globalSymbolTable);
int program();
int function_definition();
int function_body();
int condition_body();
int cycle_body();
int bool_condition_id();
int bool_condition_lit();
int input_function();
int length_function();
int substr_function();
int ord_function();
int chr_function();
int assigning_or_expression();
int function_call();
void var_to_symtab(symtab_elem_t *existing_var, char *id_name, datatype_t type);


int token;
int inx = 0; /* hodnota pro generování labelů */
int in_function = 0;
int in_if = 0;
int in_while = 0;
char function_name[100]; /* název funkce, ve které se právě nacházíme - pro generování labelů */
symtab_t *GST; /* globalní tabulka symbolů */
symtab_t *LST; /* lokální tabulka pro funkci */
symtab_t *mainBody; /* lokální tabulka pro hlavní tělo */
symtab_t *localSymbolTable; /* lokální tabulka symbolů pro funkci */
extern tTokenList TokenList;
string attr;
stack_t stack;
tLastLine last_line;
stack_elem_t top_of_stack;


/* 1. průchod */
int first_pass(symtab_t *globalSymbolTable)
{
	GST = globalSymbolTable;
	strInit(&attr);
	symtab_elem_t *local_function;

  TokenStart();

	if ((token = getNextToken(&attr)) == ERR_LEX)
		return ERR_LEX;

  TokenListInsertLast(&TokenList,attr.str,token);

  while(token != END_OF_FILE)
	{
		if(token == DEF)
		{
			int param_num = 0;

			if ((token = getNextToken(&attr)) == ERR_LEX)
      {
          return ERR_LEX;
      }
      /* Název funkce */
			else if (token == ID)
			{
        TokenListInsertLast(&TokenList,attr.str,token);

				if ((local_function = st_search(GST, attr.str)) != NULL)
					return ERR_SEM;


				/* Uložen
        í nového identifikátoru funkce do globální tabulky symbolů */
        local_function = st_insert(GST, attr.str);
        
        local_function->e_type = ELEM_TYPE_FUNCTION;
        local_function->defined = false;

				if ((token = getNextToken(&attr)) == ERR_LEX)
        {
            return ERR_LEX;
        }

				/* Parametry funkce */
				else if (token == LEFT_BRACKET)
				{
          TokenListInsertLast(&TokenList,attr.str,token);

					if ((token = getNextToken(&attr)) == ERR_LEX)
					{
              return ERR_LEX;
          }

					else if (token != ID && token != RIGHT_BRACKET)
						return ERR_SYNTAX;
					else
					{
            TokenListInsertLast(&TokenList,attr.str,token);

						while (token != RIGHT_BRACKET)
						{
							if (token == ID)
							{
								param_num++;

								if ((token = getNextToken(&attr)) == ERR_LEX)
									return ERR_LEX;

                TokenListInsertLast(&TokenList,attr.str,token);
								
                if (token != RIGHT_BRACKET && token != COMMA)
									return ERR_SYNTAX;

							}
							else /* token == COMMA*/
							{
								if ((token = getNextToken(&attr)) == ERR_LEX)
	 								return ERR_LEX;

                TokenListInsertLast(&TokenList,attr.str,token);
								
                if (token != ID)
									return ERR_SYNTAX;
							}
						}

						if ((token = getNextToken(&attr)) == ERR_LEX)
				    {
                return ERR_LEX;
            }

						else if (token != EOL)
							return ERR_SYNTAX;
						else
							local_function->param_num = param_num; /* uložení počtu parametrů funkce do tabulky symbolů */

            TokenListInsertLast(&TokenList,attr.str,token);
          }
				}
				else
					return ERR_SYNTAX;
			}
			else
				return ERR_SYNTAX;
		}

		/* Načtení dalšího tokenu - cyklus while */
		if ((token = getNextToken(&attr)) == ERR_LEX)
			return ERR_LEX;

    TokenListInsertLast(&TokenList,attr.str,token);

	}

	strFree(&attr);
	return NO_ERR;
}

  
/* 2. průchod */
int second_pass(symtab_t *globalSymbolTable)
{
	int result;
	GST = globalSymbolTable;
  /* Inicializace lokální tabulky symbolů, stacku a stringu */
  st_init(&mainBody);
	ss_init(&stack);
	strInit(&attr);

	/* Inicializace listu pro generování instrukcí */
  codeGenStart();
  codeGen_labelHeader();
  codeGen_header();
  define_buildInFunc();
  codeGen_Main_Start();

  TokenlistFirst(&TokenList);

  /* 2. Průchod */
  token = TokenList.Active->token;

  while(token != END_OF_FILE)
  {
    in_if = 0;
    in_while = 0;
    LST = mainBody;
    strcpy(function_name, "main"); /* nacházíme se v hlavním těle funkce */
    result = program();
    if(LST == mainBody) /* zachování obsahu tabulky symbolů hlavního těla */
      mainBody = LST;
    if (result != NO_ERR)
    {
      st_free(mainBody);
      return result;
    }
    TokenList.Active = TokenList.Active->nextItem;
    if ((token = TokenList.Active->token) == ERR_LEX)
    {
      st_free(mainBody);
      return ERR_LEX;
    }
  }

	codeGen_Main_End();

  st_free(mainBody);
	strFree(&attr);
	return NO_ERR;
}


int program()
{
  int result;
  int if_index;
  int while_index;
  int last_token;
  char id_name[100]; /* název proměnné, do které se přiřazuje nějaká hodnota */
  char first_operand_name[100]; /* název první proměnné výrazu (případně funkce) */
  symtab_elem_t *existing_var;
  symtab_elem_t *var;
  symtab_elem_t *func;
  datatype_t id_data_type;
  datatype_t literal_data_type;

  switch (token)
  {
    /* Funkce */
    case DEF:
      in_function = 1;
      result = function_definition();
      st_free(localSymbolTable);
      if (result != NO_ERR)
        return result;

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != EOL && token != END_OF_FILE)
        return ERR_SYNTAX;

      return NO_ERR;

      break;

    /* Podmínka */
    case IF:
      in_if++;
      inx++;
      if_index = inx;

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != ID && token != LEFT_BRACKET && token != INT_LIT && token != FLOAT_LIT && token != STR_LIT && token != NIL)
        return ERR_SYNTAX;

      else
      {
        if(token == ID)
        {
          if ((result = bool_condition_id()) != NO_ERR)
            return result;
        }

        else if(token == LEFT_BRACKET)
        {
          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if (token != ID && token != INT_LIT && token != FLOAT_LIT && token != STR_LIT && token != NIL)
            return ERR_SYNTAX;
          else
          {
            if (token == ID)
            {
              if ((result = bool_condition_id()) != NO_ERR)
                return result;
            }
            else
            {
              if ((result = bool_condition_lit()) != NO_ERR)
                return result;
            }
          }
        }

        else {
            if ((result = bool_condition_lit()) != NO_ERR)
              return result;
        }
      }

      codeGen_if_start(function_name, if_index);

      if (token == RIGHT_BRACKET)
      {
        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != THEN)
          return ERR_SYNTAX;
      }

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != EOL)
        return ERR_SYNTAX;
      else
      {

        result = condition_body();
        if (result != NO_ERR)
          return result;

        /* Existuje-li ELSE větev */
        if (token == ELSE)
        {
          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if (token != EOL)
            return ERR_SYNTAX;
          else
          {
            codeGen_else_start(function_name, if_index);

            result = condition_body();
            if (result != NO_ERR)
              return result;
          }

          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if (token != EOL && token != END_OF_FILE)
            return ERR_SYNTAX;
          else
          {
            inx++;
            codeGen_if_end(function_name, if_index);
            return NO_ERR;
          }
        }

        /* Neexistuje-li ELSE větev */
        else
        {
          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if (token != EOL && token != END_OF_FILE)
            return ERR_SYNTAX;
          else
          {
            codeGen_if_end(function_name, if_index);
            return NO_ERR;
          }
        }
      }

      break;

    /* Cyklus While */
    case WHILE:
      in_while++;
      inx++;
      while_index = inx;

      codeGen_while_start_label(function_name, while_index);

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != ID && token != LEFT_BRACKET && token != INT_LIT && token != FLOAT_LIT && token != STR_LIT && token != NIL)
        return ERR_SYNTAX;
      else
      {
        if(token == ID)
        {
          if ((result = bool_condition_id()) != NO_ERR)
            return result;
        }

        else if(token == LEFT_BRACKET)
        {
          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if (token != ID && token != INT_LIT && token != FLOAT_LIT && token != STR_LIT && token != NIL)
            return ERR_SYNTAX;
          else
          {
            if (token == ID)
            {
              if ((result = bool_condition_id()) != NO_ERR)
                return result;
            }
            else
            {
              if ((result = bool_condition_lit()) != NO_ERR)
                return result;
            }
          }

          if (token == RIGHT_BRACKET)
          {
            TokenList.Active = TokenList.Active->nextItem;
            token = TokenList.Active->token;

            if (token != DO)
              return ERR_SYNTAX;

          }
        }

        else
        {
          if ((result = bool_condition_lit()) != NO_ERR)
            return result;
        }
      }

      codeGen_while_start(function_name, while_index);

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != EOL)
        return ERR_SYNTAX;
      else
      {

        result = cycle_body();
        if (result != NO_ERR)
          return result;
      }

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != EOL && token != END_OF_FILE)
        return ERR_SYNTAX;

      codeGen_while_end(function_name, while_index);

      return NO_ERR;

      break;

    /* Vestavěná funkce PRINT */
    case PRINT:

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token == LEFT_BRACKET)
      {
        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != ID && token != STR_LIT && token != INT_LIT && token != FLOAT_LIT)
          return ERR_SYNTAX;
        else
        {

          while(token != RIGHT_BRACKET)
          {
            if (token == ID)
            {
              if (st_search(LST, TokenList.Active->attr) == NULL)
                return ERR_SEM;
              else
              {
                var = st_search(LST, TokenList.Active->attr);   //attr->str
                if(var->e_type == ELEM_TYPE_VAR)
                  codeGen_printID(var->key);
                else
                  return ERR_SEM;
              }

              TokenList.Active = TokenList.Active->nextItem;
              token = TokenList.Active->token;

              if (token != COMMA && token != RIGHT_BRACKET)
                return ERR_SYNTAX;
            }
            else if (token == STR_LIT || token == INT_LIT || token == FLOAT_LIT)
            {
              if(token == STR_LIT)
                codeGen_printLIT(TokenList.Active->attr, DATA_TYPE_STRING);
              else if(token == INT_LIT)
                codeGen_printLIT(TokenList.Active->attr, DATA_TYPE_INT);
              else
                codeGen_printLIT(TokenList.Active->attr, DATA_TYPE_DOUBLE);

              TokenList.Active = TokenList.Active->nextItem;
              token = TokenList.Active->token;

              if (token != COMMA && token != RIGHT_BRACKET)
                return ERR_SYNTAX;
            }
            else /* token == COMMA */
            {
              TokenList.Active = TokenList.Active->nextItem;
              token = TokenList.Active->token;

              if (token != ID && token != STR_LIT && token != INT_LIT && token != FLOAT_LIT)
                return ERR_SYNTAX;
            }
          }

          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if (token != EOL)
            return ERR_SYNTAX;
          else
          {
            if(in_function == 1)
            {
              last_line.last = PRINT;
              strcpy(last_line.name_or_buffer, "");
            }

            return NO_ERR;
          }
        }
      }

      else if (token == STR_LIT || token == INT_LIT || token == FLOAT_LIT || token == ID)
      {
        while(token != EOL)
        {
          if (token == ID)
          {
            if (st_search(LST, TokenList.Active->attr) == NULL)
              return ERR_SEM;
            else
            {
              var = st_search(LST, TokenList.Active->attr);
              if(var->e_type == ELEM_TYPE_VAR)
                codeGen_printID(var->key);
              else
                return ERR_SEM;
            }

            TokenList.Active = TokenList.Active->nextItem;
            token = TokenList.Active->token;

            if (token != COMMA && token != EOL)
              return ERR_SYNTAX;
          }
          else if (token == STR_LIT || token == INT_LIT || token == FLOAT_LIT)
          {
            if(token == STR_LIT)
              codeGen_printLIT(TokenList.Active->attr, DATA_TYPE_STRING);
            else if(token == INT_LIT)
              codeGen_printLIT(TokenList.Active->attr, DATA_TYPE_INT);
            else
              codeGen_printLIT(TokenList.Active->attr, DATA_TYPE_DOUBLE);

            TokenList.Active = TokenList.Active->nextItem;
            token = TokenList.Active->token;

            if (token != COMMA && token != EOL)
              return ERR_SYNTAX;
          }
          else /* token == COMMA */
          {
            TokenList.Active = TokenList.Active->nextItem;
            token = TokenList.Active->token;

            if (token != ID && token != STR_LIT && token != INT_LIT && token != FLOAT_LIT)
              return ERR_SYNTAX;
          }
        }
        if(in_function == 1)
        {
          last_line.last = PRINT;
          strcpy(last_line.name_or_buffer, "");
        }

        return NO_ERR;
      }

      else
        return ERR_SYNTAX;

      break;

    /* Vestavěné funkce  INPUTS, INPUTI a INPUTF - bez efektu, pouze jako návratová hodnota */
    case INPUTS:
    case INPUTI:
    case INPUTF:
      if((result = input_function()) != NO_ERR)
        return result;
      else
        return NO_ERR;

      break;

    /* Vestavěná funkce LENGTH - bez efektu, pouze jako návratová hodnota */
    case LENGTH:
      if((result = length_function()) != NO_ERR)
        return result;
      else
        return NO_ERR;

      break;

    /* Vestavěná funkce SUBSTR - bez efektu, pouze jako návratová hodnota */
    case SUBSTR:
      if((result = substr_function()) != NO_ERR)
        return result;
      else
        return NO_ERR;

      break;

    /* Vestavěná funkce ORD - bez efektu */
    case ORD:
      if((result = ord_function()) != NO_ERR)
        return result;
      else
        return NO_ERR;

      break;

    /* Vestavěná funkce CHR - bez efektu */
    case CHR:
      if((result = chr_function()) != NO_ERR)
        return result;
      else
        return NO_ERR;

      break;

    /* Řetězcový literál a případná konkatenace - bez efektu */
    case STR_LIT:
    case INT_LIT:
    case FLOAT_LIT:
    case NIL:
      if((result = assigning_or_expression()) != NO_ERR)
        return result;
      else
        return NO_ERR;

      break;

      /* Řetězcový literál a případná konkatenace - bez efektu */
    case LEFT_BRACKET:
      if (ss_push(&stack, token, DATA_TYPE_NIL) == ERR_INTERN)
        return ERR_INTERN;

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if ((result = expression(1)) != NO_ERR)
        return result;

      top_of_stack = *ss_top(&stack);

      if(in_function == 1)
      {
        last_line.last = INT_LIT; /* Ilustrativně, jedná se o jakýkoliv literál */
        strcpy(last_line.name_or_buffer, " GF@%tmp_op3");
      }

      codeGen_PopsExpr();

      ss_pop(&stack);
      ss_push(&stack, PT_R, DATA_TYPE_NIL);
      return NO_ERR;

      break;

    /* Identifikátor - přířazení, výraz, volání funkce, atd. */
    case ID:

      strcpy(id_name, TokenList.Active->attr);

      /* Volání funkce - bez efektu */
      if((func = st_search(GST, id_name)) != NULL)
      {
        /* Zákaz volání funkce z hlavního těla, pokud nebyla definovaná */
        if (LST == mainBody && func->defined == false)
          return ERR_SEM;

        if((result = function_call()) != NO_ERR)
          return result;
        else
          return NO_ERR;
      }

      existing_var = st_search(LST, id_name);

      if(in_while > 0 && existing_var == NULL)
      {
        codeGen_beforeWhileRep(id_name);
        var_to_symtab(existing_var, id_name, DATA_TYPE_UNKNOWN);
        existing_var = st_search(LST, id_name);
      }

      if(in_if > 0 && existing_var == NULL)
      {
        codeGen_beforeIfRep(id_name);
        var_to_symtab(existing_var, id_name, DATA_TYPE_UNKNOWN);
        existing_var = st_search(LST, id_name);
      }

      if(existing_var != NULL)
      {
        if (isIdDefined(id_name) == ERR_SEM)
          return ERR_SEM;

        literal_data_type = existing_var->d_type;

        if ((ss_push(&stack, token, literal_data_type)) == ERR_INTERN)
          return ERR_INTERN;

        if (reduce(1))
          return ERR_SYNTAX;

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;


        if ((token == PLUS || token == MINUS || token == MUL || token == DIV))
        {
          if ((result = expression(1)) != NO_ERR)
            return result;

          if(in_function == 1)
          {
            last_line.last = ID;
            strcpy(last_line.name_or_buffer, id_name);
          }

          top_of_stack = *ss_top(&stack);

          codeGen_PopsExpr();
          codeGen_MoveExprResult(id_name);

          ss_pop(&stack);
          ss_push(&stack, PT_R, DATA_TYPE_NIL);

          return NO_ERR;
        }


        else if (token == EOL || token == ASSIGN)
        {
          top_of_stack = *ss_top(&stack);

          ss_pop(&stack);
          ss_push(&stack, PT_R, DATA_TYPE_NIL);

          if(token == EOL)
          {

            if(in_function == 1)
            {
              last_line.last = ID;
              strcpy(last_line.name_or_buffer, id_name);
            }

            return NO_ERR;
          }
        }

        else if(token != ASSIGN)
          return ERR_SYNTAX;
      }
      else
      {
        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;
      }

      if (token == ASSIGN)
      {
        if(existing_var == NULL)
          codeGen_var_declare(id_name);

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        /* Vestavěné funkce */
        /* Vestavěná funkce INPUTS, INPUTI a INPUTF */
        if (token == INPUTS || token == INPUTI || token == INPUTF)
        {
          last_token = token;
          if((result = input_function()) != NO_ERR)
            return result;
          codeGen_retvalUse(id_name);
          if(last_token == INPUTS)
            var_to_symtab(existing_var, id_name, DATA_TYPE_STRING);
          else if(last_token == INPUTI)
            var_to_symtab(existing_var, id_name, DATA_TYPE_INT);
          else
            var_to_symtab(existing_var, id_name, DATA_TYPE_DOUBLE);
          return  NO_ERR;
        }

        /* Vestavěná funkce LENGTH */
        else if (token == LENGTH)
        {
          if((result = length_function()) != NO_ERR)
            return result;
          codeGen_retvalUse(id_name);
          var_to_symtab(existing_var, id_name, DATA_TYPE_INT);
          return  NO_ERR;
        }

        /* Vestavěná funkce SUBSTR */
        else if (token == SUBSTR)
        {
          if((result = substr_function()) != NO_ERR)
            return result;
          codeGen_retvalUse(id_name);
          var_to_symtab(existing_var, id_name, DATA_TYPE_STRING);
          return  NO_ERR;
        }

        /* Vestavěná funkce ORD */
        else if (token == ORD)
        {
          if((result = ord_function()) != NO_ERR)
            return result;
          codeGen_retvalUse(id_name);
          var_to_symtab(existing_var, id_name, DATA_TYPE_INT);
          return  NO_ERR;
        }

        /* Vestavěná funkce CHR */
        else if (token == CHR)
        {
          if((result = chr_function()) != NO_ERR)
            return result;
          codeGen_retvalUse(id_name);
          var_to_symtab(existing_var, id_name, DATA_TYPE_STRING);
          return  NO_ERR;
        }

        /* Přiřazení/Výraz/Volání funkce */
        else if (token == ID)
        {
          strcpy(first_operand_name, TokenList.Active->attr);

          /* Ošetření zvláštního případu: a = a */
          if (id_name == first_operand_name && (st_search(LST, first_operand_name) == NULL))
          {
            TokenList.Active = TokenList.Active->nextItem;
            token = TokenList.Active->token;

            if (token == PLUS || token == MINUS || token == MUL || token == DIV)
              return ERR_SEM;
            else if (token != EOL)
              return ERR_SYNTAX;
            else
            {
              var_to_symtab(existing_var, id_name, DATA_TYPE_NIL);
              return NO_ERR;
            }
          }

          if (st_search(GST, first_operand_name) == NULL && st_search(LST, first_operand_name) == NULL)
            return ERR_SEM;
          else if (st_search(GST, first_operand_name) != NULL)
          {
            func = st_search(GST, first_operand_name);
            var = NULL;
          }
          else
          {
            var = st_search(LST, first_operand_name);
            func = NULL;
          }

          /* Volání funkce */
          if (func != NULL)
          {
            /* Zákaz volání funkce z hlavního těla, pokud nebyla definovaná */
            if (LST == mainBody && func->defined == false)
              return ERR_SEM;

            if ((result = function_call()) != NO_ERR)
              return result;

            codeGen_retvalUse(id_name);
            var_to_symtab(existing_var, id_name, DATA_TYPE_UNKNOWN);

            if(in_function == 1)
            {
              last_line.last = ID;
              strcpy(last_line.name_or_buffer, id_name);
            }

            return NO_ERR;
          }

          else if (var != NULL)
          {
            if ((id_data_type = isIdDefined(first_operand_name)) == ERR_SEM)
              return ERR_SEM;

            if ((result = assigning_or_expression()) != NO_ERR)
              return result;
            codeGen_MoveExprResult(id_name);
            var_to_symtab(existing_var, id_name, top_of_stack.type);

            if(in_function == 1)
            {
              last_line.last = ID;
              strcpy(last_line.name_or_buffer, id_name);
            }

            return NO_ERR;
          }
        }


        /* Přiřazení literálu/Výraz */
        else if (token == STR_LIT || token == INT_LIT || token == FLOAT_LIT || token == NIL)
        {
          if ((result = assigning_or_expression()) != NO_ERR)
            return result;

          if(in_function == 1)
          {
            last_line.last = ID;
            strcpy(last_line.name_or_buffer, id_name);
          }

          codeGen_MoveExprResult(id_name);
          var_to_symtab(existing_var, id_name, top_of_stack.type);

          return NO_ERR;
        }

        else if(token == LEFT_BRACKET)
        {
          if (ss_push(&stack, token, DATA_TYPE_NIL) == ERR_INTERN)
            return ERR_INTERN;

          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if ((result = expression(1)) != NO_ERR)
            return result;

          top_of_stack = *ss_top(&stack);

          if(in_function == 1)
          {
            last_line.last = ID;
            strcpy(last_line.name_or_buffer, id_name);
          }

          codeGen_PopsExpr();
          codeGen_MoveExprResult(id_name);
          var_to_symtab(existing_var, id_name, top_of_stack.type);

          ss_pop(&stack);
          ss_push(&stack, PT_R, DATA_TYPE_NIL);
          return NO_ERR;
        }
        else
          return ERR_SYNTAX;
      }
      else
        return ERR_SYNTAX;

      break;

    case EOL:
      return NO_ERR;
  }
  return ERR_SYNTAX;
}

int function_definition()
{
  int result;
  symtab_elem_t *function;
  symtab_elem_t *new_var;
	int param_num = 0; /* počitadlo parametrů funkce */
  char function_param[100];

  /* Inicializace lokální tabulky pro funkci */
  st_init(&localSymbolTable);

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

	if (token == ID) {

	  strcpy(function_name, TokenList.Active->attr);
	  function = st_search(GST, function_name);
    function->defined = true;
	  LST = localSymbolTable;

		codeGen_func_Start(function_name);
		codeGen_func_RetVal(DATA_TYPE_NIL);

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token == LEFT_BRACKET)
		{
      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

			if (token != ID && token != RIGHT_BRACKET)
				return ERR_SYNTAX;
			else
			{
				while (token != RIGHT_BRACKET)
				{
					if (token == ID)
					{
					  strcpy(function_param, TokenList.Active->attr);

					  param_num++;

						new_var = st_insert(LST, TokenList.Active->attr);
						new_var->e_type = ELEM_TYPE_VAR;
						new_var->d_type = DATA_TYPE_UNKNOWN;

            codeGen_func_ParamDeclaration(function_param, param_num);

            TokenList.Active = TokenList.Active->nextItem;
            token = TokenList.Active->token;

						if (token != RIGHT_BRACKET && token != COMMA)
							return ERR_SYNTAX;
					}

					else if (token == COMMA)
					{
            TokenList.Active = TokenList.Active->nextItem;
            token = TokenList.Active->token;

						if (token != ID)
							return ERR_SYNTAX;
					}
				}

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != EOL)
					return ERR_SYNTAX;
				else
				{
					result = function_body();
					if (result != NO_ERR)
						return result;

					return NO_ERR;
				}
			}
		}
		else
			return ERR_SYNTAX;
	}
	else
		return ERR_SYNTAX;
}

/* Tělo funkce */
int function_body()
{
	int program_result;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  while (token != END)
  {
    if(token == DEF)
      return ERR_SYNTAX;
    program_result = program();
    if (program_result != NO_ERR)
      return program_result;
    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;
  }

  codeGen_return(last_line.last, last_line.name_or_buffer);
  codeGen_func_End(function_name);

  return NO_ERR;
}

/* Tělo podmínky*/
int condition_body()
{
	int program_result;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  while(token != ELSE && token != END)
  {
    if(token == DEF)
      return ERR_SYNTAX;

    program_result = program();
    if (program_result != NO_ERR)
      return program_result;
    TokenList.Active = TokenList.Active->nextItem;
    if ((token = TokenList.Active->token) == ERR_LEX)
      return ERR_LEX;
  }
  if(token == END)
    in_if--;
  return NO_ERR;
}

/* Tělo cyklu */
int cycle_body()
{
	int program_result;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  while(token != END)
  {
    if(token == DEF)
      return ERR_SYNTAX;
    program_result = program();
    if (program_result != NO_ERR)
      return program_result;
    TokenList.Active = TokenList.Active->nextItem;
    if ((token = TokenList.Active->token) == ERR_LEX)
      return ERR_LEX;
  }

  in_while--;
  return NO_ERR;
}


int bool_condition_id()
{
  int result;
  char first_operand_name[100];
  datatype_t id_data_type;
  symtab_elem_t *id_elem;

  strcpy(first_operand_name, TokenList.Active->attr);

  if (st_search(LST, first_operand_name) == NULL ||
      ((id_data_type = isIdDefined(first_operand_name)) == ERR_SEM))
    return ERR_SEM;

  id_elem = st_search(LST, first_operand_name);
  if (id_elem->e_type == ELEM_TYPE_VAR)
  {

    if ((ss_push(&stack, token, id_data_type)) == ERR_INTERN)
      return ERR_INTERN;
    if (token == ID || token == INT_LIT || token == FLOAT_LIT || token == STR_LIT || token == NIL)
      codeGen_pushValue(token, TokenList.Active->attr);

    if (reduce(1))
      return ERR_SYNTAX;

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if ((result = expression(1)) !=
        NO_ERR)
      return result;

    codeGen_PopsBool();

    ss_pop(&stack);
    ss_push(&stack, PT_R, DATA_TYPE_NIL);

    return NO_ERR;
  }
  else
    return ERR_SEM;

  return NO_ERR;
}

int bool_condition_lit()
{
  int result;
  datatype_t id_data_type;

  if (token == INT_LIT)
    id_data_type = DATA_TYPE_INT;
  else if (token == STR_LIT)
    id_data_type = DATA_TYPE_STRING;
  else if (token == NIL)
    id_data_type = DATA_TYPE_NIL;
  else
    id_data_type = DATA_TYPE_DOUBLE;

  if ((ss_push(&stack, token, id_data_type)) == ERR_INTERN)
    return ERR_INTERN;
  if (token == ID || token == INT_LIT || token == FLOAT_LIT || token == STR_LIT || token == NIL)
    codeGen_pushValue(token, TokenList.Active->attr);

  if (reduce(1))
    return ERR_SYNTAX;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  if ((result = expression(1)) !=
      NO_ERR)
    return result;

  codeGen_PopsBool();

  ss_pop(&stack);
  ss_push(&stack, PT_R, DATA_TYPE_NIL);

  return NO_ERR;
}

int input_function()
{
  codeGen_func_PrepPass();

  if(token == INPUTS)
  {
    codeGen_func_Call("inputs");
    if (in_function == 1)
    {
      last_line.last = INPUTS;
      strcpy(last_line.name_or_buffer, "");
    }
  }
  else if(token == INPUTI)
  {
    codeGen_func_Call("inputi");
    if (in_function == 1)
    {
      last_line.last = INPUTI;
      strcpy(last_line.name_or_buffer, "");
    }
  }
  else
  {
    codeGen_func_Call("inputf");
    if (in_function == 1)
    {
      last_line.last = INPUTF;
      strcpy(last_line.name_or_buffer, "");
    }
  }

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  if (token == EOL)
    return NO_ERR;
  else if(token == LEFT_BRACKET)
  {
    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token == RIGHT_BRACKET)
    {
      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token == EOL)
        return NO_ERR;
      else
        return ERR_SYNTAX;
    }
    else
      return ERR_SYNTAX;
  }
  else
    return ERR_SYNTAX;
}



int length_function()
{
  symtab_elem_t *var;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  if (token != LEFT_BRACKET)
    return ERR_SYNTAX;
  else
  {
    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != ID && token != STR_LIT)
      return ERR_SYNTAX;
    else
    {
      codeGen_func_PrepPass();

      if (token == ID)
      { /* s = ID */
        if (st_search(LST, TokenList.Active->attr) == NULL)
          return ERR_SEM;
        var = st_search(LST, TokenList.Active->attr);
        if (var->d_type != DATA_TYPE_STRING)
          return ERR_SEM;

        codeGen_func_PassParam(var,1,1);
      }
      else /* s = STR_LIT */
          codeGen_func_PassParamVal(DATA_TYPE_STRING, TokenList.Active->attr, 1);
    }

    codeGen_func_Call("length");

    if(in_function == 1)
    {
      last_line.last = LENGTH;
      strcpy(last_line.name_or_buffer, "");
    }

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != RIGHT_BRACKET)
      return ERR_SYNTAX;
    else
    {
      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != EOL)
        return ERR_SYNTAX;
      else
        return NO_ERR;
    }
  }
}

int substr_function()
{
  symtab_elem_t *var;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  if (token != LEFT_BRACKET)
    return ERR_SYNTAX;
  else
  {
    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != ID && token != STR_LIT)
      return ERR_SYNTAX;
    else
    {
      codeGen_func_PrepPass();

      if (token == ID)
      { /* s = ID*/
        if (st_search(LST, TokenList.Active->attr) == NULL)
          return ERR_SEM;
        var = st_search(LST, TokenList.Active->attr);
        if (var->d_type != DATA_TYPE_STRING)
          return ERR_SEM;
        codeGen_func_PassParam(var,1,1);
      }
      else
      {
        codeGen_func_PassParamVal(DATA_TYPE_STRING, TokenList.Active->attr, 1);
      }
    }

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != COMMA)
      return ERR_SYNTAX;
    else
    {
      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != ID && token != INT_LIT) /* parametr i - počátek požadovaného podřetězce*/
        return ERR_SYNTAX;
      else
      {
        if (token == ID)
        { /* i = ID */
          if (st_search(LST, TokenList.Active->attr) == NULL)
            return ERR_SEM;
          var = st_search(LST, TokenList.Active->attr);
          if (var->d_type != DATA_TYPE_INT)
            return ERR_SEM;
          codeGen_func_PassParam(var,2,1);
        }
        else
        {
          codeGen_func_PassParamVal(DATA_TYPE_INT, TokenList.Active->attr, 2);
        }
      }

      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != COMMA)
        return ERR_SYNTAX;
      else
      {
        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != ID && token != INT_LIT) /* parametr n - délka podřetězce*/
          return ERR_SYNTAX;
        else
        {
          if (token == ID)
          { /* i = ID */
            if (st_search(LST, TokenList.Active->attr) == NULL)
              return ERR_SEM;
            var = st_search(LST, TokenList.Active->attr);
            if (var->d_type != DATA_TYPE_INT)
              return ERR_SEM;
            codeGen_func_PassParam(var,3,1);
          }
          else
            codeGen_func_PassParamVal(DATA_TYPE_INT, TokenList.Active->attr, 3);
        }

        if(in_function == 1)
        {
          last_line.last = SUBSTR;
          strcpy(last_line.name_or_buffer, "");
        }

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != RIGHT_BRACKET)
          return ERR_SYNTAX;
        else
        {
          TokenList.Active = TokenList.Active->nextItem;
          token = TokenList.Active->token;

          if (token != EOL)
            return ERR_SYNTAX;
          else
          {
            codeGen_func_Call("substr");
            return NO_ERR;
          }
        }
      }
    }
  }
}


int ord_function()
{
  symtab_elem_t *var;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  if (token != LEFT_BRACKET)
    return ERR_SYNTAX;
  else
  {
    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != ID && token != STR_LIT)
      return ERR_SYNTAX;
    else
    {
      codeGen_func_PrepPass();

      if (token == ID)
      { /* s = ID */
        if (st_search(LST, TokenList.Active->attr) == NULL)
          return ERR_SEM;

        var = st_search(LST, TokenList.Active->attr);
        if (var->d_type != DATA_TYPE_STRING)
          return ERR_SEM;
        codeGen_func_PassParam(var,1,1);
      }
      else
      {
        codeGen_func_PassParamVal(DATA_TYPE_STRING, TokenList.Active->attr, 1);
      }
    }

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != COMMA)
      return ERR_SYNTAX;
    else
    {
      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != ID && token != INT_LIT)
        return ERR_SYNTAX;
      else
      {
        if (token == ID)
        { /* i = ID */
          if (st_search(LST, TokenList.Active->attr) == NULL)
            return ERR_SEM;

          var = st_search(LST, TokenList.Active->attr);
          if (var->d_type != DATA_TYPE_INT)
            return ERR_SEM;
          codeGen_func_PassParam(var,2,1);
        }
        else
          codeGen_func_PassParamVal(DATA_TYPE_INT, TokenList.Active->attr, 2);
      }
    }

    codeGen_func_Call("ord");

    if(in_function == 1)
    {
      last_line.last = ORD;
      strcpy(last_line.name_or_buffer, "");
    }

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != RIGHT_BRACKET)
      return ERR_SYNTAX;
    else
    {
      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != EOL)
        return ERR_SYNTAX;
      else
        return NO_ERR;
    }
  }
}

int chr_function()
{
  symtab_elem_t *var;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  if (token != LEFT_BRACKET)
    return ERR_SYNTAX;
  else
  {
    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != ID && token != INT_LIT)
      return ERR_SYNTAX;
    else
    {
      codeGen_func_PrepPass();

      if (token == ID)
      {
        if (st_search(LST, TokenList.Active->attr) == NULL)
          return ERR_SEM;
        var = st_search(LST, TokenList.Active->attr);
        if (var->d_type != DATA_TYPE_INT)
          return ERR_SEM;
        codeGen_func_PassParam(var,1,1);
      }
      else
        codeGen_func_PassParamVal(DATA_TYPE_INT, TokenList.Active->attr, 1);
    }

    codeGen_func_Call("chr");

    if(in_function == 1)
    {
      last_line.last = CHR;
      strcpy(last_line.name_or_buffer, "");
    }

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != RIGHT_BRACKET)
      return ERR_SYNTAX;
    else
    {
      TokenList.Active = TokenList.Active->nextItem;
      token = TokenList.Active->token;

      if (token != EOL)
        return ERR_SYNTAX;
      else
        return NO_ERR;
    }
  }
}

int assigning_or_expression()
{
  int last_token;
  int result;
  symtab_elem_t *var;
  datatype_t literal_data_type;

  if (token == ID)
  {
    if((var = st_search(LST, TokenList.Active->attr)) == NULL)
      return ERR_SEM;
    literal_data_type = var->d_type;
  }
  else if (token == STR_LIT)
    literal_data_type = DATA_TYPE_STRING;
  else if (token == INT_LIT)
    literal_data_type = DATA_TYPE_INT;
  else if (token == NIL)
    literal_data_type = DATA_TYPE_NIL;
  else
    literal_data_type = DATA_TYPE_DOUBLE;

  last_token = token;

  if (ss_push(&stack, token, literal_data_type) == ERR_INTERN)
    return ERR_INTERN;

  codeGen_pushValue(token, TokenList.Active->attr);

  if (reduce(1))
    return ERR_SYNTAX;

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  /* Matematický výraz nebo konkatenace řetězců */
  if (token == PLUS || token == MINUS || token == MUL || token == DIV)
  {
    if ((result = expression(1)) != NO_ERR)
      return result;

    top_of_stack = *ss_top(&stack);

    if(in_function == 1)
    {
      last_line.last = last_token;
      strcpy(last_line.name_or_buffer, " GF@%tmp_op3");
    }

    codeGen_PopsExpr();

    ss_pop(&stack);
    ss_push(&stack, PT_R, DATA_TYPE_NIL);
    return NO_ERR;
  }
  else if (token == EOL)
  {
    top_of_stack = *ss_top(&stack);

    if(in_function == 1)
    {
      last_line.last = last_token;
      strcpy(last_line.name_or_buffer, " GF@%tmp_op3");
    }

    codeGen_PopsExpr();

    ss_pop(&stack);
    ss_push(&stack, PT_R, DATA_TYPE_NIL);
    return NO_ERR;
  }
  else
    return ERR_SYNTAX;
}

int function_call()
{
  int param_num = 0; /* počitadlo parametrů při volání funkce */
  char id_name[100];
  symtab_elem_t *var;
  symtab_elem_t *func;

  strcpy(id_name, TokenList.Active->attr);
  func = st_search(GST, id_name);


  codeGen_func_PrepPass();

  TokenList.Active = TokenList.Active->nextItem;
  token = TokenList.Active->token;

  if (token == LEFT_BRACKET)
  {
    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != ID && token != INT_LIT && token != STR_LIT && token != FLOAT_LIT &&
        token != RIGHT_BRACKET)
      return ERR_SYNTAX;

    while (token != RIGHT_BRACKET)
    {
      if (token == ID || token == INT_LIT || token == STR_LIT || token == FLOAT_LIT)
      {
        param_num++;

        if (token == ID)
        {
          if (st_search(LST, TokenList.Active->attr) == NULL)
            return ERR_SEM;
          var = st_search(LST, TokenList.Active->attr);
          codeGen_func_PassParam(var, param_num, 1);
        }

        else
        {
          if (token == INT_LIT)
            codeGen_func_PassParamVal(DATA_TYPE_INT, TokenList.Active->attr, param_num);
          if (token == STR_LIT)
            codeGen_func_PassParamVal(DATA_TYPE_STRING, TokenList.Active->attr, param_num);
          if (token == FLOAT_LIT)
            codeGen_func_PassParamVal(DATA_TYPE_DOUBLE, TokenList.Active->attr, param_num);
        }

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != RIGHT_BRACKET && token != COMMA)
          return ERR_SYNTAX;
      }

      else /* token == COMMA*/
      {
        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != ID && token != INT_LIT && token != STR_LIT && token != FLOAT_LIT)
          return ERR_SYNTAX;
      }
    }

    TokenList.Active = TokenList.Active->nextItem;
    token = TokenList.Active->token;

    if (token != EOL)
      return ERR_SYNTAX;
    else
    {
      if (param_num != func->param_num)
        return ERR_PARAMS;
    }

    codeGen_func_Call(id_name);

    if(in_function == 1)
    {
      last_line.last = DEF;
      strcpy(last_line.name_or_buffer, "");
    }

    return NO_ERR;
  }

  else if (token == ID || token == INT_LIT || token == STR_LIT || token == FLOAT_LIT || token == EOL)
  {
    while (token != EOL)
    {
      if (token == ID || token == INT_LIT || token == STR_LIT || token == FLOAT_LIT)
      {
        param_num++;

        if (token == ID)
        {
          if (st_search(LST, TokenList.Active->attr) == NULL)
            return ERR_SEM;
          var = st_search(LST, TokenList.Active->attr);
          codeGen_func_PassParam(var, param_num, 1);
        }

        else
        {
          if (token == INT_LIT)
            codeGen_func_PassParamVal(DATA_TYPE_INT, TokenList.Active->attr, param_num);
          if (token == STR_LIT)
            codeGen_func_PassParamVal(DATA_TYPE_STRING, TokenList.Active->attr, param_num);
          if (token == FLOAT_LIT)
            codeGen_func_PassParamVal(DATA_TYPE_DOUBLE, TokenList.Active->attr, param_num);
        }

        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != EOL && token != COMMA)
          return ERR_SYNTAX;

      }
      else /* token == COMMA*/
      {
        TokenList.Active = TokenList.Active->nextItem;
        token = TokenList.Active->token;

        if (token != ID && token != INT_LIT && token != STR_LIT && token != FLOAT_LIT)
          return ERR_SYNTAX;
      }
    }

    if (param_num != func->param_num)
      return ERR_PARAMS;

    codeGen_func_Call(id_name);

    if(in_function == 1)
    {
      last_line.last = DEF;
      strcpy(last_line.name_or_buffer, "");
    }

    return NO_ERR;
  }
  else
    return ERR_SYNTAX;
}


void var_to_symtab(symtab_elem_t *existing_var, char *id_name, datatype_t type)
{
  symtab_elem_t *new_var;

  if (existing_var == NULL)
  {
    /*Uložení nové proměnné do tabulky symbolů*/
    new_var = st_insert(LST, id_name);
    new_var->e_type = ELEM_TYPE_VAR;
    new_var->d_type = type;
  }
  else
  {
    /*Update proměnné v tabulce symbolů*/
    existing_var->d_type = type;
  }
}
