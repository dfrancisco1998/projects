#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Return value is the first offset *greater than or equal to* start_at
// where pattern occurs in text.  Returns -1 if there is no such
// occurrence.
//void printFunc(char query[], int place);
int pattern_match(const char t[], int tlen, const char p[], int plen, int start_at);

int validInputFile(FILE* input_file, char input[]);
int qValid(char query[]);
int test_pattern__match();
int test_validInputFile();
int test_qValid();
//void printOcc();
//void app(int i);
