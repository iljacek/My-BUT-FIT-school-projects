/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */

/**
  * Soubor:  proj3.c
  * Datum:   2.12.2017
  * Autor: Lubomir Svehla (xsvehl09)
  * Projekt: IZP Projekt 3 - Jednoduchá shluková analýza
  * Popis viz: https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt3&csid=647751&id=12214
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <memory.h>


#define MAXCOORDINATEVALUE 1000   /* coordinates have to be from interval (0,1000)*/
#define NOARGUMENTSINLINE 3       /* 3 arguments per line allowed */
#define MIN 0
#define AVG 1
#define MAX 2

/* maximal relevant number of characters in line: (5 or 10 for integer) + 2x(4 for 1000) + 2(spaces) +1(new line)*/
#define MAXNOCHARSINLINE ((INT_MAX) <= 32768 ? 16 : 21)




/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklaracia globalnej premennej pre premiove riesenie
 *
 */
int premium_case=1;

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
  assert(c != NULL);
  assert(cap >= 0);

  // TODO
  if (cap > 0)
  {
    if ((c->obj = malloc(cap * sizeof(struct obj_t))))
    {
      c->capacity = cap;
      c->size = 0;
      return;
    }
  }

  //if cap<=0 or malloc was not successful
  c->obj = NULL;
  c->capacity = 0;

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
  // TODO
  free(c->obj);
  init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
  // TUTO FUNKCI NEMENTE
  assert(c);
  assert(c->capacity >= 0);
  assert(new_cap >= 0);

  if (c->capacity >= new_cap)
    return c;

  size_t size = sizeof(struct obj_t) * new_cap;

  void *arr = realloc(c->obj, size);
  if (arr == NULL)
    return NULL;

  c->obj = (struct obj_t *) arr;
  c->capacity = new_cap;
  return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
  // TODO
  if (c->capacity <= c->size)
    resize_cluster(c, c->capacity + CLUSTER_CHUNK);

  c->obj[c->size++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
  assert(c1 != NULL);
  assert(c2 != NULL);

  // TODO
  for (int i = 0; i < (c2->size); i++)
  {
    append_cluster(c1, c2->obj[i]);
  }
  if (c2->size > 0)
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/

int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
  assert(idx < narr);
  assert(narr > 0);

  // TODO
  clear_cluster(&carr[idx]);

  for (int i = idx; i < narr - 1; i++)
  {
    carr[i] = carr[i + 1];
  }

  return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
  assert(o1 != NULL);
  assert(o2 != NULL);

  // TODO
  float a = o1->x - o2->x;
  float b = o1->y - o2->y;
  return sqrtf((a * a) + (b * b));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
  assert(c1 != NULL);
  assert(c1->size > 0);
  assert(c2 != NULL);
  assert(c2->size > 0);

  // TODO

  float result = -1, currentDistance = 0;
  int i, j;

  for (i = 0; i < c1->size; i++)
  {
    for (j = 0; j < c2->size; j++)
    {
      if (premium_case == MIN)
      {
        currentDistance = obj_distance(&c1->obj[i], &c2->obj[j]);
        if ((currentDistance < result) || (result<0))
          result = currentDistance;
      }
      if (premium_case == AVG)
        currentDistance += obj_distance(&c1->obj[i], &c2->obj[j]);
      if (premium_case == MAX)
      {
        currentDistance = obj_distance(&c1->obj[i], &c2->obj[j]);
        if (currentDistance > result)
          result = currentDistance;
      }
    }
  }
  if (premium_case == AVG)
    result = currentDistance / (i * j);

  return result;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
  assert(narr > 0);

  // TODO
  float minDistance = -1, currentDistance;

  for (int i = 0; i < narr; i++)
  {
    for (int j = i + 1; j < narr; j++)
    {
      currentDistance = cluster_distance(&carr[i], &carr[j]);

      if ((currentDistance < minDistance) || (minDistance < 0))
      {
        minDistance = currentDistance;
        *c1 = i;
        *c2 = j;
      }
    }
  }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
  // TUTO FUNKCI NEMENTE
  const struct obj_t *o1 = (const struct obj_t *) a;
  const struct obj_t *o2 = (const struct obj_t *) b;
  if (o1->id < o2->id)
    return -1;
  if (o1->id > o2->id)
    return 1;
  return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
  // TUTO FUNKCI NEMENTE
  qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
  // TUTO FUNKCI NEMENTE
  for (int i = 0; i < c->size; i++)
  {
    if (i)
      putchar(' ');
    printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
  }
  putchar('\n');
}

/**
 * Initializes array of narr clusters.
 * @param carr array of clusters
 * @param narr number of clusters in array
 */
