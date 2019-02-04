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

#ifndef XDACER00_SYMTAB_H
#define XDACER00_SYMTAB_H

#include <stdbool.h>
#include <stdlib.h>
#include <glob.h>

#define TABLE_SIZE 101

/**
 * @brief new type specifying type of data used
 */
typedef enum {
    DATA_TYPE_INT = 11,
    DATA_TYPE_BOOL = 12,
    DATA_TYPE_DOUBLE = 13,
    DATA_TYPE_STRING = 14,
    DATA_TYPE_NIL = 15,
    DATA_TYPE_UNKNOWN = 16,
} datatype_t;

/**
 * @brief new type specifying the type of element used in symbol table
 */
typedef enum {
    ELEM_TYPE_PARAM = 17,
    ELEM_TYPE_VAR = 18,
    ELEM_TYPE_FUNCTION = 19,
} elemtype_t;

typedef struct symtab_struct symtab_t;
typedef struct symtab_elem_struct symtab_elem_t;

/**
 * @brief structure of the table element
 */
struct symtab_elem_struct {
    char *key;

    datatype_t d_type;
    int param_num;
    elemtype_t e_type;
    bool defined;

    symtab_t *childTable;
    symtab_elem_t *next_item;
};

/**
 * @brief structure of the table
 */
struct symtab_struct {
    symtab_elem_t *element[TABLE_SIZE];
};

/**
 * @brief function generating hash from key based on effective algorithm from ...
 * @param str           input string, which represents the key
 * @param htab_max_size size of the table we want to create the hash for
 * @return hash
 */
unsigned int hash_function(const char *str);

/**
 * @brief function initializing the symbol table
 * @param table         table to be initialized
 * @param htab_max_size size of the table
 * @return initialized table
 */
symtab_t* st_init(symtab_t** table);

/**
 * @brief function to add new elements to the hash table
 * @param table table, where we will be inserting new element
 * @param key   string, which will be used to generate the hash, also the element we are inserting
 * @return initialized table element
 */
symtab_elem_t *st_insert(symtab_t *table, char *key);

/**
 * @brief function to find the element in table
 * @param table table, where we will be searching for the element
 * @param key   string, which will be used to generate the hash, also the element we are looking for
 * @return data of the searched element
 */
symtab_elem_t *st_search(symtab_t* table, char* key);

/**
 * @brief remove a hashtable with all its elements
 * @param table table, we will be deleting
 */
void st_free(symtab_t* table);

#endif //XDACER00_SYMTAB_H