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
    #define POP_SIZE 300
    #define MUTATION_RATE 0.01
    #define SAMPLE_SIZE 10 
    #define MAX_GENERATIONS 500
#endif

int main(int argc, char *argv[]) {

    int i,j,k;
    char filename[30],filename2[5];
    double mutationrate;

    int *cities,**mat,**pop,**next;
    char **strs;
    FILE *fp,*fp2;

    int best;
    double fit,fitbest;
    double sum;
    int *parent1,*parent2,*child1,*child2;

    if (argc!=3
            || sscanf(argv[1],"%lf",&mutationrate)!=1
            || sscanf(argv[2],"%s",filename)!=1) {
        fprintf(stderr,"genetic: mutation_rate filename\n");
        return 1;
    }

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
    for (i=0;i<N_CITIES-1;i++) {
        next[i] = (int*) malloc((N_CITIES-1)*sizeof(int)); assert(next[i]!=NULL);
    }
    fprintf(stderr," Done\n");
    /*---*/

    /*---*/
    srand(time(NULL));
    fprintf(stderr,"- Reading distance matrix...");
    readCities(mat, strs, N_CITIES, 40);
    fprintf(stderr," Done\n");
    /*for (i=0;i<N_CITIES;i++) {
        fprintf(stderr,"%12s,",strs[i]);
        for (j=0;j<N_CITIES;j++)
            fprintf(stderr,"%4d,",mat[i][j]);
        fprintf(stderr,"\n");
    }*/
    /*---*/

    /*---*/
    fprintf(stderr,"- Generating population...");
    generatePopulation(cities,pop,N_CITIES,POP_SIZE,BCN);
    fprintf(stderr," Done\n");
    /*---*/

    fp=fopen(filename,"w");
    strcpy(filename2,"fit_");
    strcat(filename2,filename);
    fp2=fopen(filename2,"w");

    /*---*/
    for (k=0;k<MAX_GENERATIONS;k++) {
        fprintf(fp,"#gen: %d\n",k);
        fprintf(stderr,"- Generation %d\n",k);
        /*---*/
        fprintf(stderr,"  - Calculating fitness...");
        best = 0;
        sum=0;
        fitbest = calculateFitness(pop[best],mat,N_CITIES,BCN);
        for(i=0;i<POP_SIZE;i++){
            fit = calculateFitness(pop[i],mat,N_CITIES,BCN);
            sum+=fit;
            if (fit<fitbest) {
                best = i;
                fitbest = fit;
            }
            fprintf(fp," fitness: %g.\n",fit);
        }
        fprintf(fp,"#Best: %d, fitness: %g\n",best,fitbest);
        fprintf(fp,"\n\n");
        fprintf(fp2,"%5d %10.7g\n",k,sum/(double)POP_SIZE);
        fprintf(stderr," Done\n");
        /*---*/

        /*---*/
        fprintf(stderr,"  - Beginning tournament...");
        tournament(pop,mat,N_CITIES,POP_SIZE,SAMPLE_SIZE,next,10,BCN);
        fprintf(stderr," Done\n");
        /*---*/

        for (i=0;i<POP_SIZE-1-SAMPLE_SIZE;i+=2) {
            memcpy(parent1,next[rand()%SAMPLE_SIZE],(N_CITIES-1)*sizeof(int));
            memcpy(parent2,next[rand()%SAMPLE_SIZE],(N_CITIES-1)*sizeof(int));
            cxC(parent1,parent2,child1,child2,N_CITIES);
            memcpy(pop[i],child1,(N_CITIES-1)*sizeof(int));
            memcpy(pop[i+1],child2,(N_CITIES-1)*sizeof(int));
        }
        for (i=POP_SIZE-1-SAMPLE_SIZE;i<POP_SIZE-1;i++) {
            memcpy(pop[i],next[i-POP_SIZE-1],(N_CITIES-1)*sizeof(int));
        }
        //applyMutations(pop,N_CITIES,POP_SIZE,mutationrate);
    }
    /*---*/


    fclose(fp);
    fclose(fp2);

    /*free(cities);
    free(parent1);
    free(parent2);
    free(child1);
    free(child2);
    doubleFreeInt(mat,N_CITIES);
    doubleFreeInt(pop,N_CITIES-1);
    doubleFreeChar(strs,N_CITIES);*/

    fprintf(stderr,"Fin.\n");
    return 0 ;
}
