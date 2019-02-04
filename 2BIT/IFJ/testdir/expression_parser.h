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

#ifndef XDACER00_SEMANTIC_H
#define XDACER00_SEMANTIC_H

#include "parser.h"
#include "symstack.h"
#include "symtable.h"
#include "str.h"

///symbols not included in scanner
#define DOLLAR 160
#define TOKEN_MAX 161
#define NON_TERMINAL 162

///types of expressions
#define    EP_ARITMETIC 171       /// *, +, -, /
#define    EP_REL 122             /// <, <=, >, >=, ==, !=
#define    EP_BRACKETS  173       /// (E)
#define    EP_OPERAND 174         /// op

/**
 * Definovanie veľkosti a obsahu precedenčnej tabuľky
 */

#define PT_SIZE 8

typedef enum
{
    PT_N = NON_TERMINAL+1,    /// # none
    PT_R,                     /// < right
    PT_E,                     /// = equal
    PT_L,                     /// > left
} PT_precedence;


/**
 * @brief Function to obtain collumn or row index in precision table
 * @param token token to be looked up
 * @return index corresponding to token from input
 */
int mapToken(int token);

/**
 * @brief Function that checks how many symbols are there between top of stack and the first mark
 * @return number of symbols
 */
int symbolsBeforeMark();

/**
 * @brief Function that looks for the rule used in expression based on number of operands on input ant their value
 * @param count number of operands
 * @param op1 typically literal, nil, id, bracket or non-terminal
 * @param op2 typically non-terminal or mathematical/boolean operator
 * @param op3 typically literal, nil, id, bracket or non-terminal
 * @return class of rule that was used
 */
int usedRule(int count, stack_elem_t* op1, stack_elem_t* op2, stack_elem_t* op3);

/**
 * @brief Function, which checks if rule corresponds with types of its parameters
 * @param rule class of the rule, that is used in expression
 * @param op1 typically literal, nil, id, bracket or non-terminal
 * @param op2 typically non-terminal or mathematical/boolean operator
 * @param op3 typically literal, nil, id, bracket or non-terminal
 * @param final_type pointer, that contains type of the final non-terminal after the application of rule
 * @param generate_flag flag, that identifies if instructions should be generated
 * @return error type
 */
int checkSemantics(int rule, stack_elem_t *op1, stack_elem_t *op2, stack_elem_t *op3, datatype_t *final_type, int generate_flag);

/**
 * @brief Function, which applicates the reduction to expressions
 * @param generate_flag flag, that identifies if instructions should be generated
 * @return error type
 */
int reduce(int generate_flag);

/**
 * @brief Function, which checks data type of id, if it is already defined
 * @param id_name name of ID
 * @return data type of ID, or semantic error
 */
int isIdDefined(char* id_name);

/**
 * @brief Function, which captures type of the used literal
 * @param token identification number of token
 * @return data type of literal
 */
int getLiteralType(int token);

/**
 * @brief Check if expression after the slash is zero
 * @param value value of the expression
 * @param symbol symbol read from TokenList
 * @param zero_flag indicates if counting for zero should continue
 * @return error type
 */
int computeForZero(double *value, int symbol, int *zero_flag);

/**
 * @brief Main function of expression parser, which reads whole expression and reduces it by using other functions
 * @param generate_flag flag, that identifies if instructions should be generated
 * @return error type
 */
int expression(int generate_flag);

#endif //XDACER00_SEMANTIC_H
