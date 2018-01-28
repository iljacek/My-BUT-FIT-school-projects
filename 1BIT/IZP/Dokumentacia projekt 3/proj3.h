/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
 *
 */

/**
  * Soubor:  proj3.h
  * Datum:   13.12.2017
  * Autor: Lubomir Svehla (xsvehl09)
  * Projekt: IZP Projekt 4 - Dokumentace zdrojových kódů
  * Popis viz: https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt4&csid=647751&id=12214
*/

#include <stdio.h>
#include <stdbool.h>

/// object in 2D space
struct obj_t {
    int id;     /**< id - identifier of object */
    float x;    /**< x - x coordinate */
    float y;    /**< y - y coordinate */
};

/// cluster of objects in 2D space
struct cluster_t {
    int size;           /**< size - actual number of objects stored in cluster */
    int capacity;       /**< capacity - number of objects, that can be stored in the cluster */
    struct obj_t *obj;  /**< array of objects that are obtained in the cluster */
};

/// @file

/// Global variable that symbolizes the algorithm we use to determine distance between clusters
int premium_case;


/** @defgroup MemoryFunctions Functions working with memory
 *  Functions that allocate and deallocate space in memory
 *  @{
 *  @} */  // end of MemoryFunctions
 */


/** @defgroup DistComputation Distance Computation
 *  Functions that compute the distance between objects and clusters
 *  @{
 *  @} */  // end of DistComputation
 */


/** @defgroup GetData Get and check data
 *  Functions that get data from user input and check the data validity
 *  @{
 *  @} */  // end of GetData
 */


/**
 * Initializes cluster. Allocates memory for capacity of object. Pointer NULL in array of objects symbolizes 0 capacity.
 * @ingroup MemoryFunctions
 * @param c         cluster that is initialized
 * @param cap       capacity of cluster
 * @pre             cap > 0
 * @post            c->capacity = cap
 * @post            c->size = 0
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * Removes all the objects from cluster and initializes cluster to empty cluster.
 * @ingroup MemoryFunctions
 * @param c         cluster that is going to be removed
 * @post            c->obj = NULL
 * @post            c->capacity = 0
 */
void clear_cluster(struct cluster_t *c);

/// Chunk of cluster objects. Value recommended for reallocation.

extern const int CLUSTER_CHUNK;

/**
 * Changes capacity of cluster to new capacity.
 * @ingroup MemoryFunctions
 * @param c         cluster, whose capatity is going to be changed
 * @param new_cap   new capacity of cluster
 * @return          returns modified cluster
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Appends object to end of cluster. If cluster is too small, it extends the capacity of cluster.
 * @param c         cluster that is going to be extended
 * @param obj       object that will be appended
 * @pre             c->capacity <= c->size
 * @post            c->obj[c->size++] = obj
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Appends objects of c2 to c1 and sorts objects in c1 in ascending order by their ID.
 * @param c1        array of clusters that will obtain objects of another one
 * @param c2        array of clusters that will hand his objects to c1
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Removes cluster from field of clusters.
 * @ingroup MemoryFunctions
 * @param carr      array of clusters
 * @param narr      number of clusters in array
 * @param idx       id of cluster that will be removed
 * @return          returns the new number of clusters in array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * computes Euclidean distance between two objects
 * @ingroup DistComputation
 * @param o1        first object that is being compared
 * @param o2        second object that is being compared
 * @return          returns the computed distance
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Computes the distance between two clusters.
 * @ingroup DistComputation
 * @param c1        first cluster that is being compared
 * @param c2        second cluster that is being compared
 * @return          returns the computed distance
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Finds two closest clusters.
 * @ingroup DistComputation
 * @param carr      array of clusters
 * @param narr      number of clusters in array
 * @param c1        pointer to address in which id of first neighbour is stored
 * @param c2        pointer to address in which id of second neighbour is stored
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Sorts objects in cluster in ascending order according to their identifier
 * @param c         cluster that is going to be sorted
 */
void sort_cluster(struct cluster_t *c);

/**
 * Prints cluster to stdout
 * @param c         cluster that is going to be printed
 */
void print_cluster(struct cluster_t *c);

