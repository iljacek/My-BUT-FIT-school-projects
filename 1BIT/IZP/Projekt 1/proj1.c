/**
  * Soubor:  proj1.c
  * Datum:   21.10.2017
  * Autor: Lubomir Svehla (xsvehl09)
  * Projekt: IZP Projekt 1 - Prace s textem
  * Popis viz: https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt1&csid=647751&id=12214
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAXNOADDRESSES (42)
#define ADDRESSLENGTH (101)

/**
 * convert string to capital letters
 * @param character
 */
void strToCapitals(char *character)
{
  while (*character != '\0')
  {
    if ((*character >= 'a') && (*character <= 'z'))
      *character = *character - 'a' + 'A';
    character++;
  }
}

/**
 * chceck validity of data on input
 * @param counter - contains length of the string from input
 * @param character - points at characters one by one
 * @return
 */
int checkValidity(int counter, int character)
{
  //checks whether the character is letter or space
  if (isalpha(character) == 0 && isspace(character) == 0)
  {
    printf("Wrong character on input\nAllowed characters are:'a'-'z','A'-'Z',' '\n");
    return 1;
  }
  //checks if the string is not too long
  if (counter >= ADDRESSLENGTH)
  {
    printf("Address is too long (limit is 100 characters per line)\n");
    return 1;
  }
  return 0;
}

/**
 * load data from standard input
 * @param addressList - 2 dimensional array which contains database of addresses
 * @param AddressCount - actual number of addresses that were loaded
 * @return - returns 0 if data are valid, returns 1 if they are invalid
 */
int loadAddresses(char addressList[MAXNOADDRESSES][ADDRESSLENGTH], int *AddressCount)
{
  int character, i = 0, j = 0;

  //reads characters from standard input until it reaches end of file
  while (((character = getchar()) != EOF) && (i < MAXNOADDRESSES))
  {
    // if new line is detected, program switches to next string
    if (character == '\n')
    {
      strToCapitals(addressList[i]);
      i++;
      j = 0;
    }
    // otherwise character is added to current string
    else
    {
      if (checkValidity(j + 1, character) == 1)
      {
        printf("the error is in line %d of the file with addresses\n", i + 1);
        return 1;
      }
      addressList[i][j] = character;
      j++;
    }
  }
  strToCapitals(addressList[i]);
  *AddressCount = i;
  return 0;
}

/**
 * load data from user input
 * @param argc - number of arguments used to start the program
 * @param argv - argument which contains the user input
 * @param userStr - if input data are valid, user input is turned to capitals and copied here
 * @return - returns 0 if data are valid, returns 1 if they are invalid
 */
int loadUserInput(int argc, char *argv, char *userStr)
{
  if (argc >= 2)
  {
    for (int i = 0; i < (int) strlen(argv); i++)
    {
      if (checkValidity((int) strlen(argv), argv[i]) != 0)
      {
        printf("the error is in user input\n");
        return 1;
      }
    }
    strcpy(userStr, argv);
  }
  strToCapitals(userStr);
  return 0;
}

/**
 * compare user input with address list
 * @param oneLeftFlag - flag which signalizes, there is only one character that was enabled
 * @param enableFlag - flag which signalizes, there are characters that were enabled
 * @param foundFlag - flag which signalizes, there is address that was found
 * @param alphabet - string of booleans which shows which letters have been enabled
 * @param addressList - 2 dimensional array which contains database of addresses
 * @param userStr - array which contains user input
 * @param commonAddresses - contains addresses which contain whole string from user input
 * @param AddressCount - actual number of addresses that were loaded to the address database
 * @return - returns number of addresses that contained user input
 */
int compareAddressStr(bool *oneLeftFlag, bool *enableFlag, bool *foundFlag, bool *alphabet,
                      char addressList[MAXNOADDRESSES][ADDRESSLENGTH], char *userStr, int *commonAddresses,
                      int AddressCount)
{
  char temp[strlen(userStr)];
  int counter = 0, j = 0;
  char watchedChar = 0;

  for (int i = 0; i < AddressCount; i++)
  {
    //save first j letters from each address line, where j is length of input argument
    for (j = 0; j < (int) strlen(userStr); j++)
    {
      temp[j] = addressList[i][j];
    }
    //if input argument is prefix of address line
    if ((strncmp(userStr, temp, j) == 0) && (strcmp(userStr,addressList[i]) != 0))
    {
      if (*enableFlag == 0)
        watchedChar = addressList[i][j];

      //if next character is space
      if (addressList[i][j] == ' ')
        alphabet['Z' - 'A' + 1] = 1;
      //if next character is letter
      else
        alphabet[addressList[i][j] - 'A'] = 1;

      // checks if enableFlag was raised only for one character
      if ((watchedChar == addressList[i][j]) && (*oneLeftFlag == 1))
      {
        commonAddresses[counter] = i;
        counter++;
      }
      else
        *oneLeftFlag = 0;

      *enableFlag = 1;
    }
    // if user string equals to one of the strings in address database, raise foundFlag
    else if (strcmp(userStr, addressList[i]) == 0)
    {
      printf("Found: %s\n", userStr);
      *foundFlag = 1;
      *oneLeftFlag = 0;
    }
  }
  return counter;
}

