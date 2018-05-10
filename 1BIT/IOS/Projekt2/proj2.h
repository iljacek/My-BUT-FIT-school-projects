//=================================================
//            IOS project 2 - Projekt2
//  Author: Lubomír Švehla <xsvehl09@stud.fit.vutbr.cz>
//
//  proj2.h created: 2018-Apr-26
//=================================================

#ifndef PROJEKT2_PROJ2_H
#define PROJEKT2_PROJ2_H

#include "TheSenateBusProblem.h"

/**
 * @brief Main function of project
 * @param argc Number of arguments
 * @param argv Array with content of arguments
 * @return exit code
 */
int main(int argc, char *argv[]);

/**
 * @brief Parser of arguments
 * @param argc Number of arguments
 * @param argv Array with content of arguments
 * @return parsed parameters in a struct
 */
params_t *parseParameters(int argc, char *argv[]);


#endif //PROJEKT2_PROJ2_H
