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

#ifndef XDACER00_PARSER_H
#define XDACER00_PARSER_H

#include "symtable.h"

typedef struct LastLine
{
    int last;
    char name_or_buffer[100];
} tLastLine;

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

#endif