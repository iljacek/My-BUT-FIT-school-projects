
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

  if (ptrht == NULL)
    return;

  for (int i = 0; i < HTSIZE; i++)                                     //každému prvku priradím počiatočnú hodnotu NULL
  {
    (*ptrht)[i] = NULL;
  }

}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

  if (ptrht == NULL || key == NULL)                                 //kontrola či tabulka a kľuč existujú
    return NULL;

  int hash = hashCode(key);
  tHTItem *item = (*ptrht)[hash];                       //generácia hash a následný býber prvku s týmto hashom z tabulky

  while (item != NULL)                                              //pre všetky synonymá skontroluj, či niektoré z nich zodpovedá hľadanému kľúču
  {
    if ((strcmp(key, item->key)) == 0)
      return item;
    item = item->ptrnext;
  }

  return NULL;                                                      //nenašiel sa

}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

  if (ptrht == NULL || key == NULL)                                               //kontrola či tabulka a kľuč existujú
    return;

  int hash = hashCode(key);                                    //získaj hash zo známeho kľúča

  tHTItem *item = htSearch(ptrht, key);                         //vyhladaj ci sa polozka uz nachadza v tabulke
  if (item != NULL)                                             //ak ano, prepis len data
  {
    item->data = data;
  }
  else{
    item = malloc(sizeof(tHTItem));          //alokácia miesta pre novú položku v hashtabulke
    if (item == NULL)
      return;

    if (!(item->key = (char *) malloc((strlen(key) + 1) * sizeof(char))))           //alokácia miesta pre kľúč novej položky
    {      //length of key +1 for NULL
      free(item);
      return;
    }
    item->data = data;
    strcpy(item->key, key);
    item->ptrnext = (*ptrht)[hash];   //if there already is item on hash, make it next element in stack
    (*ptrht)[hash] = item;              //put the new element to the front of stack
  }

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

  if (ptrht == NULL || key == NULL)                                               //kontrola či tabulka a kľuč existujú
    return NULL;

  tHTItem *item = htSearch(ptrht, key);                         //vyhladaj ci sa polozka uz nachadza v tabulke
  if (item != NULL)                                             //ak ano, precitaj data
  {
    return &(item->data);
  }

  return NULL;

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

  if (ptrht == NULL || key == NULL)                     //kontrola či tabulka a kľuč existujú
    return;

  int hash = hashCode(key);
  tHTItem *item = (*ptrht)[hash];
  if (item == NULL)
    return;

  tHTItem *prev_item = NULL;

  if ((strcmp(key, item->key)) == 0)                  //pre prvú položku skontroluj či sa kĺúč zhoduje
  {
    (*ptrht)[hash] = item->ptrnext;                   //premosti na ďalšie synonymum, prípadne NULL
    free(item->key);
    free(item);
    return;
  }

  while (item->ptrnext != NULL)                                //pre ďalšie synonymá
  {
    prev_item = item;                                 //uchovanie ukazovateľa pre prípadné premostenie po vyradení prvku
    item = item->ptrnext;
    if ((strcmp(key, item->key)) == 0)
    {
      prev_item->ptrnext = item->ptrnext;
      free(item->key);
      free(item);
      return;
    }
  }

}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

  if (ptrht == NULL)                              //kontrola či tabuľka existuje
    return;

  tHTItem *item = NULL;
  for (int i = 0; i < HTSIZE; i++)           //pre všetky riadky hashtabuľky
  {
    if ((*ptrht)[i] == NULL)                //skipni riadky, ktoré nemajú žiaden záznam
    {
      continue;
    }
    while ((*ptrht)[i] != NULL)             //maž všetky synonymá
    {
      item = (*ptrht)[i];
      (*ptrht)[i] = item->ptrnext;        //získanie ukazovateľa na ďalšie synonymum v rade/ v prípade posledného znaku NULL
      free(item->key);                            //postupné uvoĺnenie kĺúča, prvku a tabulky
      free(item);
    }
  }

}
