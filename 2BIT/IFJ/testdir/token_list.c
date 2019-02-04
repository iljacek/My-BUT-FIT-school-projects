/**
 *
 * IFJ Projekt 2018
 *
 * xdacer00: Ondřej Dacer
 * xzitny01: Roland Žitný
 * xnovot1l: Jakub Novotný
 * xsvehl09: Lubomír Švehla
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_list.h"

	void TokenlistInit(tTokenList *L) {
		L->First = NULL;
		L->Last = NULL;
	}

  void TokenStart() {
      TokenlistInit(&TokenList);
  }

	void TokenlistFree(tTokenList *L) {
		tTokenElem *ptr;
		while (L->First != NULL) {
			ptr = L->First;
			L->First = L->First->nextItem;
      		free(ptr->attr);
			free(ptr);
		}
	}

	void TokenListInsertLast(tTokenList *L, char *attr_in,int token_in)
	{
    	tTokenElem *newItem;
    	newItem = malloc(sizeof(tTokenElem));
			if (newItem == NULL)
				return;
			newItem->attr = malloc(sizeof(char)*(strlen(attr_in)+1));
			if (newItem->attr == NULL)
				return;

      strcpy(newItem->attr,attr_in);
      newItem->token = token_in;

    	newItem->nextItem = NULL;

    	if (L->First == NULL)
      		L->First = newItem;
    	else
      	L->Last->nextItem = newItem;
    		L->Last = newItem;
  	}

  void TokenlistFirst(tTokenList *L) {
    L->Active = L->First;
  }
