#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <float.h>

#include "genericFunctions.h"
#include "mutations.h"
#include "crossover.h"

#ifndef MY_CONST
#define MY_CONST
    #define N_CITIES 37
    #define BCN 4
    #define POP_SIZE 100
    #define MUTATION_RATE 0.01
    #define SAMPLE_SIZE 10 
    #define MAX_GENERATIONS 10
#endif

int main() {

    int i=0,j=0;
    int *best;
    double fit,fitbest=-DBL_MAX;

    int *cities,**mat,**pop,**next;
    char **strs;
    FILE *fp;

    fprintf(stderr,"TRAVELLING SALESMAN\n");

    /*---*/
    fprintf(stderr,"- Allocating memory...");
    cities = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(cities!=NULL);
    mat    = (int**) malloc( N_CITIES   *sizeof(int*));  assert(mat!=NULL);
    pop    = (int**) malloc( POP_SIZE   *sizeof(int*));  assert(pop!=NULL);
    next   = (int**) malloc( SAMPLE_SIZE*sizeof(int*));  assert(next!=NULL);
    strs   = (char**)malloc( N_CITIES   *sizeof(char*)); assert(strs!=NULL);
    for (i=0;i<N_CITIES;i++) {
        mat[i]  = (int*) malloc( N_CITIES   *sizeof(int));  assert(mat[i]!=NULL);
        strs[i] = (char*)malloc( 40         *sizeof(char)); assert(strs[i]!=NULL);
    }
    for (i=0;i<POP_SIZE;i++) {
        pop[i]  = (int*) malloc((N_CITIES-1)*sizeof(int));  assert(pop[i]!=NULL);
    }
    for (i=0;i<SAMPLE_SIZE;i++) {
        next[i] = (int*) malloc((N_CITIES-1)*sizeof(int));  assert(next[i]!=NULL);
    }
    fprintf(stderr," Done\n");
    /*---*/

    /*---*/
    srand(time(NULL));
    fprintf(stderr,"- Reading distance matrix...");
    readCities(mat, strs, N_CITIES, 40);
    fprintf(stderr," Done\n");
    /*---*/

    /*---*/
    fprintf(stderr,"- Generating population...");
    generatePopulation(cities,pop,N_CITIES,POP_SIZE,BCN);
    fprintf(stderr," Done\n");
    /*---*/

    fp=fopen("prova.txt","w");

    /*---*/
    for (k=0;k<MAX_GENERATIONS;k++) {
        fprintf(fp,"#gen: %d\n",k);

        /*---*/
        fprintf(stderr,"- Calculating fitness of 1st generation...");
        for(i=0;i<POP_SIZE;i++){
            for(j=0;j<(N_CITIES-1);j++){
                fprintf(fp,"%d;", pop[i][j]);
            }
            fit = calculateFitness(pop[i],mat,N_CITIES,BCN);
            if (fit>fitbest)
                best = pop[i];
            fprintf(fp," fitness: %g.\n",calculateFitness(pop[i],mat,N_CITIES,BCN));
        }
        fprintf(stderr," Done\n");
        /*---*/

        /*---*/
        fprintf(stderr,"- Beginning tournament...");
        tournament(pop,mat,N_CITIES,POP_SIZE,SAMPLE_SIZE,next,10,BCN);
        fprintf(stderr," Done\n");
        /*---*/

        fprintf(fp,"\n\n");
        for(i=0;i<SAMPLE_SIZE;i++){
            for(j=0;j<(N_CITIES-1);j++){
                fprintf(fp,"%d;", next[i][j]);
            }
            fprintf(fp," fitness: %g.\n",calculateFitness(next[i],mat,N_CITIES,BCN));
        }
        
    }
    /*---*/


    fclose(fp);
    free(cities);
    doubleFreeInt(mat,N_CITIES);
    doubleFreeInt(pop,N_CITIES-1);
    //doublefree(next,N_CITIES-1);
    doubleFreeChar(strs,N_CITIES);
    //free(mat);free(pop);free(cities);free(strs);free(next)//TODO FREE MEMORY

    fprintf(stderr,"Fin.\n");
    return 0 ;
}
