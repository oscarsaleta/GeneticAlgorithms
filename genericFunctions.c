#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "genericFunctions.h"

int readCities(int **mat, char **strs, size_t ncities, size_t d_strs2) {
    char buffer[305];
    char *record;
    int i=0,j=0;
    FILE *fstream = fopen("taulaciutats.csv","r");
    if(fstream == NULL) {
        fprintf(stderr,"readCities:: file opening failed\n");
        return 1 ;
    }

    fgets(buffer,305,fstream);
    buffer[0] = '\0';
    for (i=0;i<=ncities;i++) {
        fgets(buffer,305,fstream);
        if (i>0) {
            record = strtok(buffer,";");
            strcpy(strs[i-1],record);
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

int shuffle(int *cities, int *pop, size_t ncities, int n, int bcn) {
    int i;
    for (i=0;i<ncities-1;i++) { //here we initialize the vector we will use to create the population
       if (i<(bcn-1))
           cities[i] = i;
       else
           cities[i] = i+1;                      
    }
    if (n>1) {
        for (i=0;i<n-1;i++) {
          int j = i+rand()/(RAND_MAX/(n-i)+1);
          int t = cities[j];
          cities[j] = cities[i];
          cities[i] = t;
        }
    }
    for(i=0;i<n;i++)
        pop[i] = cities[i];
    return 0;
}

int generatePopulation(int *cities, int **pop, size_t ncities, size_t popsize, int bcn) {
     int i;
     for(i=0;i<popsize;i++) {
        shuffle(cities,pop[i],ncities,ncities-1,bcn);  
     }
     return 0;
}

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

int fittest(int **pop, int **mat, size_t ncities, size_t popsize, int **next, int n, int bcn){
    //FALTA COMPROVAR n is the number of individuals that will survive and we shall save them in nextGen
    int i,j=0,k=-1;
    int fitness[popsize];
    int low,last=0;
    for (i=0;i<popsize;i++) {
        fitness[i] = calculateFitness(pop[i],mat,ncities,bcn);
    }
    while (j<n) {
        low = 50000;
        for (i=0;i<popsize;i++) { //we want to find the n fittest individuals
            if( fitness[i]<low || fitness[i]>=last){
                low = fitness[i];            
                k = i;
            }    
        }
        for (i=0;i<ncities-1;i++)
            next[j][i] = pop[k][i];//we copy the chosen vector into next gen
        last = low; //this will be our lower bound
        j++;                        
    }
    return 0;
}

int roulette(int **pop, int **mat, size_t ncities, size_t popsize, int **next, int n, int bcn){
    //n is the number of individuals we want to pass to the next generation.
    int i,j,k;
    int fitness[popsize];
    double sum=0, partialsum=0;
    int r=0;
    for (i=0;i<popsize;i++) {
        fitness[i] = calculateFitness(pop[i],mat,ncities,bcn);
        sum += fitness[i]; //we sum their fitnesses
    }
    j = 0;
    while (j<n) { //we do as many roulette throws as individuals we want to save
        r = rand()%((int)sum-1)+1; //this is where the roulette stops (we don't want a zero here so that at least we have to do the following once
        i = -1;
        while (partialsum < r) {//with this we find in which element the roulette landed
             i++;
             partialsum += fitness[i];
        }
        for (k=0;k<ncities-1;k++)
            fprintf(stderr,"%d,%d,%d\n",i,j,k);
            next[j][k] = pop[i][k];
        j++;
        sum = 0;
        partialsum = 0;
    }
    return 0;
}

int tournament(int **pop, int **mat, size_t ncities, size_t popsize, size_t samplesize, int **next, int t, int bcn) {
    int i,j;
    int *best,*contender;
    int fitbest,fitcontender;

    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (i=0; i<samplesize; i++) {
            best = pop[rand()%popsize];
            fitbest = calculateFitness(best,mat,ncities,bcn);
            for (j=1;j<t;j++) {
                contender = pop[rand()%popsize];
                fitcontender = calculateFitness(contender,mat,ncities,bcn);
                if (fitcontender < fitbest) {
                    best = contender;
                    fitbest = fitcontender;
                }
            }
            memcpy(next[i],best,(ncities-1)*sizeof(int));
        }
    }

    return 0;
}

int doubleFreeInt(int **v, size_t dim2) {
    int i;
    for (i=0;i<dim2;i++)
        free(v[i]);
    free(v);
    return 0;
}
int doubleFreeChar(char **v, size_t dim2) {
    int i;
    for (i=0;i<dim2;i++)
        free(v[i]);
    free(v);
    return 0;
}

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