/**
 * prints enabled characters
 * @param alphabet - string of booleans which shows which letters have been enabled
 */
void printEnable(bool *alphabet)
{
  printf("Enable: ");
  for (int i = 0; i < (('Z' - 'A' + 1) + 1); i++)
  {
    if (alphabet[i] == 1)
    {
      if (i == 'Z' - 'A' + 1)
        printf(" ");
      else
        printf("%c", i + 'A');
    }
  }
  printf("\n");
}

/**
 * in case that only one character was enabled
 * finds the shortest of the triggered strings and compares it with the others
 * @param addressList - 2 dimensional array which contains database of addresses
 * @param commonAddresses - contains addresses which contain whole string from user input
 * @param counter - number of strings on the input of this function
 * @param matches - count of the strings to which the string min is prefix
 * @param min - shortest of the strings
 * @return - returns the number of matches
 */
int comparePickedStr(char addressList[MAXNOADDRESSES][ADDRESSLENGTH], int *commonAddresses, int counter, int matches,
                     int *min)
{
  // find shortest of selected strings
  for (int i = 1; i < counter; i++)
  {
    if (strlen(addressList[*min]) > strlen(addressList[commonAddresses[i]]))
      *min = commonAddresses[i];
  }
  // check if all the longer strings contain the shortest one
  for (int i = 0; i < counter; i++)
  {
    if (commonAddresses[i] != *min)
      if (strncmp(addressList[*min], addressList[commonAddresses[i]], strlen(addressList[*min])) == 0)
        matches++;
  }
  return matches;
}

/**
 * if there is only one string: it prints found
 * if the shortest string is prefix to all the others: prints found and enable for the following characters
 * if it is not prefix for all of them: prints the character that was enabled on input of this function
 * @param enableFlag - flag which signalizes, there are characters that were enabled
 * @param alphabet - string of booleans which shows which letters have been enabled
 * @param addressList - 2 dimensional array which contains database of addresses
 * @param commonAddresses - contains addresses which contain whole string from user input
 * @param counter - number of strings on the input of this function
 * @param min - shortest of the strings
 */
void findResult(bool *enableFlag, bool *alphabet, char addressList[MAXNOADDRESSES][ADDRESSLENGTH], int *commonAddresses,
                int counter, int min)
{
  printf("Found: %s\n", addressList[min]);
  //cancel flags from past
  memset(&alphabet[0], 0, ('Z' - 'A' + 1) + 1);
  //when there is just one string set enable flag to 0, because, enable would be empty anyway
  if (counter == 1)
    *enableFlag = 0;

  // set new flags following the found formula
  for (int i = 0; i < counter; i++)
  {
    //if next character is space
    if (addressList[commonAddresses[i]][strlen(addressList[min])] == ' ')
      alphabet['Z' - 'A' + 1] = 1;
    //if next character is letter
    else
      alphabet[addressList[commonAddresses[i]][strlen(addressList[min])] - 'A'] = 1;
  }
  (void) enableFlag;
}

/**
 * main
 * @param argc - number of arguments used to run the program
 * @param argv - array of strings containing the arguments
 * @return - returns 0 if the program ended successfully, returns 1 if error was detected
 */

int main(int argc, char *argv[])
{
  bool alphabet[('Z' - 'A' + 1) + 1] = {0};
  bool foundFlag = 0, enableFlag = 0, oneLeftFlag = 1;
  char addressList[MAXNOADDRESSES][ADDRESSLENGTH] = {0};
  char userStr[ADDRESSLENGTH];
  int errorCheck = 0, counter = 0, addressCount;
  int commonAddresses[MAXNOADDRESSES];

  // Read from file and from user input and test validity of data
  errorCheck = loadAddresses(addressList, &addressCount);
  errorCheck = errorCheck + loadUserInput(argc, argv[1], userStr);
  if (errorCheck != 0)
    return 1;

  // compare string with addresses
  counter = compareAddressStr(&oneLeftFlag, &enableFlag, &foundFlag, alphabet, addressList, userStr, commonAddresses,
                              addressCount);

  // one option left part
  if (oneLeftFlag == 1)
  {
    int min = commonAddresses[0];
    int matches = 0;

    matches = comparePickedStr(addressList, commonAddresses, counter, matches, &min);
    // if all strings contain the shortest one
    if (matches == counter - 1)
    {
      foundFlag=1;
      findResult(&enableFlag, alphabet, addressList, commonAddresses, counter, min);
    }
  }

  // if triggered print enable and not found
  if (enableFlag == 1)
    printEnable(alphabet);

  if ((foundFlag == 0) && (enableFlag == 0))
    printf("Not found\n");

  return 0;
}

