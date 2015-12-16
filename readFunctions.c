#include "readFunctions.h"

int readCities(int mat[N_CITIES][N_CITIES], char strs[N_CITIES][40]) {
   char buffer[305];
   char *record;
   int i=0,j=0;
   FILE *fstream = fopen("taulaciutats.csv","r");
   if(fstream == NULL) {
	  fprintf(stderr,"readCities:: file opening failed\n");
	  return 1 ;
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
   return 0;
}
