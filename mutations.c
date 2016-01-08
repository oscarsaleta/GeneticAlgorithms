#include <stdio.h>
#include <stdlib.h>
#include "genericFunctions.h"
#include "mutations.h"

/* Mutate child:
 *  Forces mutation if two brothers are identical.
 * Input:
 *  *child: child to mutate
 *  ncities: total number of cities
 * Output:
 *  *child: is modified
 */
int mutateChild(int *child, size_t ncities) {
    /* Call applyMutations with 1 population size and probability 1 */
    applyMutations(&child,ncities,1,1);
    return 0;
}

/* Apply mutations:
 *  Goes through all the population and mutates them with probability mutationrate.
 *  The kind of mutation is also randomly selected between the 4 implemented mutations.
 * Input:
 *  **pop: (popsize)*(ncities-1) matrix of population
 *  ncities: total number of cities
 *  popsize: number of individuals in population
 *  mutationrate: number between 0 and 1 that gives the probability of mutation
 * Output:
 *  **pop: the members are overwritten if mutated
 */
int applyMutations(int **pop, size_t ncities, size_t popsize, double mutationrate) {
    int mutation = (rand()%3)+1;
    int i;
    double aux;
    for (i=0; i<popsize;i++) {
        aux = (double)rand()/(double)RAND_MAX;
        if (aux<mutationrate) {
            switch (mutation) {
                case 1:
                    inversionM(pop[i],ncities);
                    break;
                case 2:
                    insertionM(pop[i],ncities);
                    break;
                case 3:
                    exchangeM(pop[i],ncities);
                    break;
                case 4:
                    displacementM(pop[i],ncities);
                    break;
                default:
                    fprintf(stderr,"Bad mutation code\n");
                    return -1;
            }
        }
    }
    return 0;
}

/* Inversion mutation:
 * Input:
 *  *ind: pointer to the individual that will mutate (length ncities-1)
 *  ncities: total number of cities
 * Output:
 *  *ind: is rewritten
 */
int inversionM(int *ind, size_t ncities) {
    /* Loop variables */
    int i;
    /* Substring selection  variables */
    int pos1,pos2;
    /* Copy of the vector */
    int copy[ncities-1];
    for (i=0;i<ncities-1;i++)
        copy[i]=ind[i];
    /* Select positions and copy the substring in opposite order */
    pos1 = rand()%(ncities-1); 
    pos2 = rand()%(ncities-1);
    if (pos2>pos1) {
        for (i=0;i<=(pos2-pos1);i++)
            ind[pos1+i] = copy[pos2-i];
    } else if (pos1>pos2) {
        for (i=0;i<=(pos1-pos2);i++)
            ind[pos2+i] = copy[pos1-i];
    }
    return 0;
}

/* Insertion mutation:
 * Input:
 *  *ind: pointer to the individual that will mutate (length ncities-1)
 *  ncities: total number of cities
 * Output:
 *  *ind: is rewritten
 */
int insertionM(int *ind, size_t ncities) {
    /* Loop variables */
    int i;
    /* Position of city and backup */
    int aux;
    int pos = rand()%(ncities-1); 
    int city = rand()%(ncities-1);
    aux = ind[city];
    /* Displace cities to the right or left */
    if (city>pos) {
        for (i=city;i>pos;i--)
            ind[i] = ind[i-1];
    } else if (pos>city) {
        for (i=city;i<pos;i++)
            ind[i] = ind[i+1];
    } else {
        return 1;
    }
    /* Copy city */
    ind[pos] = aux;
    return 0;
}

/* Exchange mutation:
 * Input:
 *  *ind: pointer to the individual that will mutate (length ncities-1)
 *  ncities: total number of cities
 * Output:
 *  *ind: is rewritten
 */
int exchangeM(int *ind, size_t ncities) {
    int aux;
    int pos1 = rand()%(ncities-1); 
    int pos2 = rand()%(ncities-1);
    aux = ind[pos1];
    ind[pos1] = ind[pos2];
    ind[pos2] = aux;
    return 0;
}

/* Displacement mutation:
 * Input:
 *  *ind: pointer to the individual that will mutate (length ncities-1)
 *  ncities: total number of cities
 * Output:
 *  *ind: is rewritten
 */
int displacementM(int *ind, size_t ncities){
    /* Loop variables */
    int i,j;
    /* Substring selection */
    int pos1,pos2,pos3,aux;
    /* Individual backup */
    int copy[ncities-1];
    for(i=0;i<ncities-1;i++)
        copy[i] = -1;
    /* Substring selection, make sure pos1<pos2 */
    pos1 = rand()%(ncities-1);
    pos2 = rand()%(ncities-1);//we select two positions and we have to compare which one is greater
    if(pos1>pos2) {
        aux = pos2;
        pos2 = pos1;
        pos1 = aux;
    }
    /* Final position selection, cannot overflow the vector and also has to
     * be different than pos1 to have mutation */
    do {
        pos3 = rand()%(ncities-1-(pos2-pos1));//position where we displace the chunk
    } while(pos3==pos1);
    /* Copy substring to backup and erase from individual */
    for(i=pos1;i<=pos2;i++) {
          copy[i+pos3-pos1] = ind[i];
          ind[i] = -1;
    }
    /* Move other cities to the right */
    if(pos3>pos1) {
         j = ncities-2;
         for(i=ncities-2;i>=0;i--) {
             if(copy[i]==-1 && ind[j]!=-1) {
                 copy[i] = ind[j];
                 j--;            
             }                             
         }
         for(i=0;i<pos1;i++)
             copy[i] = ind[i];
    }
    /* Move other cities to the left */
    if(pos3<pos1) {
        j = 0;
        for(i=0;i<ncities-1;i++) {
            if(copy[i]==-1 && ind[j]!=-1) {
                 copy[i] = ind[j];
                 j++;            
             }                             
         }
         for(i=ncities-2;i>pos2;i--)
             copy[i] = ind[i];
    }
    /* Restore backup to individual */
    if(pos3!=pos1)
        for(i=0;i<ncities-1;i++)
            ind[i] = copy[i];
    return 0;
}