/**
 * Initializes array of narr clusters.
 * @ingroup MemoryFunctions
 * @param carr      array of clusters
 * @param narr      number of clusters in array
 */
void initClusters(struct cluster_t **carr, const int narr);

/**
 * Removes the array of clusters
 * @ingroup MemoryFunctions
 * @param carr      array of clusters that will be removed
 * @param narr      nuber of clusters in array
 */
void clearClusters(struct cluster_t *carr, const int narr);

/**
 * checks whether the number has value from pre defined range
 * @ingroup GetData
 * @param input     number that is being tested
 * @param index     index of number read from token (first (index 0) is ID and the others (index 1 and 2) are
 *                  coordinates)
 * @param line      current line of file taht is being processed
 * @return          returns the tested number on success,
 *                  returns -1 if unsuccessful
 */
int checkNumberValidity(int input, int index, int line);

/**
 * checks if the first line of text contains only string "count=" and integer number, also if array of clusters was
 * loaded successfully
 * @ingroup GetData
 * @param file      file containing number of objects and info about objects themselves-
 * @param token     string containing data from one line of file, that is being processed at the moment
 * @param arr       array of clusters
 * @return          returns number of objects on success,
 *                  returns -1 if unsuccessful
 */
int checkFirstLine(FILE *file, char *token, struct cluster_t **arr);

/**
 * stores the numbers read from token to object and stores this object in cluster array
 * @ingroup GetData
 * @param arr           array of clusters
 * @param readNumbers   contains numbers that were read from token
 * @param currentObject contains the index of current Object that is being stored
 */
void saveObjectLines(struct cluster_t **arr, int *readNumbers, int currentObject);

/**
 * checks if line of text contains only 3 integer numbers, if they are of allowed value and if there isn§t already
 * object with the same ID
 * @ingroup GetData
 * @param file          file containing number of objects and info about objects themselves
 * @param arr           array of clusters
 * @param token         string containing data from one line of file, that is being processed at the moment
 * @param currentObject value between 0 and objectsCount, contains the index of current Object that is being checked
 * @param objectsCount  count of objects, that was specified in first line of file
 * @param ids           array containing IDs of all objects that were loaded so far
 * @return              returns 0 if no error was found,
 *                      returns 1 if error was found
 */
bool checkObjectLines(FILE *file, struct cluster_t **arr, char *token, int currentObject, int objectsCount, int *ids);

/**
 * Loads objects from file. Creates cluster for each object and saves it to array of clusters. Allocates space for
 * array of all the clusters and saves the pointer to the first element.
 * @ingroup GetData
 * @param filename  file from which the data are read
 * @param arr       array of clusters
 * @return          returns number of objects read, if successful,
 *                  returns -1 if unsuccessful
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Prints array of clusters
 * @param carr      array of clusters
 * @param narr      number of clusters in array
*/
void print_clusters(struct cluster_t *carr, int narr);

/**
 * finds closest clusters and merges them, until there is only finalSize number of clusters
 * @ingroup DistComputation
 * @param carr      array of clusters
 * @param narr      number of clusters in array
 * @param finalSize final number of clusters specified by user
 * @return          returns new number of clusters in array, if successful,
 *                  returns -1 if not successful
 */
int setClustersCount(struct cluster_t *carr, int narr, const int finalSize);

/**
 * checks if the second parameter of user input contains only number that is higher than 0
 * @ingroup GetData
 * @param argv      argument, that should contain the desired number of clusters
 * @return          returns user specified number of clusters, if successful,
 *                  returns -1, if not successful
 */
int userInputValidation(char *argv);

/**
 * if program started with more than 1 argument, checks number and validity of other arguments
 * it also reads finalSize and chooses how the distance will be computed
 * @ingroup GetData
 * @param argc      number of arguments used on start of program
 * @param argv      array containing arguments of program
 * @param finalSize final number of clusters specified by user
 * @return          returns finalSize if successful,
 *                  returns -1 if unsuccessful
 */
int checkSpecialArguments(int argc, char **argv, int  finalSize);

/**
 * main function
 * @param argc      number of arguments used on start of program
 * @param argv      array containing arguments of program
 * @return          returns whether the program was processed successfully or not
 */
int main(int argc, char *argv[]);

