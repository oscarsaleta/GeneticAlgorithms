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
    #define POP_SIZE 100
    #define MUTATION_RATE 0.01
    #define SAMPLE_SIZE 10 
#endif

int main() {

    int i=0,j=0;

    int *cities,**mat,**pop,**next;
    char **strs;
    FILE *fp;

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

    srand(time(NULL));
    readCities(mat, strs, N_CITIES, 40);
    generatePopulation(cities,pop,N_CITIES,POP_SIZE,BCN);
    /*for (i=0;i<POP_SIZE;i++){
        for(j=0;j<N_CITIES;j++)
            fprintf(stderr,"%d ",pop[i][j]);
        fprintf(stderr,"\n");
    }*/

    fp=fopen("prova.txt","w");

    for(i=0;i<POP_SIZE;i++){
        for(j=0;j<(N_CITIES-1);j++){
            fprintf(fp,"%d;", pop[i][j]);
        }
        fprintf(fp," fitness: %g.\n",calculateFitness(pop[i],mat,N_CITIES,BCN));
    }

    tournament(pop,mat,N_CITIES,POP_SIZE,SAMPLE_SIZE,next,10,BCN);

    for(i=0;i<SAMPLE_SIZE;i++){
        for(j=0;j<(N_CITIES-1);j++){
            fprintf(fp,"%d;", next[i][j]);
        }
        fprintf(fp,"\n");
    }
    /* for(j=0;j<(N_CITIES-1);j++){
    fprintf(fp,"%d;", pop[0][j]);
    }
    //displacementMutation(pop[0]);
    fprintf(fp,"%\n");
    for(j=0;j<(N_CITIES-1);j++){
    fprintf(fp,"%d;", pop[1][j]);
    }
    orderCrossover(pop[0],pop[1],pop[2]);
    fprintf(fp,"%\n");
    for(j=0;j<(N_CITIES-1);j++){
    fprintf(fp,"%d;", pop[2][j]);
    }*/

    fclose(fp);
    //free(mat);free(pop);free(cities);free(strs);free(next)//TODO FREE MEMORY

    return 0 ;
}
