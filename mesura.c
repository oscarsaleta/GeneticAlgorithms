#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genericFunctions.h"

int main() {
    int i;
    int *optim;
    int opt[36];//[]={20,16,19,12,5,36,23,31,30,11,8,18,26,15,7,17,10,6,1,0,32,29,9,14,4,27,35,2,34,24,28,13,22,33,25,21};
    optim=(int*)malloc(36*sizeof(int));

    for (i=0;i<36;i++)
        fscanf(stdin,"%d",&opt[i]);

    for (i=0;i<36;i++)
        optim[i] = opt[i];

    int **mat;
    char **strs;
    strs    = (char**)malloc( 37   *sizeof(char*)); 
    mat     = (int**) malloc( 37   *sizeof(int*));  
    for (i=0;i<37;i++) {
        mat[i]  = (int*) malloc( 37   *sizeof(int));  
        strs[i] = (char*)malloc( 40         *sizeof(char)); 
    }
    readCities(mat, strs, 37, 40);
    fprintf(stderr,"DIST:%5d\n",calculateFitness(optim,mat,37,4));
    return 0;
}

