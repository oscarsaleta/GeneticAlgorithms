#include <stdio.h>
#include <stdlib.h>
#include "genericFunctions.h"
#include "crossover.h"

int cxC(int *parent1, int *parent2, int *child1, int *child2, size_t ncities) {
    int i, cycle=0, nextval, current, start;
    
    for (i=0;i<ncities-1;i++) {
        child1[i] = -1; //we initialize the vector with impossible values
        child2[i] = -1;
    }
    start = rand()%(ncities-1);

    child1[start] = parent1[start];
    current = start;
    while (!cycle) {
        if ((nextval = parent2[current])==parent1[start])
            break;
        for (i=0;i<ncities-1;i++) {
            if (parent1[i]==nextval) {
                child1[i] = nextval;
                current = i;
            }
        }
    }
    child2[start] = parent2[start];
    current = start;
    while (!cycle) {
        if ((nextval = parent1[current])==parent2[start])
            break;
        for (i=0;i<ncities-1;i++) {
            if (parent2[i]==nextval) {
                child2[i] = nextval;
                current = i;
            }
        }
    }

    for(i=0;i<ncities-1;i++) {
        if(child1[i]==-1) 
            child1[i] = parent2[i];
        if (child2[i]==-1)
            child2[i] = parent1[i];
    }
    return 0;
}
      
int orderC(int *parent1, int *parent2, int *child1, int *child2, size_t ncities) {
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
            child1[i] = -1; //if we are outside the interval -1
            child2[i] = -1;
        } else {
            for (j=0;j<ncities-1;j++) { //we remove the value from parent2
                if (parent2[j]==parent1[i])
                    copy[j] = -1;
            }
            child1[i] = parent1[i]; //we copy the subset
            child2[i] = parent2[i];
        }
    }
    for (i=0;i<ncities-1;i++) {
        if (child1[i] == -1) { // for every -1 in child
            for (j=0;j<ncities-1;j++) {
                if (copy[j]!=-1) { // find the first parent2 not used value
                    child1[i] = copy[j]; // copy to the child
                    child2[i] = parent1[j];
                    copy[j] = -1; // erase from parent
                }
            }
        }
    }
    return 0;
}
