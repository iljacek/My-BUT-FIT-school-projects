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
#ifndef XDACER00_STR_H
#define XDACER00_STR_H

#define STR_LEN_INC 8 // počet bytů, na kolik provedeme počáteční alokaci paměti, pokud načítáme řetězec znak po znaku, paměť se bude postupně alokovat na násobky tohoto čísla
 
typedef struct
{
  char* str;		// místo pro daný řetězec ukončený znakem '\0'
  int length;		// skutečná délka řetězce
  int allocSize;	// velikost alokované paměti
} string;

int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strCmpConstStr(string *s1, char *s2);

#endif