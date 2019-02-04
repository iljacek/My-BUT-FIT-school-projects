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
#include "symtable.h"
#include "symstack.h"
#include "expression_parser.h"
#include "errors.h"


void ss_init(stack_t *s)
{
  s->top = NULL;
  ss_push(s, DOLLAR, DATA_TYPE_NIL);
  ss_push(s, PT_R, DATA_TYPE_NIL);
}

int ss_push(stack_t *s, int symbol, datatype_t datatype)
{
  stack_elem_t *new_item = (stack_elem_t*)malloc(sizeof(stack_elem_t));

  if (new_item == NULL)
    return ERR_INTERN;

  new_item->symbol = symbol;
  new_item->type = datatype;
  new_item->next = s->top;
  s->top = new_item;
  return NO_ERR;

}
void ss_pop(stack_t *s)
{
  if (s->top == NULL)
  {
    return;
  }
  else
  {
    stack_elem_t *tmp = s->top;
    s->top = tmp->next;
    free(tmp);
  }
}

void ss_multiplePop(stack_t *s, int n)
{
  while (n > 0)
  {
    ss_pop(s);
    n--;
  }
}

stack_elem_t* ss_top(stack_t *s)
{
  return s->top;
}

int ss_isTerminal(int symbol)
{
  return (symbol < TOKEN_MAX);
}

stack_elem_t *ss_topTerminal(stack_t *s)
{
  stack_elem_t* tmp = s->top;
  while (tmp != NULL)
  {
    if (ss_isTerminal(tmp->symbol))
      return tmp;
    else
      tmp = tmp->next;
  }
  return NULL;
}

int ss_insertBeforeTopTerminal(stack_t *s, int symbol, datatype_t datatype)
{
  stack_elem_t *tmp = s->top;
  stack_elem_t *prev_item = NULL;


  while (tmp != NULL)
  {
    if (ss_isTerminal(tmp->symbol))
    {
      stack_elem_t *new_item = (stack_elem_t*)malloc(sizeof(stack_elem_t));

      if (new_item == NULL)
        return ERR_INTERN;

      new_item->symbol = symbol;
      new_item->type = datatype;
      if (prev_item == NULL)
      {
        new_item->next = s->top;
        s->top = new_item;
        return NO_ERR;
      }
      else
      {
        new_item->next = prev_item->next;
        prev_item->next = new_item;
        return NO_ERR;
      }
    }
    else
    {
      prev_item = tmp;
      tmp = tmp->next;
    }
  }
  return ERR_INTERN;
}

void ss_free(stack_t *s)
{
  while (s->top)
  {
    ss_pop(s);
  }
}