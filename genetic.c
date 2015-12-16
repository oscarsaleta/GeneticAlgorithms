#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N_CITIES 37 //this is actually the number of rows in our cities file.
#define BCN 4
#define POP_SIZE 100


int readCities(int mat[N_CITIES][N_CITIES], char strs[N_CITIES][40])
{
   char buffer[305];
   char *record;
   int i=0,j=0;
   FILE *fstream = fopen("taulaciutats.csv","r");
   if(fstream == NULL) {
	  printf("\n file opening failed ");
	  return 0 ;
   }

   while(fgets(buffer,305,fstream)!=NULL) {
	 record = strtok(buffer,";");
	 j=0;
	 while(record != NULL) {
		 if(j == 0){
			 if(i > 0 )
				 strcpy(strs[i-1], record);//we save the name of the cities in this vector
		 }
		 else if(i>0){
			 if(!strcmp(record,"-")) *record='0';
			 mat[i-1][j-1] = atoi(record) ; //ROW and column one are strings
		 }
		 record = strtok(NULL,";"); //Field separator
		 j++;
	 }
	 i++ ;
   }
   fclose(fstream);
   return 1;
}

void shuffle(int cities[N_CITIES-1], int pop[N_CITIES-1], int n){
   int i;
   for(i=0;i<(N_CITIES-1);i++){ //here we initialize the vector we will use to create the population
      if(i<(BCN-1)) cities[i]=i;
      else cities[i]=i+1;                      
   }
    if (n > 1) {
        for (i = 0; i < n - 1; i++) {
          int j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = cities[j];
          cities[j] = cities[i];
          cities[i] = t;
        }
    }
    for(i=0;i<n;i++) pop[i]=cities[i];
}

void generatePopulation(int cities[N_CITIES-1], int pop[POP_SIZE][N_CITIES-1]){
     int i=0;
     for(i=0;i<(POP_SIZE);i++){
        shuffle(cities,pop[i],N_CITIES-1);  
     }         
}

int calculateFitness(int pop[N_CITIES-1], int mat[N_CITIES][N_CITIES]){
    int i=0,fitness=0;
    fitness+=mat[(BCN-1)][(pop[0])]; 
    for(i=0;i<(N_CITIES-2);i++)  fitness+=mat[(pop[i])][(pop[i+1])]; 
    fitness+=mat[(pop[i])][(BCN-1)];
    return fitness;
} 


int main()
{
   int cities[N_CITIES-1];//we dont take into account BCN since it has to be the first and the last city
   int i=0,j=0;
   int mat[N_CITIES][N_CITIES];
   int pop[POP_SIZE][N_CITIES-1];
   char strs[N_CITIES][40];
   
   srand(time(NULL));
   readCities(mat, strs);
   generatePopulation(cities, pop);


   FILE *fp;
   fp=fopen("prova.txt","w");
   
   for(i=0;i<POP_SIZE;i++){
    for(j=0;j<(N_CITIES-1);j++){
       fprintf(fp,"%d;", pop[i][j]);
     }
     fprintf(fp," fitness: %d.\n",calculateFitness(pop[i],mat));
   }
   fclose(fp);


   return 0 ;
 }
