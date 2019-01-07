#include "dnasearch.h"
int main(){
  
 int n =  test_validInputFile();
 int p =  test_qValid();
 int m = test_pattern_match();
 // int o =  test_printFunc();
 if((m +  p + n) == 3){
   fprintf(stdout, "This passed the tests\n");
 }
  
 return 0;
}

int  test_validInputFile(){
  char array[15000];
  FILE* input = fopen("fDNA.txt", "r");
  assert(validInputFile(input, array)==2);
  for(int i = 0; i < 15000; i++){
    array[i] = 0;
  }
  
  FILE* empty = fopen("empty.txt", "r");
  assert(validInputFile(empty, array)==0);
  for(int i = 0; i < 15000; i++){
    array[i] = 0;
  }
  FILE* sortaOK = fopen("sortaOK.txt", "r");
  assert(validInputFile(sortaOK, array) == 0);
  return 1;	
}

int  test_qValid(){
  char q[15001];
  for(int i = 0; i < 15001; i++){
    q[i] = 'a';
  }
  assert(qValid(q)==1);
  
  
  char t[5] = "ATag";
  assert(qValid(t) == 2);
  char r[2] = " ";
  assert(qValid(r) != 2);
  char y[2] = "0";
  assert(qValid(y) != 2);
  return 1;
}

int test_pattern_match(){
  char t[30] = "ataggggccct";
  int len = (int) strlen(t);
  char p[4] = "ata";
  int plen = (int) strlen(t);
  assert(pattern_match(t,len, p, plen, 0) != -1);
  char y[4] = "agc";
  assert(pattern_match(t, len, y, plen, 0) == -1);
  return 1;

}
