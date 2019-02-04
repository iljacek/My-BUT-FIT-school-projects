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

#ifndef _TOKEN_LIST_H
#define _TOKEN_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "str.h"

typedef struct TokenItem
{
	int token;
	char *attr;
	struct TokenItem *nextItem;
} tTokenElem;

typedef struct
{
	struct TokenItem *First;
	struct TokenItem *Last;
	struct TokenItem *Active;
} tTokenList;


tTokenList TokenList;

void TokenStart();
void TokenlistInit(tTokenList *L);
void TokenlistFree(tTokenList *L);
void TokenListInsertLast(tTokenList *L, char *attr_in, int token_in);
void TokenlistFirst(tTokenList *L);

#endif