void initClusters(struct cluster_t **carr, const int narr)
{
  assert(carr != 0);
  assert(narr >= 0);

  if (!(*carr = malloc(narr * sizeof(struct cluster_t))))
  {
    //terminate if unsuccessful
    return;
  }

  for (int i = 0; i < narr; i++)
  {
    init_cluster(&(*carr)[i], 0);
  }
}

/**
 * Removes the array of clusters
 * @param carr array of clusters that will be removed
 * @param narr nuber of clusters in array
 */
void clearClusters(struct cluster_t *carr, const int narr)
{
  assert(carr != 0);
  assert(narr >= 0);

  for (int i = 0; i < narr; i++)
  {
    clear_cluster(&carr[i]);
  }

  free(carr);
}

/**
 * checks whether the number has value from pre defined range
 * @param input - number that is being tested
 * @param index - index of number read from token (first (index 0) is ID and the others (index 1 and 2) are coordinates)
 * @param line - current line of file taht is being processed
 * @return returns the tested number on success, returns -1 if unsuccessful
 */
int checkNumberValidity(int input, int index, int line)
{
  if ((index == 0) && (input < 0))
  {
    fprintf(stderr, "IDs on line %d in text file is lower than 0\n", line);
    return -1;
  }
  if (((index == 1) || (index == 2)) && ((input < 0) || (input > MAXCOORDINATEVALUE)))
  {
    fprintf(stderr, "Coordinate, that is out of the defined interval (0,1000) was found on line %d\n", line);
    return -1;
  }
  return input;

}

/**
 * checks if the first line of text contains only string "count=" and integer number, also if array of clusters was
 * loaded successfully
 * @param file - file containing number of objects and info about objects themselves-
 * @param token - string containing data from one line of file, that is being processed at the moment
 * @param arr - array of clusters
 * @return on success, returns cnumber of objects, if unsuccessful, returns -1
 */
int checkFirstLine(FILE *file, char *token, struct cluster_t **arr)
{
  int objectsCount;
  char endChar;

  if (sscanf(token, "count=%d%[^\n]", &objectsCount, &endChar) != 1)
  {
    fprintf(stderr, "Invalid format of first line\n");
    fclose(file);
    return -1;
  }

  if (objectsCount <= 0)
  {
    fprintf(stderr, "number of repetitions has to be integer higher than 0\n");
    fclose(file);
    return -1;
  }

  initClusters(arr, objectsCount);
  if (*arr == '\0')
  {
    fprintf(stderr, "Allocation error.\n");
    fclose(file);
    return -1;
  }
  return objectsCount;
}

/**
 * stores the numbers read from token to object and stores this object in cluster array
 * @param arr - array of clusters
 * @param readNumbers - contains numbers that were read from token
 * @param currentObject - contains the index of current Object that is being stored
 */
void saveObjectLines(struct cluster_t **arr, int *readNumbers, int currentObject)
{
  struct obj_t obj;
  struct cluster_t *cluster;

  obj.id = readNumbers[0];
  obj.x = readNumbers[1];
  obj.y = readNumbers[2];
  cluster = &(*arr)[currentObject];
  append_cluster(cluster, obj);
}

/**
 * checks if line of text contains only 3 integer numbers, if they are of allowed value and if there isn§t already
 * object with the same ID
 * @param file - file containing number of objects and info about objects themselves
 * @param arr - array of clusters
 * @param token - string containing data from one line of file, that is being processed at the moment
 * @param currentObject - value between 0 and objectsCount, contains the index of current Object that is being checked
 * @param objectsCount - count of objects, that was specified in first line of file
 * @param ids - array containing IDs of all objects that were loaded so far
 * @return returns 0 if no error was found, returns 1 if error was found
 */
