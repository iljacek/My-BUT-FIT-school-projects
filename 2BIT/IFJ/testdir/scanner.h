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

#ifndef XDACER00_SCANNER_H
#define XDACER00_SCANNER_H

#include "str.h"
#include <stdio.h>

extern int tmp;

// Token: END OF FILE
#define END_OF_FILE		100

// Token: END OF LINE
#define EOL		101

// Tokeny: Speciální znaky - ukončení po 0. stavu (case 0)
#define COMMA			      102	// ','
#define LEFT_BRACKET	  103	// '('
#define RIGHT_BRACKET   104	// ')'
#define LEFT_VINCULUM	  105	// '{'
#define RIGHT_VINCULUM	106	// '}'


// Tokeny: Klíčová slova (case 1)
#define DEF		111
#define DO		112
#define ELSE	113
#define END		114
#define IF		115
#define NOT		116
#define NIL		117
#define THEN	118
#define WHILE	119

// Token: Vestavěné funkce
#define INPUTS  120
#define INPUTI  121
#define INPUTF  122
#define PRINT   123
#define LENGTH  124
#define SUBSTR  125
#define ORD     126
#define CHR     127

// Token: Identifikátor
#define ID		128


// Tokeny: Operátory

// Jeden znak
#define PLUS	  129
#define MINUS	  130
#define MUL		  131
#define DIV		  132
#define LESS	  133
#define GREAT	  134
#define ASSIGN	135

// Dva znaky
#define LESS_EQUAL		136
#define GREAT_EQUAL		137
#define EQUAL			    138
#define NOT_EQUAL		  139


// Token: Řetězcový literál
#define STR_LIT		  140


// Token: Celočíselný literál
#define INT_LIT		  141


// Token: Desetinný literál
#define FLOAT_LIT	  142

void setSourceFile(FILE *file);
int getNextToken(string *attr);

#endif
