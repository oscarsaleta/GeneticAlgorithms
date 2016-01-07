#ifndef F_READ_CITIES
#define F_READ_CITIES
int readCities(int **mat, char **strs, size_t ncities, size_t d_strs2);
#endif

#ifndef F_SHUFFLE
#define F_SHUFFLE
int shuffle(int *cities, int *pop, size_t ncities, int n, int bcn);
#endif

#ifndef F_GEN_POP
#define F_GEN_POP
int generatePopulation(int *cities, int **pop, size_t ncities, size_t popsize, int bcn);
#endif

#ifndef F_CALC_FIT
#define F_CALC_FIT
int calculateFitness(int *pop, int **mat, size_t ncities, int bcn);
#endif

#ifndef F_FITTEST
#define F_FITTEST
int fittest(int **pop, int **mat, size_t ncities, size_t popsize, int **next, int n, int bcn);
#endif

#ifndef F_ROULETTE
#define F_ROULETTE
int roulette(int **pop, int **mat, size_t ncities, size_t popsize, int **next, int n, int bcn);
#endif

#ifndef F_TOURNAMENT
#define F_TOURNAMENT
int tournament (int **pop, int **mat, size_t ncities, size_t popsize, size_t samplesize, int **next, int t, int bcn);
#endif

#ifndef F_DOUBLE_FREE_INT
#define F_DOUBLE_FREE_INT
int doubleFreeInt(int **v, size_t dim2);
#endif

#ifndef F_DOUBLE_FREE_CHAR
#define F_DOUBLE_FREE_CHAR
int doubleFreeChar(char **v, size_t dim2);
#endif

#ifndef PRINT_POP
#define PRINT_POP
int printPop(int **pop, size_t popsize, size_t ncities, FILE *out);
#endif
