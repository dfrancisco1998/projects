#include "dnasearch.h"
//This will check to see if the file to be scnanned is corret
int validInputFile(FILE* input_file, char input[]){
  char c = ' ';
  int  i = 0;
  
  while(( c = fgetc(input_file)) != EOF){   
    //this is ignonring various whitespace
    if(c == 32 || c == 10 || c == 9){
      continue;
    }
    if(c>64 && c<91){
      c += 32;// making it to lower
    }
    //this is taking care of anythign that is not a letter
    if(!( c <  123 && c >  96)){
      fprintf(stderr, "this file does not contain right character\n");
      return 0;
    }
    //this is anyhtign that is a letter but not atcg
    if (c != 'a' &&  c!= 't' &&  c!= 'g' &&  c!= 'c'){
      fprintf(stderr, "this file is invalid: containts characters other than actg \n");
      return 0;
    }
    //if it passes the letter dn case test it will be put in the array   
    else{
      input[i] = c;
      i++;
     
    }
  }
  return 2;
  
}
// this will check if each queery entered is correct
int qValid(char query[]){
  // this checck to make sure query deos not surpass the length of the file
  if((int) strlen(query) > 15000){
    fprintf(stderr, "this queery is invalid because too long\n");
    return 1;
  }
  for(int i = 0; i < (int) strlen(query); i++){
    char look = query[i];
    //making the letter lower case
    if(look > 64 && look < 91){
      look += 32;
    }
    // making sure the letter are only actg
    if(look != 'c' &&  look != 'a' && look != 't' &&  look != 'g'){
      fprintf(stderr,"queery is invalid because not actg\n");
      return 1;
    }
  }
    return 2; 
}


//this checks the pattern if it matches
int pattern_match(const char t[], int tlen, const char p[], int plen, int start_at){
   int match;
   int place = 0; // this is to keep track in occ array
   int occ[15000]; // array of occurences for a particualr match
   for(int i = 0; i < 15000; i++){
     occ[i] = -2;
   }
   //doing the actual comparing
   for (int i = start_at; i <= tlen- plen; i++) {
    match = 1;
    int j;
    for (j = 0; j < plen; j++){
      if (t[i + j] != p[j]){
	match = 0;
	break;
	 
      }
    }
    //if a match then putting in in the occ array
    if (match == 1){ 
      occ[place] = i;
      place++;
    }
   }
   //print out the patter
   for(int i = 0; i < (int) strlen(p); i++){
     fprintf(stdout, "%c", p[i]);
   }
   fprintf(stdout, " ");
   //if place is 0 that means that no matches were founf
   if(place == 0){
     fprintf(stdout, "Not found \n");
   }
   //prints the occ array
   for(int i = 0; i < place; i++){
     fprintf(stdout, "%d ", occ[i]); 
   }
   fprintf(stdout, "\n");
    
   //returning an impossible index
   return -1;
}


