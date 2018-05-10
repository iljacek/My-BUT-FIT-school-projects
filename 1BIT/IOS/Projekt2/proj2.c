//=================================================
//            IOS project 2 - Projekt2
//  Author: Lubomír Švehla <xsvehl09@stud.fit.vutbr.cz>
//
//  proj2.c created: 2018-Apr-26
//=================================================

#include <stdio.h>
#include <stdlib.h>

#include "proj2.h"



int main(int argc, char *argv[])
{
  //Get and validate arguments
  params = parseParameters(argc, argv);
  initProgram();

  pid_t busProcess = fork();

  if (busProcess == 0)
  {
    doBus();
    closeSemaphores();
    exit(0);
  }

  else if (busProcess < 0)
  {
    perror("ERROR: Fork error");
    endProgram();
    exit(EXIT_FAILURE);
  }

  else
  {
    //Create process for Child
    pid_t riderProcess = -1;
    pid_t riderGeneratorProcess = fork();

    if (riderGeneratorProcess == 0)
    {
      for (int j = 1; j <= params->R; j++)
      {
        if (params->ART != 0)
        {
          randomWait(params->ART);
        }

        riderProcess = fork();
        if (riderProcess == 0)
        {
          doRider(j);
          closeSemaphores();
          exit(0);
        }
        else if (riderProcess < 0)
        {
          perror("ERROR: Fork error");
          endProgram();
          exit(EXIT_FAILURE);
        }
      }

      if (riderProcess > 0)
      {
        while (wait(NULL) > 0);
      }

      closeSemaphores();
      exit(0);
    }
    else if (riderGeneratorProcess < 0)
    {
      perror("ERROR: Fork error");
      endProgram();
      exit(EXIT_FAILURE);
    }
  }

  if (busProcess > 0)
  {
    while (wait(NULL) > 0);
  }

  endProgram();
  exit(0);
}


params_t *parseParameters(int argc, char *argv[])
{
  //Number of arguments must be 7
  if (argc != 5)
  {
    perror("ERROR: Invalid number of arguments given");
    exit(1);
  }

  char *endPtr;

  //Number of adults must be integer bigger then zero
  int R;
  R = (int) strtol(argv[1], &endPtr, 10);
  if (R <= 0 || *endPtr != '\0')
  {
    perror("ERROR: The first argument must be an integer bigger then zero");
    exit(1);
  }

  //Number of child must be integer bigger then zero
  int C;
  C = (int) strtol(argv[2], &endPtr, 10);
  if (C <= 0 || *endPtr != '\0')
  {
    perror("ERROR: The second argument must be an integer bigger then zero");
    exit(1);
  }

  // Maximum time in ms, that new process adult is generated for must be integer between 0 and 5000!
  int ART;
  ART = (int) strtol(argv[3], &endPtr, 10);
  if (ART < 0 || ART > 5000 || *endPtr != '\0')
  {
    perror("ERROR: The third argument must be integer between 0 and 1000");
    exit(1);
  }

  // Maximum time in ms, that new process child is generated for must be integer between 0 and 5000!
  int ABT;
  ABT = (int) strtol(argv[4], &endPtr, 10);
  if (ABT < 0 || ABT > 5000 || *endPtr != '\0')
  {
    perror("ERROR: The last argument must be an integer between 0 and 1000");
    exit(1);
  }

  //All arguments validated, create struct
  params_t *parsedParameters = NULL;
  parsedParameters = malloc(sizeof(params_t));

  //Check for malloc fail
  if (parsedParameters == NULL)
  {
    perror("ERROR: Failed malloc for Parameters struct");
    exit(1);
  }

  parsedParameters->R = R;
  parsedParameters->C = C;
  parsedParameters->ART = ART;
  parsedParameters->ABT = ABT;

  return parsedParameters;
}