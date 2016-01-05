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
    #define POP_SIZE 500
    #define MUTATION_RATE 0.15
    #define SAMPLE_SIZE 10 
    #define MAX_GENERATIONS 500
#endif

int main() {

    int i,j,k;

    int *cities,**mat,**pop,**next;
    char **strs;
    FILE *fp;

    int best;
    double fit,fitbest;
    int *parent1,*parent2,*child1,*child2;

    fprintf(stderr,"TRAVELLING SALESMAN\n");

    /*---*/
    fprintf(stderr,"- Allocating memory...");
    cities  = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(cities!=NULL);
    parent1 = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(parent1!=NULL);
    parent2 = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(parent2!=NULL);
    child1  = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(child1!=NULL);
    child2  = (int*)  malloc((N_CITIES-1)*sizeof(int));   assert(child2!=NULL);
    mat     = (int**) malloc( N_CITIES   *sizeof(int*));  assert(mat!=NULL);
    pop     = (int**) malloc( POP_SIZE   *sizeof(int*));  assert(pop!=NULL);
    next    = (int**) malloc( SAMPLE_SIZE*sizeof(int*));  assert(next!=NULL);
    strs    = (char**)malloc( N_CITIES   *sizeof(char*)); assert(strs!=NULL);
    for (i=0;i<N_CITIES;i++) {
        mat[i]  = (int*) malloc( N_CITIES   *sizeof(int));  assert(mat[i]!=NULL);
        strs[i] = (char*)malloc( 40         *sizeof(char)); assert(strs[i]!=NULL);
    }
    for (i=0;i<POP_SIZE;i++) {
        pop[i]  = (int*) malloc((N_CITIES-1)*sizeof(int));  assert(pop[i]!=NULL);
    }
    /*for (i=0;i<SAMPLE_SIZE;i++) {
        next[i] = (int*) malloc((N_CITIES-1)*sizeof(int));  assert(next[i]!=NULL);
    }*/
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
        fprintf(stderr,"- Generation %d\n",k);
        /*---*/
        fprintf(stderr,"  - Calculating fitness...");
        best = 0;
        fitbest = calculateFitness(pop[best],mat,N_CITIES,BCN);
        for(i=0;i<POP_SIZE;i++){
            for(j=0;j<(N_CITIES-1);j++){
                fprintf(fp,"%d;", pop[i][j]);
            }
            fit = calculateFitness(pop[i],mat,N_CITIES,BCN);
            if (fit>fitbest) {
                best = i;
                fitbest = fit;
            }
            fprintf(fp," fitness: %g.\n",fit);
        }
        fprintf(fp,"#Best: %d, fitness: %g\n",best,fitbest);
        fprintf(fp,"\n\n");
        fprintf(stderr," Done\n");
        /*---*/

        /*---*/
        fprintf(stderr,"  - Beginning tournament...");
        tournament(pop,mat,N_CITIES,POP_SIZE,SAMPLE_SIZE,next,10,BCN);
        fprintf(stderr," Done\n");
        /*---*/

        for (i=0;i<POP_SIZE-1;i+=2) {
            memcpy(parent1,next[rand()%SAMPLE_SIZE],N_CITIES-1);
            memcpy(parent2,next[rand()%SAMPLE_SIZE],N_CITIES-1);
            memcpy(child1,pop[i],N_CITIES-1);
            memcpy(child2,pop[i+1],N_CITIES-1);
            cxC(parent1,parent2,child1,child2,N_CITIES);
            memcpy(pop[i],child1,N_CITIES-1);
            memcpy(pop[i+1],child2,N_CITIES-1);
        }
        applyMutations(pop,N_CITIES,POP_SIZE,MUTATION_RATE);
        /*for(i=0;i<POP_SIZE;i++){
            for(j=0;j<(N_CITIES-1);j++){
                fprintf(fp,"%d;", pop[i][j]);
            }
            fprintf(fp,"\n");
        }*/

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
