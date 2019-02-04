
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

  L->First = NULL;
  L->Act = NULL;
  L->Last = NULL;
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

  while (L->First != NULL)              //while there is any active element left
  {
    DLDeleteFirst(L);
  }

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  tDLElemPtr firstElemPtr = (struct tDLElem*) malloc(sizeof(struct tDLElem));
  if (firstElemPtr == NULL)         //if the allocation was faulty, execute Error() function
	  DLError();
  else
  {
    firstElemPtr->data = val;         //pass new data
    firstElemPtr->lptr = NULL;        //there is no element before the first element
    firstElemPtr->rptr = L->First;    //new first element will point to the former one
    if (L->Last == NULL)              //if the new element is only element in list mark it also as last element
      L->Last = firstElemPtr;
    else                              //if not, the former fist element will point left to the new one
      L->First->lptr = firstElemPtr;
    L->First = firstElemPtr;          //new element is marked as First
  }


 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  tDLElemPtr lastElemPtr = (struct tDLElem*) malloc(sizeof(struct tDLElem));
  if (lastElemPtr == NULL)         //if the allocation was faulty, execute Error() function
    DLError();
  else
  {
    lastElemPtr->data = val;         //pass new data
    lastElemPtr->rptr = NULL;        //there is no element after the last element
    lastElemPtr->lptr = L->Last;     //new last element will point to the former one
    if (L->First == NULL)            //if the new element is only element in list mark it also as first element
      L->First = lastElemPtr;
    else                             //if not, the former last element will point right to the new one
      L->Last->rptr = lastElemPtr;
    L->Last = lastElemPtr;           //new element is marked as First
  }
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

  L->Act = L->First;

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

  L->Act = L->Last;
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

  if (L->First == NULL)               //if the list is empty, execute Error() function
    DLError();
  else                                //otherwise, copy active element value to *val pointer
  {
    *val = L->First->data;
  }
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

  if (L->Last == NULL)               //if the list is empty, execute Error() function
    DLError();
  else                                //otherwise, copy active element value to *val pointer
  {
    *val = L->Last->data;
  }

  //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

  if (L->First != NULL)                         //execute only if there is something to delete
  {
    if (L->First == L->Act)
    {
      L->Act = NULL;
    }
    tDLElemPtr deletedElementPtr = L->First;     //point to the element to delete
    L->First = deletedElementPtr->rptr;          //pass the first element pointer to next element after the deleted one
    if (L->First == NULL)                        //if the deleted element was the last element, set Last to NULL as well
    {
      L->Last = NULL;
    }
    else                                         //if not, make the new first elements left pointer point to NULL
    {
      L->First->lptr = NULL;
    }
    free(deletedElementPtr);
  }

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

  if (L->Last != NULL)                         //execute only if there is something to delete
  {
    if (L->Last == L->Act)
    {
      L->Act = NULL;
    }
    tDLElemPtr deletedElementPtr = L->Last;     //point to the element to delete
    L->Last = deletedElementPtr->lptr;          //pass the last element pointer to the element before the deleted one
    if (L->Last == NULL)                        //if deleted element was the first element, set First to NULL as well
    {
      L->First = NULL;
    }
    else                                         //if not, make the new first elements right pointer point to NULL
    {
      L->Last->rptr = NULL;
    }
    free(deletedElementPtr);
  }
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

  if (L->Act != NULL && L->Act->rptr != NULL)     //execute only if there is something to delete
  {
    tDLElemPtr deletedElementPtr = L->Act->rptr;  //point to the element to delete
    L->Act->rptr = deletedElementPtr->rptr;       //pass the active element pointer to next element after deleted one
    if (L->Last == deletedElementPtr)             //if deleted element was the last one, set Last to active element
    {
      L->Last = L->Act;
    }
    else                                          //if not, make the following element point back to the active element
    {
      deletedElementPtr->rptr->lptr = L->Act;
    }
    free(deletedElementPtr);
  }

  //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

  if (L->Act != NULL && L->Act->lptr != NULL)     //execute only if there is something to delete
  {
    tDLElemPtr deletedElementPtr = L->Act->lptr;  //point to the element to delete
    L->Act->lptr = deletedElementPtr->lptr;       //pass the active element pointer to next element after deleted one
    if (L->First == deletedElementPtr)            //if deleted element was the first one, set First to active element
    {
      L->First = L->Act;
    }
    else                                          //else, make the previous element point forward to the active element
    {
      deletedElementPtr->lptr->rptr = L->Act;
    }
    free(deletedElementPtr);
  }

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  if (L->Act != NULL)                     //execute only if list is active
  {
    tDLElemPtr newElementPtr = (struct tDLElem *) malloc(sizeof(struct tDLElem));
    if (newElementPtr == NULL)            //if the allocation was faulty, execute Error() function
      DLError();
    else {
      newElementPtr->data = val;          //pass new data
      newElementPtr->lptr = L->Act;       //left pointer of the new element points to the active element
      newElementPtr->rptr = L->Act->rptr; //right pointer of new element points where active element used to point
      L->Act->rptr = newElementPtr;       //active element points right to the new element
      if (L->Act == L->Last)        //if the new element is last element in list mark it as last element
      {
        L->Last = newElementPtr;
      }
      else                                //if not, make the following element point back to the new element
      {
        newElementPtr->rptr->lptr = newElementPtr;
      }
    }

  }

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  if (L->Act != NULL)                     //execute only if list is active
  {
    tDLElemPtr newElementPtr = (struct tDLElem *) malloc(sizeof(struct tDLElem));
    if (newElementPtr == NULL)            //if the allocation was faulty, execute Error() function
      DLError();
    else {
      newElementPtr->data = val;          //pass new data
      newElementPtr->rptr = L->Act;       //right pointer of the new element points to the active element
      newElementPtr->lptr = L->Act->lptr; //left pointer of new element points where active element used to point
      L->Act->lptr = newElementPtr;       //active element points left to the new element
      if (L->Act == L->First)       //if the new element is first element in list mark it as first element
      {
        L->First = newElementPtr;
      }
      else                                //if not, make the previous element point forward to the new element
      {
        newElementPtr->lptr->rptr = newElementPtr;
      }
    }

  }
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

  if (L->Act == NULL)                //if the list is inactive, execute Error() function
    DLError();
  else                               //otherwise, copy active element value to *val pointer
  {
    *val = L->Act->data;
  }
	
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

  if (L->Act != NULL)                //execute only if element is active
  {
    L->Act->data = val;
  }
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

  if (L->Act != NULL)                //execute only if element is active
  {
    L->Act = L->Act->rptr;
  }
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

  if (L->Act != NULL)                //execute only if element is active
  {
    L->Act = L->Act->lptr;
  }
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

  return (L->Act != NULL);
	
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

/* Konec c206.c*/
