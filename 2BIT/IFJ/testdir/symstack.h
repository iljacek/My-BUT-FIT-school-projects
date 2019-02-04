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

#ifndef XDACER00_STACK_H
#define XDACER00_STACK_H



typedef struct stack_elem_t {
    int symbol;
    int type;
    struct stack_elem_t *next;
} stack_elem_t;

typedef struct {
    stack_elem_t *top;
} stack_t;

/* Funkce pro implementaci */
void ss_init(stack_t *s);
int ss_push(stack_t *s, int symbol, datatype_t datatype);
void ss_pop(stack_t *s);
void ss_multiplePop(stack_t *s, int n);
stack_elem_t* ss_top(stack_t *s);
int ss_isTerminal(int symbol);
stack_elem_t* ss_topTerminal(stack_t *s);
int ss_insertBeforeTopTerminal(stack_t *s, int symbol, datatype_t data);
void ss_free(stack_t *s);

#endif //XDACER00_STACK_H
