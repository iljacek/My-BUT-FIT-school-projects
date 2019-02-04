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

#include <memory.h>
#include <stdio.h>
#include "symtable.h"

unsigned int hash_function(const char *str)
{
  unsigned int hash = 0;
  int c;
  unsigned char *p = (unsigned char *) str;

  while ((c = *p++))                                     //postupne beriem všetky znaky hashovaného stringu až prídem po NULL
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash % TABLE_SIZE;                            //vysledne číslo normujem na veľkosť hashovacej tabulky
}

symtab_t *st_init(symtab_t **table)
{
  (*table) = (symtab_t *) malloc(sizeof(symtab_elem_t) * TABLE_SIZE);         //alokácia miesta pre tabulku
  if ((*table) == NULL)
  {
    fprintf(stderr, "ERROR: internal error of Compiler (allocation error)");
    exit(99);
  }

  for (int i = 0; i < TABLE_SIZE; i++)                                     //každému prvku priradím počiatočnú hodnotu NULL
  {
    (*table)->element[i] = NULL;
  }

  return (*table);
}

symtab_elem_t *st_insert(symtab_t *table, char *key)
{
  if (table == NULL || key == NULL)
    return NULL;

  unsigned int hash = hash_function(key);                                    //získaj hash zo známeho kľúča

  symtab_elem_t *item = (symtab_elem_t *) malloc(sizeof(symtab_elem_t));          //alokácia miesta pre novú položku v hashtabulke
  if (item == NULL)
  {
    fprintf(stderr, "ERROR: internal error of Compiler (allocation error)");
    exit(99);
  }
  if (!(item->key = (char *) malloc((strlen(key) + 1) * sizeof(char))))           //alokácia miesta pre kľúč novej položky
  {                                                                               
    free(item);
    fprintf(stderr, "ERROR: internal error of Compiler (allocation error)");
    exit(99);
  }
  strcpy(item->key, key);
  item->next_item = table->element[hash];   //ak už na danom hashi je prvok, tak ho posuň na ďalšiu pozíciu
  table->element[hash] = item;              //vlož novú hodnotu na začiatok zásobníku

  item->childTable = NULL;

  return item;
}

symtab_elem_t *st_search(symtab_t *table, char *key)
{
  if (table == NULL || key == NULL)
    return NULL;

  unsigned int hash = hash_function(key);
  symtab_elem_t *item = table->element[hash];                       //generácia hash a následný býber prvku s týmto hashom z tabulky

  while (item != NULL)                                              //pre všetky synonymá skontroluj, či niektoré z nich zodpovedá hľadanému kľúču
  {
    if (strcmp(item->key, key) == 0)
      return item;
    item = item->next_item;
  }
  return NULL;
}

void st_free(symtab_t *table)
{
  if (table == NULL)
    return;

  symtab_elem_t *item = NULL;
  for (int i = 0; i < TABLE_SIZE; i++)           //pre všetky riadky hashtabuľky
  {
    if (table->element[i] == NULL)                //skipni riadky, ktoré nemajú žiaden záznam
    {
      continue;
    }
    while (table->element[i] != NULL)             //maž všetky synonymá
    {
      item = table->element[i];
      if (item->childTable)                       //ak má tabuľka podriadene tabuľky, vymaž najskôr rekurzívne tie
        st_free(item->childTable);
      table->element[i] = item->next_item;        //získanie ukazovateľa na ďalšie synonymum v rade/ v prípade posledného znaku NULL
      free(item->key);                            //postupné uvoĺnenie kĺúča, prvku a tabulky
      free(item);
    }
  }
  free(table);
}