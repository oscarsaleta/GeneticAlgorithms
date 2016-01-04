#include <stdlib.h>
#include "genericFunctions.h"
#include "crossover.h"

int cxC(int *parent1, int *parent2, int *child, size_t ncities) {
    int i, cycle, current, start;
    for(i=0;i<ncities-1;i++)
        child[i] = -1; //we initialize the vector with impossible values
    cycle = 0;
    current = parent2[rand()%(ncities-1)];
    start = current; //we define the starting position
    while(!cycle){ //while we have not found a cycle
        for(i=0;i<ncities-1;i++) {
             if(parent1[i]==current) {
                 current = parent2[i];
                 child[i] = parent1[i];
                 if(parent2[i]==start)
                     cycle = 1;
                 break;                    
             }                            
        }
    }
    for(i=0;i<ncities-1;i++)
        if(child[i]==-1)
            child[i] = parent2[i];
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
        if (i<pos1 || i>pos2)
            child[i] = -1; //if we are outside the interval -1
        else {
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
