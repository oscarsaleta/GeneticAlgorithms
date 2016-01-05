# ======================
# Opcions d'optimització
# ======================
COMPILER=gcc
#OPT=-g -Wall 
OPT=-O3 -Wall  
TAIL=-lm

all : genetic

# =========
# Utilitats
# =========
genetic : genetic.o genericFunctions.o crossover.o mutations.o
	$(COMPILER) -o genetic $(OPT) genetic.o genericFunctions.o crossover.o mutations.o

# ===========
# Bibliotecas
# ===========
genetic.o : genetic.c
	$(COMPILER) -c $(OPT) genetic.c
genericFunctions.o : genericFunctions.c
	$(COMPILER) -c $(OPT) genericFunctions.c
mutations.o : mutations.c
	$(COMPILER) -c $(OPT) mutations.c
crossover.o : crossover.c
	$(COMPILER) -c $(OPT) crossover.c

# ======
# Neteja
# ======
clean :
	rm -f *.o
realclean : clean
	rm -f genetic


