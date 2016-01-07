# ======================
# Opcions d'optimització
# ======================
COMPILER=gcc
OPT=-g -Wall 
#OPT=-O3 -Wall  
TAIL=-fopenmp
#TAIL=

all : genetic

# =========
# Utilitats
# =========
genetic : genetic.o genericFunctions.o crossover.o mutations.o
	$(COMPILER) -o genetic $(OPT) genetic.o genericFunctions.o crossover.o mutations.o $(TAIL)
mesura : mesura.o genericFunctions.o
	$(COMPILER) -o mesura $(OPT) mesura.o genericFunctions.o $(TAIL)

# ===========
# Bibliotecas
# ===========
genetic.o : genetic.c
	$(COMPILER) -c $(OPT) genetic.c
genericFunctions.o : genericFunctions.c
	$(COMPILER) -c $(OPT) genericFunctions.c $(TAIL)
mutations.o : mutations.c
	$(COMPILER) -c $(OPT) mutations.c $(TAIL)
crossover.o : crossover.c
	$(COMPILER) -c $(OPT) crossover.c $(TAIL)
mesura.o : mesura.c
	$(COMPILER) -c $(OPT) mesura.c $(TAIL)

# ======
# Neteja
# ======
clean :
	rm -f *.o
realclean : clean
	rm -f genetic


