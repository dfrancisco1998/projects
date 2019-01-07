#include "dnasearch.h"

int main(int argc,  char *argv[]){
   char query[15000] = {0};// arry for storing the pattern to match
   if(argc < 1){
      fprintf(stdout, "Not enough arguments");
   }
   int place = 0;
   char input[15000] = {0}; //where the actg from txt file go

   char* filename = argv[1];// name of txt file
  
   FILE* input_file = fopen(filename, "r");
   if(input_file == NULL){
      fprintf(stderr, "This file is empty\n");
      return 1;
   }
  
   int check = validInputFile(input_file, input);
  
   if(check==2){
      while(scanf(" %s", query) != EOF){
         int check2 = qValid(query);
         if(check2 != 2){
            fprintf(stderr, "your search pattern is invalid\n");
            return 1;
         }
         for(int i = 0; i < (int) strlen(query); i++){
            if(query[i] > 64 && query[i] < 91){
               query[i] = query[i] + 32;
            }
            else{
               query[i] = query[i];
            }
         }
         pattern_match(input, (int)strlen(input), query, (int) strlen(query), place ); 
      
      
      }
   }
   else{
      fprintf(stderr,"invalid input file\n");
      return 1;
   }
   if(ferror(input_file)){
      fprintf(stderr, "no input file\n");
   }    
   fclose(input_file);
  
  
   return 0;
}
