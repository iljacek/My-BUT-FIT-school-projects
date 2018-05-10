//=================================================
//            IOS project 2 - Projekt2
//  Author: Lubomír Švehla <xsvehl09@stud.fit.vutbr.cz>
//
//  TheSenateBusProblem.h created: 2018-Apr-26
//=================================================

#ifndef PROJEKT2_THESENATEBUSPROBLEM_H
#define PROJEKT2_THESENATEBUSPROBLEM_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>

#define LOG_FILE_NAME "proj2.out" /** Name of log file */

//Constants - Semaphore names
#define SEM_MUTEX "/xvehl09-projekt2-semMutex"
#define SEM_BUS "/xvehl09-projekt2-semBus"
#define SEM_BOARDED "/xvehl09-projekt2-semBoarded"
#define SEM_INDESTINATION "/xvehl09-projekt2-semInDestination"

//Log file
extern FILE *logFile; /** Output log file */

/**
 * @brief Structure for program parameters
 */
typedef struct params{
    int R; /**< Number of rider processes */
    int C; /**< Bus capacity */
    int ART; /**< Maximum time in ms, for new rider process to generate */
    int ABT; /**< Maximum time in ms, for bus process to simulate ride */
} params_t;

//global struct pointer to parameters
params_t *params;

/**
 * @brief Structure for semaphores
 */
struct semaphores
{
    sem_t *mutex;              //mutual exclusion semaphore
    sem_t *bus;                //semaphore signaling that bus is on stop and allows riders to board
    sem_t *boarded;            //semaphore signaling that rider has got in or out of the bus and allows bus to continue
    sem_t *inDestination;      //semaphore signaling that bus is on stop and allows riders to leave bus and finish
};

//global struct of semaphores
struct semaphores semaphores;

/**
 * @brief Structure for shared memory variables
 */
struct sharedMemory
{
    int *waitingRidersCount;    // Number of riders waiting
    int waitingRidersCount_id;  //ID of variable in shared memory

    int *actionOrder;           //Number of actions done
    int actionOrder_id;

    int *ridersCount;           //Number of riders processes that already exists
    int ridersCount_id;
};

//global struct of shared memory variables
struct sharedMemory sharedMemory;

/**
 * @brief Init program
 */
void initProgram();

/**
 * @brief Destroys and cleans everything
 */
void endProgram();

/**
 * @brief Closes all semaphores
 */
void closeSemaphores();

/**
 * @brief Compares two numbers and finds the smaller one
 * @param a first number
 * @param b second number
 * @return smaller number
 */
int getMin(int a, int b);

/**
 * @brief Compares two numbers and finds the greater one
 * @param a first number
 * @param b second number
 * @return greater number
 */
int getMax(int a, int b);

/**
 * @brief wait for random time from interval
 * @param max maximal time to wait
 */
void randomWait(int max);

/**
 * @brief Bus process
 */
void doBus();

/**
 * @brief Rider process
 * @param id id of current rider process
 */
void doRider(int id);


#endif //PROJEKT2_THESENATEBUSPROBLEM_H
