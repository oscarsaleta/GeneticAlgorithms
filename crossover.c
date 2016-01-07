#include <stdio.h>
#include <stdlib.h>
#include "genericFunctions.h"
#include "crossover.h"

int cxC(int *parent1, int *parent2, int *child, size_t ncities) {
    int i, cycle=0, nextval, current, start;
    
    for (i=0;i<ncities-1;i++) 
        child[i] = -1; //we initialize the vector with impossible values
    start = rand()%(ncities-1);

    child[start] = parent1[start];
    current = start;
    while (!cycle) {
        if ((nextval = parent2[current])==parent1[start])
            break;
        for (i=0;i<ncities-1;i++) {
            if (parent1[i]==nextval) {
                child[i] = nextval;
                current = i;
            }
        }
    }
    
    for(i=0;i<ncities-1;i++) {
        if(child[i]==-1) 
            child[i] = parent2[i];
    }
    return 0;
}

int oxC(int *parent1, int *parent2, int *child, size_t ncities) {
    int i,j;
    int start,end,aux;
    int copy[ncities-1];
    for (i=0;i<ncities-1;i++) {
        copy[i] = parent2[i];
    }
    start = rand()%(ncities-1);
    end = rand()%(ncities-1);
    if (end<start) {
        aux = end;
        end = start;
        start = aux;
    }
    
    for (i=start;i<=end;i++) {
        child[i] = parent1[i];
        for (j=0;j<ncities-1;j++) {
            if (copy[j] == parent1[i]) {
                copy[j] = -1;
                break;
            }
        }
    }
    /*fprintf(stderr,"cp:");
    for (i=0;i<ncities-1;i++)
        fprintf(stderr,"%2d,",copy[i]);
    fprintf(stderr,"\n");*/

    for (i=0;i<ncities-1;i++) {
        if (i<start || i>end) {
            for (j=0;j<ncities-1;j++) {
                if (copy[j]!=-1) {
                    child[i] = copy[j];
                    copy[j] = -1;
                    break;
                }
            }
        }
    }

    return 0;
}
      
int orderC(int *parent1, int *parent2, int *child, size_t ncities) {
    int i,j,aux;
    int pos1,pos2;
    int copy[ncities-1];
    for (i=0;i<ncities-1;i++)
        copy[i] = parent2[i];
    pos1 = rand()%(ncities-1);
    pos2 = rand()%(ncities-1);
    if(pos1>pos2) { //with these we have the segment we copy
        aux = pos2;
        pos2 = pos1;
        pos1 = aux;
    }
    for(i=0;i<ncities-1;i++) {  
        if (i<pos1 || i>pos2) {
            child[i] = -1; //if we are outside the interval -1
        } else {
            for (j=0;j<ncities-1;j++) { //we remove the value from parent2
                if (parent2[j]==parent1[i])
                    copy[j] = -1;
            }
            child[i] = parent1[i]; //we copy the subset
        }
    }
    for (i=0;i<ncities-1;i++) {
        if (child[i] == -1) { // for every -1 in child
            for (j=0;j<ncities-1;j++) {
                if (copy[j]!=-1) { // find the first parent2 not used value
                    child[i] = copy[j]; // copy to the child
                    copy[j] = -1; // erase from parent
                }
            }
        }
    }
    return 0;
}