bool checkObjectLines(FILE *file, struct cluster_t **arr, char *token, int currentObject, int objectsCount, int *ids)
{
  char endChar;
  bool pointsValidityFlag = 0;
  int readNumbers[NOARGUMENTSINLINE];

  if (sscanf(token, "%d %d %d%[^\n]", &readNumbers[0], &readNumbers[1], &readNumbers[2], &endChar) != NOARGUMENTSINLINE)
  {
    fprintf(stderr, "line %d has invalid format (should contain 3 integers separated by ' '\n", currentObject + 2);
    pointsValidityFlag = 1;
  }

  for (int i = 0; i < NOARGUMENTSINLINE; ++i)
  {
    readNumbers[i] = checkNumberValidity(readNumbers[i], i, currentObject + 2);
    if (readNumbers[i] < 0)
      pointsValidityFlag = 1;
  }

  for (int i = 0; i < currentObject; i++)
  {
    if (readNumbers[0] == ids[i])
    {
      fprintf(stderr, "same id was used on line %d and %d (or there are less lines in file than count says)\n", i + 2,
              currentObject + 2);
      pointsValidityFlag = 1;
    }
  }
  ids[currentObject] = readNumbers[0];

  if (pointsValidityFlag == 1)
  {
    clearClusters(*arr, objectsCount);
    *arr = NULL;
    fclose(file);
  }
  else
    saveObjectLines(arr, readNumbers, currentObject);
  return pointsValidityFlag;
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
  assert(arr != NULL);

  // TODO
  int currentObject = 0, objectsCount;

  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    fprintf(stderr, "Flie loading error\n");
    return -1;
  }

  char token[MAXNOCHARSINLINE];

  fgets(token, MAXNOCHARSINLINE, file);
  objectsCount = checkFirstLine(file, token, arr);
  if (objectsCount == -1)
    return -1;

  int ids[objectsCount];
  while (currentObject < objectsCount)
  {
    fgets(token, MAXNOCHARSINLINE, file);
    if (checkObjectLines(file, arr, token, currentObject, objectsCount, ids) == 1)
      return -1;

    currentObject++;
  }
  fclose(file);

  if (currentObject < objectsCount)
  {
    fprintf(stderr, "Number of objects can't be less than %d\n", objectsCount);
    clearClusters(*arr, objectsCount);
    *arr = NULL;
    fclose(file);
    return -1;
  }

  return objectsCount;

}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
  printf("Clusters:\n");
  for (int i = 0; i < narr; i++)
  {
    printf("cluster %d: ", i);
    print_cluster(&carr[i]);
  }
}

/**
 * finds closest clusters and merges them, until there is only finalSize number of clusters
 * @param carr - array of clusters
 * @param narr - number of clusters in array
 * @param finalSize - final number of clusters specified by user
 * @return if successful, returns new number of clusters in array, if not, returns -1
 */
int setClustersCount(struct cluster_t *carr, int narr, const int finalSize)
{
  if (finalSize > narr)
  {
    fprintf(stderr, "Value of cluster count, which is '%d' can't be bigger than objects count '%d'.\n", finalSize, narr);
    return -1;
  }

  int idx1, idx2;
  while (narr > finalSize)
  {
    find_neighbours(carr, narr, &idx1, &idx2);
    merge_clusters(&carr[idx1], &carr[idx2]);
    narr = remove_cluster(carr, narr, idx2);
  }
  return narr;
}

/**
 * checks if the second parameter of user input contains only number that is higher than 0
 * @param argv - argument, that should contain the desired number of clusters
 * @return if successful, returns user specified number of clusters, if not successful, returns -1
 */
int userInputValidation(char *argv)
{
  char *endptr = NULL;
  int result = (int) strtol(argv, &endptr, 10);

  if (*endptr != '\0')
  {
    fprintf(stderr, "Argument N in user input should contain only integer number\n");
    return -1;
  }
  else if (result <= 0)
  {
    fprintf(stderr, "Argument N has to be higher than 0\n");
    return -1;
  }
  return result;
}

/**
 * if program started with more than 1 argument, checks number and validity of other arguments
 * it also reads finalSize and chooses how the distance will be computed
 * @param argc - number of arguments used on start of program
 * @param argv - array containing arguments of program
 * @param finalSize - final number of clusters specified by user
 * @return returns finalSize if successful, returns -1 if unsuccessful
 */
int checkSpecialArguments(int argc, char **argv, int  finalSize)
{
  finalSize = userInputValidation(argv[2]);
  if (finalSize == -1)
    return -1;
  if (argc == 4)
  {
    if (strcmp(argv[3],"--min") == 0)
      premium_case=0;
    else if (strcmp(argv[3],"--avg") == 0)
      premium_case=1;
    else if (strcmp(argv[3],"--max") == 0)
      premium_case=2;
    else
    {
      fprintf(stderr, "wrong string in 3rd argument (allowed strings are '--min', '--avg' or '--max')\n");
      return -1;
    }
  }
  return finalSize;
}

/**
 * main function
 * @param argc - number of arguments used on start of program
 * @param argv - array containing arguments of program
 * @return returns whether the program was processed successfully or not
 */
int main(int argc, char *argv[])
{
  struct cluster_t *clusters;


  // TODO
  if (argc > 1 && argc <= 4)
  {
    int size, finalSize = 1;

    if (((argc == 3) || (argc == 4)))
    {
      finalSize = checkSpecialArguments(argc, argv, finalSize);
      if (finalSize == -1)
        return EXIT_FAILURE;
    }

    if ((size = load_clusters(argv[1], &clusters)) == -1)
      return EXIT_FAILURE;

    int newSize = setClustersCount(clusters, size, finalSize);
    if (newSize == -1)            //if function returns -1, exit program
    {
      clearClusters(clusters, size);
      return EXIT_FAILURE;
    }
    size = newSize;

    print_clusters(clusters, size);
    clearClusters(clusters, size);

    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

