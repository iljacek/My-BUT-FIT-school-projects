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
#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "errors.h"
#include "code_generator.h"

int main()
{
  FILE *read_file;

  int result;
  int c;

  if ((read_file = fopen("second.txt", "w")) == NULL)
    return ERR_INTERN;


  /* Načtení stdin do souboru, se kterým budeme následně pracovat */
  c = getc(stdin);
  while(c != EOF)
  {
    putc(c, read_file);
    c = getc(stdin);
  }

  fclose(read_file);

  if ((read_file = fopen("second.txt", "r")) == NULL)
    return ERR_INTERN;

  setSourceFile(read_file);

  /* Inicializace tabulky symbolů */
  symtab_t *globalSymbolTable;
  st_init(&globalSymbolTable);

  /* Syntaktická analýza */
  /* 1. Průchod */
  result = first_pass(globalSymbolTable);

  switch (result)
  {
    /* V průběhu 1. průchodu nastala některá z chyb */
    case ERR_LEX:
    case ERR_SYNTAX:
    case ERR_SEM:
    case ERR_TYPE:
    case ERR_PARAMS:
    case ERR_OTHER:
    case ERR_ZERO:
    case ERR_INTERN:

      st_free(globalSymbolTable);
      fclose(read_file);

      return result;

      break;
  }

  fclose(read_file);



  /* 2. Průchod */
  result = second_pass(globalSymbolTable);

  switch (result)
  {
    /* V průběhu 2. průchodu nastala některá z chyb */
    case ERR_LEX:
    case ERR_SYNTAX:
    case ERR_SEM:
    case ERR_TYPE:
    case ERR_PARAMS:
    case ERR_OTHER:
    case ERR_ZERO:
    case ERR_INTERN:

      st_free(globalSymbolTable);

      return result;

      break;
  }

  /* Vyprintování vygenerovaných instrukcí */
  CodelistFirst(&List);
  while(List.Active != NULL){
    printf("%s", List.Active->Instruction);
    List.Active = List.Active->nextItem;
  }
  CodelistFree(&List);

  st_free(globalSymbolTable);

  return NO_ERR;
}

