#include <stdio.h>
#include <stdlib.h>

#include "genericFunctions.h"
#include "mutations.h"

int mutateChild(int *child, size_t ncities, double mutationrate) {
    applyMutations(&child,ncities,1,mutationrate);
    return 0;
}

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

int inversionM(int *ind, size_t ncities) {
    int copy[ncities-1];
    int i;
    int pos1,pos2;
    for (i=0;i<ncities-1;i++)
        copy[i]=ind[i];//first we copy the vector 
    pos1 = rand()%(ncities-1); 
    pos2 = rand()%(ncities-1);//we select two positions and we have to compare which one is greater
    if (pos2>pos1) {
        for (i=0;i<=(pos2-pos1);i++)
            ind[pos1+i] = copy[pos2-i];
    } else if (pos1>pos2) {
        for (i=0;i<=(pos1-pos2);i++)
            ind[pos2+i] = copy[pos1-i];
    }
    return 0;
}

int insertionM(int *ind, size_t ncities) {
    int aux,i;
    int pos = rand()%(ncities-1); 
    int city = rand()%(ncities-1);//we select two positions: the city and the position we insert it in
    aux = ind[city];//we copy the value of the city so that we can recover it later
    if (city>pos) {
        for (i=city;i>pos;i--)
            ind[i] = ind[i-1];//we displace everything forward if the city we are moving goes to an earlier position
    } else if (pos>city) {
        for (i=city;i<pos;i++)
            ind[i] = ind[i+1];//in this case we move all backwards
    } else {
        // Do nothing
        return 1;
    }
    ind[pos] = aux;//we insert the city in the vector
    return 0;
}

int exchangeM(int *ind, size_t ncities) {
    int aux;
    int pos1 = rand()%(ncities-1); 
    int pos2 = rand()%(ncities-1);
    aux = ind[pos1];
    ind[pos1] = ind[pos2];
    ind[pos2] = aux;
    return 0;
}

int displacementM(int *ind, size_t ncities){
    int copy[ncities-1];
    int i,j,aux;
    int pos1,pos2,pos3;
    for(i=0;i<ncities-1;i++)
        copy[i] = -1;//first we initialize the vector 
    pos1 = rand()%(ncities-1);
    pos2 = rand()%(ncities-1);//we select two positions and we have to compare which one is greater
    if(pos1>pos2) {
        aux = pos2;
        pos2 = pos1;
        pos1 = aux;
    }
    do {
        pos3 = rand()%(ncities-1-(pos2-pos1));//position where we displace the chunk
    } while(pos3==pos1);
    for(i=pos1;i<=pos2;i++) {
              copy[i+pos3-pos1] = ind[i];//we copy the chunk of the vector displaced
              ind[i] = -1;//we erase the part we copied in the original vector
    }
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
    if(pos3!=pos1)
        for(i=0;i<ncities-1;i++)
            ind[i] = copy[i];
    return 0;
}
