//=================================================
//            IOS project 2 - Projekt2
//  Author: Lubomír Švehla <xsvehl09@stud.fit.vutbr.cz>
//
//  TheSenateBusProblem.c created: 2018-Apr-26
//=================================================

#include "TheSenateBusProblem.h"

FILE *logFile;


void initProgram() {

  //Create semaphores
  semaphores.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
  semaphores.bus = sem_open(SEM_BUS, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
  semaphores.boarded = sem_open(SEM_BOARDED, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
  semaphores.inDestination = sem_open(SEM_INDESTINATION, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

  //Check for failure
  if (semaphores.mutex == SEM_FAILED || semaphores.bus == SEM_FAILED || semaphores.boarded == SEM_FAILED || semaphores.inDestination == SEM_FAILED)
  {
    perror("ERROR: Semaphores initialization error");
    endProgram();
    exit(EXIT_FAILURE);
  }

  //Create shared memory variables
  sharedMemory.waitingRidersCount_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
  sharedMemory.actionOrder_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
  sharedMemory.ridersCount_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666);

  //Check for failure
  if (sharedMemory.waitingRidersCount_id == -1 || sharedMemory.actionOrder_id == -1 || sharedMemory.ridersCount_id == -1)
  {
    perror("ERROR: Shared memory initialization error");
    endProgram();
    exit(EXIT_FAILURE);
  }

  //Map
  sharedMemory.waitingRidersCount = (int *) shmat(sharedMemory.waitingRidersCount_id, NULL, 0);
  sharedMemory.actionOrder = (int *) shmat(sharedMemory.actionOrder_id, NULL, 0);
  sharedMemory.ridersCount = (int *) shmat(sharedMemory.ridersCount_id, NULL, 0);


  //Check for failure
  if (sharedMemory.waitingRidersCount == NULL || sharedMemory.actionOrder == NULL || sharedMemory.ridersCount == NULL)
  {
    perror("ERROR: Shared memory mapping error");
    endProgram();
    exit(EXIT_FAILURE);
  }

  //Init logfile
  logFile = fopen(LOG_FILE_NAME, "w");
  if (logFile == NULL) {
    perror("ERROR: Can't open/create logfile");
    exit(1);
  }

  setbuf(logFile, NULL);
}


void endProgram() {
  //Close semaphores
  closeSemaphores();

  //Unlink semaphores
  sem_unlink(SEM_MUTEX);
  sem_unlink(SEM_BUS);
  sem_unlink(SEM_BOARDED);
  sem_unlink(SEM_INDESTINATION);

  //Close shared memory
  close(sharedMemory.waitingRidersCount_id);
  close(sharedMemory.actionOrder_id);
  close(sharedMemory.ridersCount_id);

  //Unmap
  munmap(sharedMemory.waitingRidersCount, sizeof(int));
  munmap(sharedMemory.actionOrder, sizeof(int));
  munmap(sharedMemory.ridersCount, sizeof(int));

  //Unlink shared memory
  shmctl(sharedMemory.waitingRidersCount_id, IPC_RMID, NULL);
  shmctl(sharedMemory.actionOrder_id, IPC_RMID, NULL);
  shmctl(sharedMemory.ridersCount_id, IPC_RMID, NULL);

  //Free memory for params
  free(params);

  //Close logfile
  fclose(logFile);
}


void closeSemaphores() {
  sem_close(semaphores.mutex);
  sem_close(semaphores.bus);
  sem_close(semaphores.boarded);
  sem_close(semaphores.inDestination);
}

int getMin(int a, int b) {
  if (a > b) {
    return b;
  }

  return a;
}

int getMax(int a, int b) {
  if (a > b) {
    return a;
  }

  return b;
}

void randomWait(int max)
{
  if (max > 0) {
    usleep((useconds_t) (random() % max * 1000));
  }
}

void doBus() {

  //Starting
  sem_wait(semaphores.mutex);
  (*sharedMemory.actionOrder)++;
  fprintf(logFile, "%d: BUS: start\n", *sharedMemory.actionOrder);
  sem_post(semaphores.mutex);

  static int ridersToBoard = 0;

  while ((*sharedMemory.ridersCount<params->R) || (*sharedMemory.waitingRidersCount != 0))
  {
    //Arival
    sem_wait(semaphores.mutex);
    (*sharedMemory.actionOrder)++;
    fprintf(logFile, "%d: BUS: arrival\n", *sharedMemory.actionOrder);

    //A loop to get all the boarded riders off board
    for (int i = 0; i < ridersToBoard; i++)
    {
      sem_post(semaphores.inDestination);         //bus signals that it has arrived and passenger can get off board
      sem_wait(semaphores.boarded);               //Allows one rider to get off board
    }

    //check for number of waiting riders with concern to capacity of bus
    ridersToBoard = getMin(*sharedMemory.waitingRidersCount, params->C);
    (*sharedMemory.actionOrder)++;
    fprintf(logFile, "%d: BUS: start boarding: %d\n", *sharedMemory.actionOrder, *sharedMemory.waitingRidersCount);

    //A loop to get all the available riders  on board
    for (int i = 0; i < ridersToBoard; i++)
    {
      sem_post(semaphores.bus);                   //bus signals that it has arrived and can take a passenger on board
      sem_wait(semaphores.boarded);               //Allows one rider to get on board
    }

    //Finish boarding and depart
     *sharedMemory.waitingRidersCount = getMax(*sharedMemory.waitingRidersCount - params->C, 0);
    (*sharedMemory.actionOrder)++;
    fprintf(logFile, "%d: BUS: end boarding: %d\n", *sharedMemory.actionOrder, *sharedMemory.waitingRidersCount);
    (*sharedMemory.actionOrder)++;
    fprintf(logFile, "%d: BUS: depart\n", *sharedMemory.actionOrder);
    sem_post(semaphores.mutex);

    //Simulate journey
    if (params->ABT != 0)
    {
      randomWait(params->ABT);
    }

    //end ride
    sem_wait(semaphores.mutex);
    (*sharedMemory.actionOrder)++;
    fprintf(logFile, "%d: BUS: end\n", *sharedMemory.actionOrder);
    sem_post(semaphores.mutex);

  }

  //Finish
  sem_wait(semaphores.mutex);
  (*sharedMemory.actionOrder)++;
  fprintf(logFile, "%d: BUS: finish\n", *sharedMemory.actionOrder);

  //let the rest of rider processes finish
  for (int i = 0; i < ridersToBoard; i++)
  {
    sem_post(semaphores.inDestination);
    sem_wait(semaphores.boarded);
  }
  sem_post(semaphores.mutex);
}



void doRider(int id)
{
  //Starting
  sem_wait(semaphores.mutex);
  (*sharedMemory.actionOrder)++;
  fprintf(logFile, "%d: RID %d: start\n", *sharedMemory.actionOrder, id);
  sem_post(semaphores.mutex);

  //Enter bus stop
  sem_wait(semaphores.mutex);
  (*sharedMemory.ridersCount)++;
  (*sharedMemory.waitingRidersCount)++;
  (*sharedMemory.actionOrder)++;
  fprintf(logFile, "%d: RID %d: enter: %d\n", *sharedMemory.actionOrder, id, *sharedMemory.waitingRidersCount);
  sem_post(semaphores.mutex);

  //Enter bus
  sem_wait(semaphores.bus);                  //acquire the bus semaphore to get on board
  (*sharedMemory.actionOrder)++;
  fprintf(logFile, "%d: RID %d: boarding\n", *sharedMemory.actionOrder, id);
  sem_post(semaphores.boarded);              //once boarded, release the boarded semaphore

  //Leave bus
  sem_wait(semaphores.inDestination);        //acquire the inDestination semaphore to get off board
  (*sharedMemory.actionOrder)++;
  fprintf(logFile, "%d: RID %d: finish\n", *sharedMemory.actionOrder, id);
  sem_post(semaphores.boarded);              //once off board, release the boarded semaphore
}
