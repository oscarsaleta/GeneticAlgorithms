#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "genericFunctions.h"

/* Read cities:
 *  Reads taulaciutats.csv file and stores the distance matrix in **mat,
 *  and the city names in **strs.
 * Input:
 *  **mat: (ncities)*(ncities) distance matrix
 *  **strs: (ncities)*(d_strs2) city names
 *  ncities: total number of cities
 *  d_strs2: max length of city names
 * Output:
 *  **mat: is modified and will contain distance matrix
 *  **strs: is modified and will contain city names
 */
int readCities(int **mat, char **strs, size_t ncities, size_t d_strs2) {
    /* Buffer variable to store one line of the .csv file each time */
    char buffer[305];
    /* Tokenizer for the buffer */
    char *record;
    /* Loop variables */
    int i=0,j=0;
    /* File to read */
    FILE *fstream = fopen("taulaciutats.csv","r");
    if(fstream == NULL) {
        fprintf(stderr,"readCities:: file opening failed\n");
        return 1 ;
    }
    /* Read first line and discard */
    fgets(buffer,305,fstream);
    buffer[0] = '\0';
    /* Loop for the rest of the file */
    for (i=0;i<=ncities;i++) {
        /* Get line */
        fgets(buffer,305,fstream);
        if (i>0) {
            /* Get first token (city name) and store it */
            record = strtok(buffer,";");
            strcpy(strs[i-1],record);
            /* Loop to get all the distances for each line */
            j=1;
            while((record=strtok(NULL,";"))!=NULL) {
                if (!strcmp(record,"-"))
                    *record = '0';
                mat[i-1][j-1] = atoi(record);
                j++;
            }
            record = strtok(NULL,";");
            buffer[0] = '\0';
        }
    }
    return 0;

    fclose(fstream);
    return 0;
}

/* Shuffle:
 *  Shuffle a vector of cities to generate a random individual
 * Input:
 *  *cities: (ncities-1) empty dummy vector, used to avoid allocation at every call
 *  *pop: (n) emtpy individual
 *  ncities: total number of cities
 *  n: size of individual
 *  bcn: number of Barcelona in the distance matrix (3)
 * Output:
 *  *cities: not usable output
 *  *pop: brand new random city route
 */
int shuffle(int *cities, int *pop, size_t ncities, int n, int bcn) {
    /* Loop variable */
    int i;
    /* Initialize cities with all cities except Barcelona */
    for (i=0;i<ncities-1;i++) {
       if (i<(bcn-1))
           cities[i] = i;
       else
           cities[i] = i+1;                      
    }
    /* Change the order of the cities */
    if (n>1) {
        for (i=0;i<n-1;i++) {
          int j = i+rand()/(RAND_MAX/(n-i)+1);
          int t = cities[j];
          cities[j] = cities[i];
          cities[i] = t;
        }
    }
    /* Copy the result to pop */
    for(i=0;i<n;i++)
        pop[i] = cities[i];
    return 0;
}

/* Generate population:
 *  Uses shuffle to generate popsize random routes
 * Input:
 *  *cities: (ncities-1) dummy city vector for shuffle
 *  **pop: (popsize)*(ncities-1) population matrix
 *  ncities: total number of cities
 *  popsize: number of individuals in population
 *  bcn: Barcelona (3)
 * Output:
 *  *cities: not usable output
 *  **pop: population matrix of random individuals
 */
int generatePopulation(int *cities, int **pop, size_t ncities, size_t popsize, int bcn) {
     int i;
     for(i=0;i<popsize;i++) {
        shuffle(cities,pop[i],ncities,ncities-1,bcn);  
     }
     return 0;
}

/* Calculate fitness:
 *  IMPORTANT: in this implementation, the fittest is the individual with LOWER
 *  fitness (because fitness is computed as the total distance travelled)
 * Input:
 *  *pop: (ncities-1) individual (route) to measure
 *  **mat: (ncities)*(ncities) distance matrix
 *  ncities: total number of cities
 *  bcn: Barcelona (3)
 * Return:
 *  Value for the fitness of the individual
 */
int calculateFitness(int *pop, int **mat, size_t ncities, int bcn){
    int i;
    int fitness;
    fitness = mat[bcn-1][pop[0]]; 
    //fprintf(stderr," i:%2d->%2d: %5d\n",bcn-1,pop[0],fitness);
    for (i=0;i<(ncities-1)-1;i++) {
        //fprintf(stderr,"%2d:%2d->%2d: %5d\n",i,pop[i],pop[i+1],mat[pop[i]][pop[i+1]]);
        fitness += mat[pop[i]][pop[i+1]]; 
    }
    //fprintf(stderr," f:%2d->%2d: %5d\n",pop[ncities-2],bcn-1,mat[pop[ncities-2]][bcn-1]);
    fitness += mat[pop[ncities-2]][bcn-1];
    //fprintf(stderr,"Total:%5d\n\n\n",fitness);
    return fitness;
}

/* Tournament selection:
 *  Pick one individual at random, make it compete with other t individuals picked at random,
 *  and return the best of them. Repeat for the number of parents we want.
 * Input:
 *  **pop: (popsize)*(ncities-1) population matrix
 *  **mat: (ncities)*(ncities) distance matrix
 *  samplesize: how many parents do we want to find
 *  **next: (samplesize)*(ncities-1) matrix with all the parents
 *  t: tournament size
 *  bcn: Barcelona (3)
 * Output:
 *  **next: is modified, will contain the parents found in the tournament
 */
int tournament(int **pop, int **mat, size_t ncities, size_t popsize, size_t samplesize, int **next, int t, int bcn) {
    /* Loop variables */
    int i,j;
    /* Best, contender and their fitness */
    int *best,*contender;
    int fitbest,fitcontender;

    /* This routine is really simple to parallelize and it will improve
     * execution time significantly */
    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (i=0; i<samplesize; i++) {
            /* Pick first at random and compute fitness */
            best = pop[rand()%popsize];
            fitbest = calculateFitness(best,mat,ncities,bcn);
            for (j=1;j<t;j++) {
                /* Compete with random contender */
                contender = pop[rand()%popsize];
                fitcontender = calculateFitness(contender,mat,ncities,bcn);
                if (fitcontender < fitbest) {
                    best = contender;
                    fitbest = fitcontender;
                }
            }
            /* Copy the best to the parents matrix */
            memcpy(next[i],best,(ncities-1)*sizeof(int));
        }
    }

    return 0;
}

/* Double free routines:
 *  Functions to ease the process of freeing memory for double pointers
 */
int doubleFreeInt(int **v, size_t dim) {
    int i;
    for (i=0;i<dim;i++)
        free(v[i]);
    free(v);
    return 0;
}

/* Print population:
 *  (Debugging purposes) Auxiliar function to make population printing faster
 */
int printPop(int **pop, size_t popsize, size_t ncities, FILE *out) {
    int i,j;
    for (i=0;i<popsize;i++) {
        for (j=0;j<ncities-1;j++)
            fprintf(out,"%3d,",pop[i][j]);
        fprintf(out,"\n");
    }
    fprintf(out,"\n");
    return 0;
}
