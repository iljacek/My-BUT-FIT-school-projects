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
 * Knihovna pro práci s nekonečně dlouhými řetězci 
 *
 */

#include <string.h>
#include <malloc.h>
#include "str.h"
#include "errors.h"

int strInit(string *s) // funkce vytvoří nový řetězec
{
   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
      return ERR_INTERN;
   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;
   return NO_ERR	;
}

void strFree(string *s) // funkce uvolní řetězec z paměti
{
   free(s->str);
}

void strClear(string *s) // funkce vymaže obsah řetězce
{
   s->str[0] = '\0';
   s->length = 0;
}

int strAddChar(string *s1, char c) // přidá na konec řetězce jeden znak
{
   if (s1->length + 1 >= s1->allocSize)
   {
      if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
         return ERR_INTERN;
      s1->allocSize = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';
   return NO_ERR	;
}


int strCmpConstStr(string *s1, char* s2) // porovná náš řetězec s konstantním řetězcem
{
   return strcmp(s1->str, s2);
}