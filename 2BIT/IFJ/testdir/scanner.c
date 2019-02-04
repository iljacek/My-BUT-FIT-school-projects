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
 * Lexikální analyzátor
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "str.h"
#include "scanner.h"
#include "errors.h"

FILE *source;
int tmp = 0;
void setSourceFile(FILE *file)
{
  source = file;
  ungetc('\n', source);
}


int getNextToken(string *attr)
{
  int state = 0;
  int c;

  strClear(attr); /* vymažeme obsah atributu a v případě identifikátoru budeme postupně do něj vkládat jeho název */

  int less_switch = 0; /* na vstupu '<', musíme zkontrolovat, zda další znak není '=', pokud ano - LESS_EQUAL, pokud ne - LESS */
  int great_switch = 0; /* na vstupu '>', musíme zkontrolovat, zda další znak není '=', pokud ano - GREAT_EQUAL, pokud ne - GREAT */
  int assign_switch = 0; /* na vstupu '=', musíme zkontrolovat, zda další znak není '=', pokud ano - EQUAL, pokud ne - ASSIGN */
  int exclam_switch = 0; /* na vstupu '!', musíme zkontrolovat, zda další znak není '=', pokud ano - NOT_EQUAL, pokud ne - ERR_LEX */

  int backslash_switch = 0; /* signalizace speciálního znaku (např. \n, \t, \s, \\, \") */
  int hex_num_count = 0; /* identifikace '\x' a následných dvou hexadecimálních znaků */

  int multiline_com = 0; /* načetli jsme '=begin ' a jsme uvnitř blokového komentáře a hledáme '\n=end '*/
  int end_multiline_com = 0; /* už víme, že jsme na konci blokového komentáře, ale potřebujeme všechny znaky z posledního řádku přeskočit*/
  int comm_end = 0; /* pomocná proměnná pro ukončení blokového komentáře (aby se necyklil =end)*/

  char tmpattr[3];  /* Pole pro práci s převodem hex čísel uvnitř řetězce */
  tmpattr[2]= '\0';

  while (1) {

    c = getc(source); /* načtení dalšího znaku */

    if (tmp == 1)
        state = 8;


    switch (state)
    {
      /* Počíteční stav automatu */
      case 0:

        if (c == '\n') /* nový řádek */
          {
              c = getc(source);

              if(c == '=') /* možný blokový komentář */
              {
                  tmp = 1;
                  ungetc(c, source);
                  return EOL;
              }
              else if (c == '\n')
              {
                  tmp = 0;
                  ungetc(c, source);
                  state = 0;
              }
              else
              {
                ungetc(c, source);
                return EOL;
              }

          }

        else if (isspace(c)) /* ignorování bílých míst */
          state = 0;
        else if (c == ',')
          return COMMA;
        else if (c == '(')
          return LEFT_BRACKET;
        else if (c == ')')
          return RIGHT_BRACKET;
        else if (c == '{')
          return LEFT_VINCULUM;
        else if (c == '}')
          return RIGHT_VINCULUM;
          /* state = 0: jednoznakové operátory */
        else if (c == '+')
          return PLUS;
        else if (c == '-')
          return MINUS;
        else if (c == '*')
          return MUL;
        else if (c == '/')
          return DIV;

        /* state = 1 */
        else if ((isalpha(c) && islower(c)) || c == '_')/* identifikátor nebo klíčové slovo */
        {
          strAddChar(attr, c);
          state = 1;
        }

        /* state = 2: Možné dvouznakové operátory */
        else if (c == '<')
        {
          less_switch = 1;
          state = 2;
        }
        else if (c == '>')
        {
          great_switch = 1;
          state = 2;
        }

        else if (c == '=')
        {
          assign_switch = 1;
          state = 2;
        }
        else if (c == '!')
        {
          exclam_switch = 1;
          state = 2;
        }


        /* state = 3: řetězcové literály */
        else if (c == '"')
          state = 3;

        /* state = 4: celočíselné literály a desetinné literály */
        else if (isdigit(c)) {
          strAddChar(attr, c);
          if(c == '\x30'){
            c = getc(source);
            if(isdigit(c))
              return ERR_LEX;
            else
              ungetc(c, source);
          }
          state = 4;
        }

        /* state = 5: desetinné literály (začátek tečkou) */
        else if (c == '.')
        {
          strAddChar(attr, c);
          state = 5;
        }
        /* Přímo do state = 6 nejde přejít */
        /* state = 7: jednořádkový komentář */
        else if (c == '#')
          state = 7;

          // EOF
        else if (c == EOF)
          return END_OF_FILE;
          // Lexikální chyba
        else
          return ERR_LEX;

        break;

      /* ID, vestavěná funkce nebo klíčové slovo */
      case 1:
        if (isalnum(c) || c == '_' || c == '!' || c == '?'){ /* identifikátor pokračuje */
          strAddChar(attr, c);
          state = 1;
        }
        else{ /* Konec identifikátoru */
          ungetc(c, source); /* vrácení posledního načteného znaku */
          state = 0;
          /* Kontrola, zda se nejedná o klíčove slovo nebo vestavěnou funkci */
          if (strCmpConstStr(attr, "def") == 0)       /* klíčová slova */
            return DEF;
          else if (strCmpConstStr(attr, "do") == 0)
            return DO;
          else if (strCmpConstStr(attr, "else") == 0)
            return ELSE;
          else if (strCmpConstStr(attr, "end") == 0)
            return END;
          else if (strCmpConstStr(attr, "if") == 0)
            return IF;
          else if (strCmpConstStr(attr, "not") == 0)
            return NOT;
          else if (strCmpConstStr(attr, "nil") == 0)
            return NIL;
          else if (strCmpConstStr(attr, "then") == 0)
            return THEN;
          else if (strCmpConstStr(attr, "while") == 0)
            return WHILE;
          else if (strCmpConstStr(attr, "inputs") == 0)   /* vestavěné funkce */
            return INPUTS;
          else if (strCmpConstStr(attr, "inputi") == 0)
            return INPUTI;
          else if (strCmpConstStr(attr, "inputf") == 0)
            return INPUTF;
          else if (strCmpConstStr(attr, "print") == 0)
            return PRINT;
          else if (strCmpConstStr(attr, "length") == 0)
            return LENGTH;
          else if (strCmpConstStr(attr, "substr") == 0)
            return SUBSTR;
          else if (strCmpConstStr(attr, "ord") == 0)
            return ORD;
          else if (strCmpConstStr(attr, "chr") == 0)
            return CHR;
          else{ /* jednalo se o identifikátor */

            /* Kontrola, zda někde uprostřed není '!' nebo '?' - znak na indexu 0 není třeba načítat, kontrolou prošel už ve state 0
             rovněž není třeba procházet poslední znak řetězce '\0' i poslední, protože ten může být '!' i '?' */
            for (int i = 1; i < attr->length - 1; i++){
              if (attr->str[i] == '!' || attr->str[i] == '?')
                return ERR_LEX;
            }
            return ID;
          }
        }

        break;

      /* Operátory */
      case 2:

        if (c == '=' && less_switch == 1){
          less_switch = 0;
          return LESS_EQUAL;
        }
        else if (c != '=' && less_switch == 1) {
          ungetc(c, source);
          less_switch = 0;
          return LESS;
        }

        if (c == '=' && great_switch == 1){
          great_switch = 0;
          return GREAT_EQUAL;
        }
        else if (c != '=' && great_switch == 1) {
          ungetc(c, source);
          great_switch = 0;
          return GREAT;
        }

        if (c == '=' && assign_switch == 1){
          assign_switch = 0;
          return EQUAL;
        }

        else if (c == 'b' && assign_switch == 1)
            return ERR_LEX;
        else if (c != '=' && assign_switch == 1) {
          ungetc(c, source);
          assign_switch = 0;
          return ASSIGN;
        }

        if (c == '=' && exclam_switch == 1){
          exclam_switch = 0;
          return NOT_EQUAL;
        }
        else if (c != '=' && exclam_switch == 1) {
          ungetc(c, source);
          exclam_switch = 0;
          return ERR_LEX;
        }

        break;


      /* Řetězcový literál */
      case 3:
        if (backslash_switch == 0 && c == '"') /* ukončení řetězce uvozovkou */
          return STR_LIT;

        else if(c == '\x23' || c == '#') /* nahrazení '#' */
        {
            strAddChar(attr, '\\');
            strAddChar(attr, '0');
            strAddChar(attr, '3');
            strAddChar(attr, '5');
            state = 3;
        }

        else if (backslash_switch == 0 && c == '\x5c') {  /* nahrazení '\' */
          backslash_switch = 1;
          state = 3;
        }
        else if (backslash_switch == 1 && c == 'n') { /* nahrazení '\n' */
          strAddChar(attr, '\\');
          strAddChar(attr, '0');
          strAddChar(attr, '1');
          strAddChar(attr, '0');
          backslash_switch = 0;
          state = 3;
        }
        else if (backslash_switch == 1 && c == 't') { /* nahrazení '\t' */
          strAddChar(attr, '\\');
          strAddChar(attr, '0');
          strAddChar(attr, '0');
          strAddChar(attr, '9');
          backslash_switch = 0;
          state = 3;
        }
        else if ((backslash_switch == 1 && c == 's')|| c == ' ') { /* nahrazení mezery */
          strAddChar(attr, '\\');
          strAddChar(attr, '0');
          strAddChar(attr, '3');
          strAddChar(attr, '2');
          backslash_switch = 0;
          state = 3;
        }


        else if (backslash_switch == 1 && c == '\x5c') { /* nahrazení '\\' */
          strAddChar(attr, '\\');
          strAddChar(attr, '0');
          strAddChar(attr, '9');
          strAddChar(attr, '2');
          backslash_switch = 0;
          state = 3;
        }
        else if (backslash_switch == 1 && c == '"') { /* nahrazení '\"' */
          strAddChar(attr, '\\');
          strAddChar(attr, '0');
          strAddChar(attr, '3');
          strAddChar(attr, '4');
          backslash_switch = 0;
          state = 3;
        }
        else if (backslash_switch == 1 && c == 'x') { /* hexadecimální číslo */
          backslash_switch = 0;

          state = 9;
        }
        else if (c == '\n' && backslash_switch == 0) /* řetězec musí být zapsán na jednom řádku */
          return ERR_LEX;
        else {
          strAddChar(attr, c);
          backslash_switch = 0;
          state = 3;
        }
        break;


      /* Celočíselné literály */
      case 4:
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '<' || c == '>' || c == ')' ||
            c == ',' || isspace(c) || c == EOF){
          ungetc(c, source);
          return INT_LIT;
        }
        else if (c == '.') {
          strAddChar(attr, c);
          state = 5;
        }
        else if (c == 'e' || c == 'E') {
          strAddChar(attr, c);
          state = 6;
        }
        else if (isdigit(c)) {
          strAddChar(attr, c);
          state = 4;
        }
        else
        {
            return ERR_LEX;
        }

        break;



      /* Desetinné literály */
      case 5:
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '<' || c == '>' || c == ')' ||
            c == ',' || isspace(c) || c == EOF){
          ungetc(c, source);
          return FLOAT_LIT;
        }
        else if (c == 'e' || c == 'E') {
          /* Kontrola, zda se bezprostředně za '.' nebude nacházet 'e'/'E', pokud ano - ERR_LEX (např. 10.e15) */
          if (attr->str[attr->length-1] == '.')
            return ERR_LEX;
          else {
            strAddChar(attr, c);
            state = 6;
          }
        }
        else if (isdigit(c)) {
          strAddChar(attr, c);
          state = 5;
        }
        else
        {
            return ERR_LEX;
        }

        break;

      /* Desetinné literály s exponentem */
      case 6:
        if ((c == '+' || c == '-' || isdigit(c)) && (attr->str[attr->length-1] == 'e' || attr->str[attr->length-1] == 'E')) {
          if (c != '\x30') /* kontrola před například 5e05 - první číslo za 'e'/'E' nesmí být 0 */
            strAddChar(attr, c);
          state = 6;
        }
        else if ((c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '<' || c == '>' || c == ')' ||
                  c == ',' || isspace(c) || c == EOF) && (isdigit(attr->str[attr->length-1]))) {
          ungetc(c, source);
          return FLOAT_LIT;
        }
        else if (isdigit(c)) {
          strAddChar(attr, c);
          state = 6;
        }
        else
        {
            return ERR_LEX;
        }

        break;

      /* Jednořádkový komentář */
      case 7:

        if (c == '\n')
        {
          ungetc(c, source);
          return EOL;
        }
        else if (c == EOF)
            return END_OF_FILE;
        else
          state = 7;

        break;


      /* Víceřádkový komentář */
      case 8:
        if (c == EOF && comm_end == 0) {return ERR_LEX;} /* kontrola na neukončený komentář */
        if (multiline_com == 0) {
                tmp = 0;
                /* Kontrola =begin */
          if (c == '=')
          {
            strAddChar(attr, c);
          }
          else if (attr->str[attr->length-1] == '=' && c == 'b')
          {
            strClear(attr);
            strAddChar(attr, c);
          }
          else if (attr->str[attr->length-1] == 'b' && c == 'e') {
            strAddChar(attr, c);
            state = 8;
          }
          else if (attr->str[attr->length-1] == 'e' && c == 'g') {
            strAddChar(attr, c);
            state = 8;
          }
          else if (attr->str[attr->length-1] == 'g' && c == 'i') {
            strAddChar(attr, c);
            state = 8;
          }
          else if (attr->str[attr->length-1] == 'i' && c == 'n') {
            strAddChar(attr, c);
            state = 8;
          }
          else if (attr->str[attr->length-1] == 'n' && isspace(c)) {
            strClear(attr); /*dále buffer nepotřebujeme, víme, že jsme načetli '=begin ' a nastavíme multiline_com na 1 */
            multiline_com = 1;
            state = 8;
          }
          else
            return ERR_LEX;

        }
        /* Kontrola =end */
        else {
          if (end_multiline_com == 0) {

            if (c == '\n' && comm_end == 0) {
              strAddChar(attr, c);
              state = 8;
            }
            else if (attr->str[attr->length-1] == '\n' && c == '=') {
              strAddChar(attr, c);
              state = 8;
            }
            else if (attr->str[attr->length-1] == '=' && c == 'e') {
              strAddChar(attr, c);
              state = 8;
            }
            else if (attr->str[attr->length-1] == 'e' && c == 'n') {
              strAddChar(attr, c);
              state = 8;
            }
            else if (attr->str[attr->length-1] == 'n' && c == 'd') {
              strAddChar(attr, c);
              /* Pro případ end bez '=' nebo '\n' */
              if ((attr->str[attr->length-1] == 'd' && attr->str[attr->length-2] == 'n' && attr->str[attr->length-3] == 'e') && (attr->str[attr->length-4] != '=' || attr->str[attr->length-5] != '\n'))
              {   strClear(attr);
                  state = 8;
                  comm_end = 0;
                  end_multiline_com = 0;
              }
              else
              {
              state = 8;
              comm_end = 1;
              }

            }
            else if (attr->str[attr->length-1] == 'd' && isspace(c)) {
              strAddChar(attr, c);
              end_multiline_com = 1;
              state = 8;
            }

            else if (c == EOF && attr->str[attr->length-1] == 'd' && attr->str[attr->length-2] == 'n' && attr->str[attr->length-3] == 'e' && attr->str[attr->length-4] == '=')
            {
                strClear(attr);
                return END_OF_FILE;
            }
            else {
              if (c == EOF) {return ERR_LEX;}  /*TEST na neukonecny komentář v cyklu komentáře */
              strAddChar(attr, c);
              state = 8;


            }
          }
          /* Část za =end a poslední kontrola validního '\n=end'*/
          else
          {
          if (attr->str[attr->length-1] == 'd' && attr->str[attr->length-2] == 'n' && attr->str[attr->length-3] == 'e' && attr->str[attr->length-4] == 'd'){
            if(attr->str[attr->length-5] != '\n')
               return ERR_LEX;
          }
          else if (attr->str[attr->length-1] == '\n') {
            ungetc(c, source);
            state = 0;
            comm_end = 0;
            multiline_com = 0;
            end_multiline_com = 0;
            strClear(attr);
          }
          else {
            strAddChar(attr, c);
            state = 8;

            if(end_multiline_com == 1 && c == '\n')
            {
            state = 0;
            comm_end = 0;
            multiline_com = 0;
            end_multiline_com = 0;
            strClear(attr);
            }
          }
        }
    }
        break;

      case 9:
        if(isxdigit(c) && (hex_num_count < 2)) { /* stav po /x    - očekáváme 2hex čísla. (v bufferu je /, x tam není) */
          if(hex_num_count == 0) /* přidá se první popř. druhé číslo po x do bufferu */
            tmpattr[0] = c;
          else if(hex_num_count == 1)
            tmpattr[1] = c;
          hex_num_count++;
        } else if (hex_num_count == 0) {
          return ERR_LEX;
        } else {
          int i =(int)strtol(tmpattr, NULL, 16); /* do i uložím hex hodnotu čísla po X (ASCII) */
          ungetc(c, source); /* vrátíme znak, který máme načtený */
          if (i >= 0) {
            strAddChar(attr, i); /* Načteme do bufferu i */
            hex_num_count = 0;
          } else {
            return ERR_LEX;
          }
          state = 3;
        }
        break;
    }
   }
  }

