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

int main(int argc, char *argv[]) {

    int i,j,k;
    char filename[30],filename2[5];
    double mutationRate;
    int popSize,nParents,maxGenerations,tournamentSize;

    int *cities,**mat,**pop,**next;
    char **strs;
    FILE *fp,*fp2;

    int best;
    int mutExtra;
    int fit,fitbest;
    int *parent1,*parent2,*child1,*child2;

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
    if (popSize<=nParents || mutationRate>=1) {
        fprintf(stderr,"genetic:: invalid population size/parent number/mutation rate.\n");
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
    pop     = (int**) malloc( popSize    *sizeof(int*));  assert(pop!=NULL);
    next    = (int**) malloc( nParents   *sizeof(int*));  assert(next!=NULL);
    strs    = (char**)malloc( N_CITIES   *sizeof(char*)); assert(strs!=NULL);
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

    /*---*/
    srand(time(NULL));
    fprintf(stderr,"- Reading distance matrix...");
    readCities(mat, strs, N_CITIES, 40);
    fprintf(stderr," Done\n");
    /*for (i=0;i<N_CITIES;i++) {
        fprintf(stderr,"%12s,",strs[i]);
        or (j=0;j<N_CITIES;j++)
            fprintf(stderr,"%4d,",mat[i][j]);
        fprintf(stderr,"\n");
    }*/
    /*---*/

    /*---*/
    fprintf(stderr,"- Generating population...");
    generatePopulation(cities,pop,N_CITIES,popSize,BCN);
    fprintf(stderr," Done\n");
    /*---*/

    fp=fopen(filename,"w");
    strcpy(filename2,"fit_");
    strcat(filename2,filename);
    fp2=fopen(filename2,"w");

    /*---*/
    for (k=1;k<=maxGenerations;k++) {
        fprintf(fp,"#gen: %d\n",k);
        fprintf(stderr,"- Generation ");
        fprintf(stderr,"%6d of %6d\r",k,maxGenerations);
        /*---*/
        best = 0;
        fitbest = calculateFitness(pop[best],mat,N_CITIES,BCN);
        for(i=0;i<popSize;i++){
            fit = calculateFitness(pop[i],mat,N_CITIES,BCN);
            if (fit<fitbest) {
                best = i;
                fitbest = fit;
            }
        }
        fprintf(fp,"#Best: %6d, distance: %5d\n",best,fitbest);
        for (i=0;i<N_CITIES-1;i++)
            fprintf(fp,"%2d #%s\n",pop[best][i],strs[pop[best][i]]);

        fprintf(fp,"\n\n");
        fprintf(fp2,"%5d %5d\n",k,fitbest);
        /*---*/

        /*---*/
        tournament(pop,mat,N_CITIES,popSize,nParents,next,tournamentSize,BCN);
        /*---*/

        //printPop(pop,popSize,N_CITIES,stderr);
        memcpy(pop[popSize-1],pop[best],(N_CITIES-1)*sizeof(int));
        mutExtra = 0;
        for (i=0;i<popSize-1-nParents-1;i+=2) {
            memcpy(parent1,next[rand()%nParents],(N_CITIES-1)*sizeof(int));
            memcpy(parent2,next[rand()%nParents],(N_CITIES-1)*sizeof(int));
            /*fprintf(stderr,"p1:");
            for (j=0;j<N_CITIES-1;j++)
                fprintf(stderr,"%2d,",parent1[j]);
            fprintf(stderr,"\n");
            fprintf(stderr,"p2:");
            for (j=0;j<N_CITIES-1;j++)
                fprintf(stderr,"%2d,",parent2[j]);
            fprintf(stderr,"\n");*/
            
            oxC(parent1,parent2,child1,N_CITIES);
            oxC(parent2,parent1,child2,N_CITIES);
            
            mutExtra = 1;
            for (j=0;j<N_CITIES-1;j++) {
                if (child1[i] != child2[i]) {
                    mutExtra = 0;
                    break;
                }
            }
            if (mutExtra == 1)
                mutateChild(child1,N_CITIES,1);

            /*fprintf(stderr,"c1:");
            for (j=0;j<N_CITIES-1;j++)
                fprintf(stderr,"%2d,",child1[j]);
            fprintf(stderr,"\n");
            fprintf(stderr,"c2:");
            for (j=0;j<N_CITIES-1;j++)
                fprintf(stderr,"%2d,",child2[j]);
            fprintf(stderr,"\n");*/
            memcpy(pop[i],child1,(N_CITIES-1)*sizeof(int));
            memcpy(pop[i+1],child2,(N_CITIES-1)*sizeof(int));
        }
        for (i=popSize-1-nParents-1;i<popSize-2;i++) {
            memcpy(pop[i],next[i-popSize-1],(N_CITIES-1)*sizeof(int));
        }
        applyMutations(pop,N_CITIES,popSize,mutationRate);
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

    fprintf(stderr,"\nFin.\n");

    return 0 ;
}
