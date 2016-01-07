#ifndef M_CHILD
#define M_CHILD
int mutateChild(int *child, size_t ncities, double mutationrate);
#endif

#ifndef M_APPLY
#define M_APPLY
int applyMutations(int **pop, size_t ncities, size_t popsize, double mutationrate);
#endif

#ifndef M_INVERSION
#define M_INVERSION
int inversionM(int *ind, size_t ncities);
#endif

#ifndef M_INSERTION
#define M_INSERTION
int insertionM(int *ind, size_t ncities);
#endif

#ifndef M_EXCHANGE
#define M_EXCHANGE
int exchangeM(int *ind, size_t ncities);
#endif

#ifndef M_DISPLACEMENT
#define M_DISPLACEMENT
int displacementM(int *id, size_t ncities);
#endif
