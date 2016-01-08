#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "genericFunctions.h"
#include "mutations.h"
#include "crossover.h"

#ifndef MY_CONST
#define MY_CONST
    #define N_CITIES 37
    #define BCN 4
#endif

/* Main program:
 *  Arguments:
 *   popSize: (int) population size
 *   mutationRate: (double) between 0 and 1
 *   nParents: (int) how many parents are used to create new generation
 *      (parents are also passed to the next generation)
 *   maxGenerations: (int) how many generations we simulate
 *   tournamentSize: (int) size of each tournament
 *   filename: (char*) name of output file where the best of each generation
 *      is stored
 * In/Out:
 *  stdout: (out) prints the best of all generations and its fitness
 *  filename: (out) stores the best of each generation along with its fitness
 *  fit_filename: (out) stores two columns: generation and fitness (for plots)
 */
int main(int argc, char *argv[]) {
    /* Loop variables */
    int i,j,k;
    /* File variables */
    char filename[50],filename2[55];
    FILE *fp,*fp2;
    /* Variables that control the size of the problem */
    int popSize,nParents,maxGenerations,tournamentSize;
    /* Vectors and matrices used */
    int *cities,**mat,**pop,**next;
    char **strs;
    /* Crossover variables */
    int *parent1,*parent2,*child1,*child2;
    /* Mutation variables */
    double mutationRate;
    int mutExtra;
    /* Fitness comparison variables */
    int best,fit,fitbest;
    int *bestOfAll,fitBOA=50000;

    /* Getting variables from program arguments */
    if (argc!=7
            || sscanf(argv[1],"%d",&popSize)!=1
            || sscanf(argv[2],"%lf",&mutationRate)!=1
            || sscanf(argv[3],"%d",&nParents)!=1
            || sscanf(argv[4],"%d",&maxGenerations)!=1
            || sscanf(argv[5],"%d",&tournamentSize)!=1
            || sscanf(argv[6],"%s",filename)!=1) {
        fprintf(stderr,"genetic:: population_size mutation_rate n_parents max_generations tournament_size filename\n");
        return 1;
    }
    /* Check for some possible errors */
    if (popSize<=nParents || mutationRate>=1) {
        fprintf(stderr,"genetic:: invalid population size/parent number/mutation rate.\n");
        return 1;
    }


    fprintf(stderr,"TRAVELLING SALESMAN\n");

    /* Memory allocation for every pointer and double pointer */
    fprintf(stderr,"- Allocating memory...");
    cities    = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(cities!=NULL);
    parent1   = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(parent1!=NULL);
    parent2   = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(parent2!=NULL);
    child1    = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(child1!=NULL);
    child2    = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(child2!=NULL);
    mat       = (int**) malloc( N_CITIES   *sizeof(int*));  assert(mat!=NULL);
    pop       = (int**) malloc( popSize    *sizeof(int*));  assert(pop!=NULL);
    next      = (int**) malloc( nParents   *sizeof(int*));  assert(next!=NULL);
    strs      = (char**)malloc( N_CITIES   *sizeof(char*)); assert(strs!=NULL);
    bestOfAll = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(bestOfAll!=NULL);
    for (i=0;i<N_CITIES;i++) {
        mat[i]  = (int*) malloc( N_CITIES   *sizeof(int));  assert(mat[i]!=NULL);
        strs[i] = (char*)malloc( 40         *sizeof(char)); assert(strs[i]!=NULL);
    }
    for (i=0;i<popSize;i++) {
        pop[i]  = (int*) malloc((N_CITIES-1)*sizeof(int));  assert(pop[i]!=NULL);
    }
    for (i=0;i<N_CITIES-1;i++) {
        next[i] = (int*) malloc((N_CITIES-1)*sizeof(int)); assert(next[i]!=NULL);
    }
    fprintf(stderr," Done\n");
    /*---*/

    /* Read the distance matrix */
    srand(time(NULL));
    fprintf(stderr,"- Reading distance matrix...");
    readCities(mat, strs, N_CITIES, 40);
    fprintf(stderr," Done\n");
    /*---*/

    /* Generate random first generation */
    fprintf(stderr,"- Generating population...");
    generatePopulation(cities,pop,N_CITIES,popSize,BCN);
    fprintf(stderr," Done\n");
    /*---*/

    /* Open files 1 and 2 */
    fp=fopen(filename,"w");
    strcpy(filename2,"fit_");
    strcat(filename2,filename);
    fp2=fopen(filename2,"w");
    fprintf(fp2,"# ");
    for (i=0;i<7;i++)
        fprintf(fp2,"%s ",argv[i]);
    fprintf(fp2,"\n");

    /* Loop through all generations */
    for (k=1;k<=maxGenerations;k++) {
        /* Stderr progress indicator */
        fprintf(stderr,"- Generation ");
        fprintf(stderr,"%6d of %6d\r",k,maxGenerations);
        /* Search the fittest of the generation */
        best = 0;
        fitbest = calculateFitness(pop[best],mat,N_CITIES,BCN);
        for(i=0;i<popSize;i++){
            fit = calculateFitness(pop[i],mat,N_CITIES,BCN);
            if (fit<fitbest) {
                best = i;
                fitbest = fit;
            }
        }
        /* Print the fittest to fp */
        fprintf(fp,"#gen: %d\n",k);
        fprintf(fp,"#Best: %6d, distance: %5d\n",best,fitbest);
        for (i=0;i<N_CITIES-1;i++)
            fprintf(fp,"%2d #%s\n",pop[best][i],strs[pop[best][i]]);
        fprintf(fp,"\n\n");
        /* Print the pair generation,fitness to fp2 */
        fprintf(fp2,"%5d %5d\n",k,fitbest);
        /* Compare the best of this generation with the best of all time */
        if (fitbest<fitBOA) {
            for (i=0;i<N_CITIES-1;i++)
                bestOfAll[i] = pop[best][i];
            fitBOA = fitbest;
        }
        /* Perform tournament to selet the parents of this generation */
        tournament(pop,mat,N_CITIES,popSize,nParents,next,tournamentSize,BCN);
        /* Make sure the fittest goes to the next generation (elitism)
         * probably will also be repeated because can be a parent */
        memcpy(pop[popSize-1],pop[best],(N_CITIES-1)*sizeof(int));
        /* Generate new generation using crossover (only popSize-nParents-1
         * because all the parents and the fittest go to the next generation) */
        mutExtra = 0;
        for (i=0;i<popSize-1-nParents-1;i+=2) {
            /* Pick two parents at random */
            memcpy(parent1,next[rand()%nParents],(N_CITIES-1)*sizeof(int));
            memcpy(parent2,next[rand()%nParents],(N_CITIES-1)*sizeof(int));
            /* Form one child */
            oxC(parent1,parent2,child1,N_CITIES);
            /* Form another with parents swapped */
            oxC(parent2,parent1,child2,N_CITIES);
            /* Check if children are equal. If so, mutate */
            mutExtra = 1;
            for (j=0;j<N_CITIES-1;j++) {
                if (child1[i] != child2[i]) {
                    mutExtra = 0;
                    break;
                }
            }
            if (mutExtra == 1)
                mutateChild(child1,N_CITIES);
            /* Copy children to population */
            memcpy(pop[i],child1,(N_CITIES-1)*sizeof(int));
            memcpy(pop[i+1],child2,(N_CITIES-1)*sizeof(int));
        }
        /* Copy parents to new generation (elitism) */
        for (i=popSize-1-nParents-1;i<popSize-2;i++) {
            memcpy(pop[i],next[i-popSize-1],(N_CITIES-1)*sizeof(int));
        }
        /* Mutate all the individuals with probability mutationRate (exploratory) */
        applyMutations(pop,N_CITIES,popSize,mutationRate);
    }
    /* Print best of all through stdout */
    fprintf(stdout,"# ");
    for (i=0;i<7;i++)
        fprintf(stdout,"%s",argv[i]);
    fprintf(stdout,"\n");
    fprintf(stdout,"# Fitness: %d\n",fitBOA);
    for (i=0;i<N_CITIES-1;i++)
        fprintf(stdout,"%2d #%s\n",bestOfAll[i],strs[bestOfAll[i]]);
    fprintf(fp,"\n\n");

    fclose(fp);
    fclose(fp2);

    free(cities);
    free(parent1);
    free(parent2);
    free(child1);
    free(child2);
    doubleFreeInt(mat,N_CITIES);

    fprintf(stderr,"\nFin.\n");

    return 0 ;
}
