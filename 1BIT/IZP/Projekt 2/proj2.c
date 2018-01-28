/**
  * Soubor:  proj2.c
  * Datum:   7.11.2017
  * Autor: Lubomir Svehla (xsvehl09)
  * Projekt: IZP Projekt 2 - Iteracni vypocty
  * Popis viz: https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?title=IZP:Projekt2&csid=647751&id=12214
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HELP 1    //help mode
#define TAN 2     //tangent mode
#define MA 3      //measurement mode with only angle A specified by user
#define MAB 4     //measurement mode with only angles A and B specified by user
#define CXMA 5    //measurement mode with only angle A and device height X specified by user
#define CXMAB 6   //measurement mode with all arguments specified by user

#define RESET   "\033[0m"     // Reset standard output text to default format
#define GREEN   "\033[32m"    // Green colour for text at standard output

//minimal number of iterations needed for 10 place precision for number 1.4 (the least possible precise value)
#define ITERATIONSCOUNT 9


/**
 * selects mode depending on user input
 * @param argc - number of arguments used to start the program
 * @param argv - two dimensional array containing arguments with the user input
 * @param mode - mode selected by user (--help, --tan or -m (with variable combinations of arguments))
 */
void selectMode(int argc, char **argv, int *mode)
{
  if ((argc == 2) && (strcmp(argv[1], "--help") == 0))
  {
    *mode = HELP;
  }
  else if ((argc == 5) && (strcmp(argv[1], "--tan") == 0))
  {
    *mode = TAN;
  }
  else if (((argc == 3) || (argc == 4)) && (strcmp(argv[1], "-m") == 0))
  {
    if (argc == 3)
      *mode = MA;
    else
      *mode = MAB;
  }
  else if (((argc == 5) || (argc == 6)) && (strcmp(argv[1], "-c") == 0) && (strcmp(argv[3], "-m") == 0))
  {
    if (argc == 5)
      *mode = CXMA;
    else
      *mode = CXMAB;
  }
  else
  {
    fprintf(stderr, "Invalid user input - none of the program modes was selected\n"
            "For more information about how to use the program, start program with argument --help\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * chcecks validity of data on input and stores it in memory if validity was proven
 * @param argv - string of characters that is tested
 * @return - returns input argument as a double format number
 */
double checkValidity(char *argv)
{
  char *pEnd;
  double result;

  result = strtod(argv, &pEnd);
  if (*pEnd != '\0')
  {
    fprintf(stderr, "Arguments other than mode select should contain only numbers and '.', "
            "first character can be also '-'\n");
    exit(EXIT_FAILURE);
  }
  return result;
}

/**
 * Checks whether the user input is in allowed interval
 * @param parameters - arguments from user converted to double format numbers
 * @param counter - the number of parameter that is being controlled
 * @param maxValues - array containing the maximal allowed values
 */
void checkInterval(double *parameters, int counter, double *maxValues)
{
  if ((parameters[counter] > maxValues[counter]) || (parameters[counter] <= 0))
  {
    fprintf(stderr, "%dth parameter value is out of allowed borders\n", counter + 1);
    exit(EXIT_FAILURE);
  }
}

/**
 * sets maximal allowed values depending on the selected mode
 * @param mode - selected mode (--tan or -m)
 * @param maxValues - array containing the maximal allowed values
 */
void setMaxValues(int mode, double *maxValues)
{
  if (mode == TAN)
  {
    maxValues[0] = 1.4;
    maxValues[1] = 13.;
    maxValues[2] = 13.;
  }
  else
  {
    maxValues[0] = 100.;
    maxValues[1] = 1.4;
    maxValues[2] = 1.4;
  }
}

/**
 *
 * while the parameters are being loaded, they are also checked, whether they are in allowed range of values
 * @param argc - number of arguments used to start the program
 * @param argv - two dimensional array containing arguments with the user input
 * @param parameters - arguments from user converted to double format numbers
 * @param mode - selected mode (--tan or -m)
 */
void loadParameters(int argc, char **argv, double *parameters, int mode)
{
  double maxValues[3];
  int counter = 0;
  setMaxValues(mode, maxValues);

  if ((mode == TAN) || (mode == MA) || (mode == MAB))
  {
    if ((mode == MA) || (mode == MAB))
    {
      parameters[counter] = 1.5;
      counter++;
    }

    for (int i = 2; i < argc; i++)
    {
      parameters[counter] = checkValidity(argv[i]);   //loads arguments from user input if they are valid numbers
      //don't check the first parameter of tangent function, because there are no limitations to its value
      if ((mode != TAN) || (counter != 0))
        checkInterval(parameters, counter, maxValues);
      counter++;
    }

    if ((mode == TAN) && (parameters[1]>parameters[2]))
    {
      fprintf(stderr, "Wrong input: N has to be equal or smaller than M\n");
      exit(EXIT_FAILURE);
    }
  }

  if ((mode == CXMA) || (mode == CXMAB))
  {
    for (int i = 2; i < argc; i++)
    {
      if (i == 3)
        i++;
      parameters[counter] = checkValidity(argv[i]);
      checkInterval(parameters, counter, maxValues);
      counter++;
    }
  }
}


/**
 * Computes tangent value of angle x by using Taylor polynomial
 * @param x - angle at input of tangent function
 * @param n - number of iterations
 * @return - returns computed value of tangent
 */
double taylor_tan(double x, unsigned int n)
{
  //
  const long long numerators[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862,
                              58870668456604};
  const long long denominator[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625,
                               194896477400625, 49308808782358125, 3698160658676859375};
  double t = x;
  double s = t;
  unsigned int i = 1;

  while (i < n)
  {
    t = t * x * x;
    s += t * numerators[i] / denominator[i];
    i++;
  }
  return s;
}

/**
 * Computes tangent value of angle x by using chained fraction
 * @param x - angle at input of tangent function
 * @param n - number of iterations
 * @return - returns computed value of tangent
 */
double cfrac_tan(double x, unsigned int n)
{
  double a;
  double b = x * x;
  double cf = 0.;
  for (unsigned int i = n; i > 1; i--)
  {
    a = 2 * i - 1;
    cf = b / (a - cf);
  }
  return x / (1 - cf);
}

/**
 * prints the help menu
 */
void printHelp()
{
  printf("Program created by Lubomir Svehla (xsvehl09) on 7.11.2017\n"
                 "You can run the program in following forms:\n"
                 GREEN "./proj2 --help\n" RESET ""
                 "--help will print the program guide and terminates the program.\n\n"
                 GREEN "./proj2 --tan A N M\n" RESET ""
                 "--tan compares the accuracy of the tangent calculation of angle A in radians. "
                 "Tangents are calculated in two ways: by using Taylor polynomial and by chained fraction. "
                 "Both of thse are compared to function tan() from <math.h> library. "
                 "Arguments N and M indicate in which iterations the comparision is to be performed. "
                 "N is smaller or equal to M.\n\n"
                 GREEN "./proj2 [-c X] -m A [B]\n" RESET ""
                 "-m calculates and measures the distance. "
                 "A is the angle between the bottom of observed object and the level of measuring device. "
                 "B is the angle between the top of observed object and the level of measuring device. "
                 "Both of these angles are in radians and are from interval (0 , 1.4>. "
                 "Argument -c X sets the height of measuring device. The height X is from interval (0, 100>.\n"
                 "Arguments in brackets [] are optional.\n");
}

/**
 * gets computed values of tangent by Taylor and chained fraction and prints their comparison with tan() from <math.h>
 * @param parameters - arguments from user converted to double format numbers
 */
void printTan(double *parameters)
{
  double resultTaylor, resultCfrac;

  for (int i = (int) parameters[1]; i <= (int) parameters[2]; i++)
  {
    resultTaylor = taylor_tan(parameters[0], i);
    resultCfrac = cfrac_tan(parameters[0], i);
    printf("%d %e %e %e %e %e\n", i, tan(parameters[0]), resultTaylor, fabs(tan(parameters[0]) - resultTaylor),
           resultCfrac, fabs(tan(parameters[0]) - resultCfrac));
  }
}


/**
 * main
 * @param argc - number of arguments used to run the program
 * @param argv - array of strings containing the arguments
 * @return - returns 0 if the program ended successfully, returns 1 if error was detected
 */

int main(int argc, char *argv[])
{
  int mode = 0;
  double parameters[3] = {-1., -1., -1.};
  double distance, height;

  selectMode(argc, argv, &mode);
  loadParameters(argc, argv, parameters, mode);

  switch (mode)
  {
    case HELP:
      printHelp();
      break;
    case TAN:
      printTan(parameters);
      break;
    case MA:
    case CXMA:
      distance = parameters[0] / cfrac_tan(parameters[1], ITERATIONSCOUNT);
      printf("%.10e\n", distance);
      break;
    default:
      distance = parameters[0] / cfrac_tan(parameters[1], ITERATIONSCOUNT);
      height = cfrac_tan(parameters[2], 8) * distance + parameters[0];
      printf("%.10e\n%.10e\n", distance, height);
  }
  return EXIT_SUCCESS;
}
