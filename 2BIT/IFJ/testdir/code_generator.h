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

#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "str.h"
#include "symtable.h"

#define INSTRUCTION 50

typedef struct CodeItem
{
	char *Instruction;
	struct CodeItem *nextItem;
	struct CodeItem *prevItem;
} tCodeElem;

typedef struct
{
	struct CodeItem *First;
	struct CodeItem *Last;
	struct CodeItem *Active;
	struct CodeItem *IfRepair;
	struct CodeItem *WhileRepair;
} tCodeList;

tCodeList List;

/**
 *
 * @param L
 */
void CodelistInit(tCodeList *L);

/**
 *
 * @param L
 */
void CodelistFree(tCodeList *L);

/**
 *
 * @param L
 * @param instr
 */
void CodeListInsertLast(tCodeList *L, char *instr);

/**
 *
 * @param L
 */
void CodelistFirst(tCodeList *L);

/**
 *
 * @param L
 */
void CodelistNext(tCodeList *L);

/**
 *
 * @param L
 * @param gotoInstr
 */
void listGoto(tCodeList *L, void *gotoInstr );

/**
 *
 * @param I
 * @param instr
 */
void CodeListInsertBTW(tCodeElem *I , char *instr);


/**
 *
 */
void codeGenStart();

/**
 *
 */
void define_buildInFunc();

/**
 *
 */
void codeGen_labelHeader();

/**
 *
 */
void codeGen_header();

/**
 *
 */
void codeGenClear();

/**
 *
 */
void codeGen_Main_Start();		

/**
 *
 */
void codeGen_Main_End();			

/**
 *
 * @param function_id
 */
void codeGen_func_Start(char *function_id);

/**
 *
 * @param function_id
 */
void codeGen_func_End(char *function_id);

/**
 *
 * @param type
 */
void codeGen_func_RetVal(datatype_t type);

/**
 *
 * @param id_name
 */
void codeGen_retvalUse(char *id_name);

/**
 *
 * @param function_id
 */
void codeGen_func_Call(char *function_id);

/**
 *
 * @param param_id
 * @param index
 */
void codeGen_func_ParamDeclaration(char *param_id, int index);

/**
 *
 */
void codeGen_func_PrepPass();

/**
 *
 * @param token
 * @param term_str
 * @param is_id
 */
void codeGen_func_TermmVal(symtab_elem_t *token, char* term_str, int is_id);

/**
 *
 * @param token
 * @param index
 * @param is_id
 */
void codeGen_func_PassParam(symtab_elem_t *token, int index,int is_id);

/**
 *
 * @param type
 * @param value
 * @param index
 */
void codeGen_func_PassParamVal(datatype_t type,char *value , int index);

/**
 *
 * @param last
 * @param name_or_buffer
 */
void codeGen_return(int last, char *name_or_buffer);

/**
 *
 * @param id_name
 */
void codeGen_retvalUse(char *id_name);

/**
 *
 * @param var_id
 */
void codeGen_var_declare(char *var_id);

/**
 *
 * @param var_id
 * @param type
 */
void codeGen_var_defAssgn(char *var_id, datatype_t type);

/**
 *
 * @param var_id
 * @param type
 */
void codeGen_input(char *var_id, datatype_t type);

/**
 *
 * @param id_name
 */
void codeGen_printID(char *id_name);

/**
 *
 * @param content
 * @param type
 */
void codeGen_printLIT(char *content, datatype_t type);

/**
 *
 * @param token
 * @param is_id
 */
void codeGen_push(symtab_elem_t *token,int is_id);

/**
 *
 * @param token
 * @param attr
 */
void codeGen_pushValue(int token, char *attr);

/**
 *
 * @param rule
 * @param op_1
 * @param op_2
 */
void codeGen_stack_operation(int rule , int op_1, int op_2);

/**
 *
 */
void codeGen_concat_StackStrings();

/**
 *
 */
void codeGen_stackToDouble_1();

/**
 *
 */
void codeGen_stackToDouble_2();

/**
 *
 */
void codeGen_PopsBool();

/**
 *
 */
void codeGen_PopsExpr();

/**
 *
 * @param id_name
 */
void codeGen_MoveExprResult(char *id_name);

/**
 *
 * @param var_id
 */
void codeGen_beforeIfRep(char *var_id);

/**
 *
 * @param function_id
 * @param label_idx
 */
void codeGen_if_start(char *function_id, int label_idx);

/**
 *
 * @param function_id
 * @param label_idx
 */
void codeGen_else_start(char *function_id, int label_idx);

/**
 *
 * @param functuon_id
 * @param label_idx
 */
void codeGen_if_end(char *functuon_id, int label_idx);

/**
 *
 * @param function_id
 * @param label_idx
 */
void codeGen_while_start_label(char *function_id, int label_idx);

/**
 *
 * @param function_id
 * @param label_idx
 */
void codeGen_while_start(char *function_id, int label_idx);

/**
 *
 * @param var_id
 */
void codeGen_beforeWhileRep(char *var_id);

/**
 *
 * @param function_id
 * @param label_idx
 */
void codeGen_while_end(char *function_id, int label_idx);

#endif
