#include <stdio.h>
#include <stdlib.h>
#include "genericFunctions.h"
#include "crossover.h"

/* OX Crossover
 *  Performs OX Crossover using 2 parents and forms 1 child (can be
 *  called with parent2 as parent1 to create a different child)
 * Input:
 *  *parent1: (ncities-1) pointer to 1st parent
 *  *parent2: (ncities-1) pointer to 2nd parent
 *  *child: (ncities-1) pointer to child
 *  ncities: total number of cities
 * Output:
 *  *child: memory modified, contains the outcome of the crossover
 */
int oxC(int *parent1, int *parent2, int *child, size_t ncities) {
    /* Loop Variables */
    int i,j;
    /* Substring selection variables */
    int start,end,aux;
    /* Copy of parent2 (we cannot modify the original) */
    int copy[ncities-1];
    for (i=0;i<ncities-1;i++) {
        copy[i] = parent2[i];
    }
    /* Randomly select a substring */
    start = rand()%(ncities-1);
    end = rand()%(ncities-1);
    if (end<start) {
        aux = end;
        end = start;
        start = aux;
    }
    /* Copy it to child and remove elements from copy (parent2) */
    for (i=start;i<=end;i++) {
        child[i] = parent1[i];
        for (j=0;j<ncities-1;j++) {
            if (copy[j] == parent1[i]) {
                copy[j] = -1;
                break;
            }
        }
    }
    /* Look for remaining elements and copy them from parent2
     * to child in order */
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
