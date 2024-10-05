/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 1/10/24
 * Description: This code generates a crossword
 *  puzzle by finding a common letter, if it
 *  exists, from the two words provided in the
 *  command line, and creates a crossword puzzle
 *   with the common word as the intersection 
 *   point. The empty spaces are dots. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printCrossword(char **crossword, int rows, int collums){
for(int i = 0; i < rows; i++){
for(int j = 0; j < collums; j++){
printf("%c", crossword[i][j]);
}
printf("\n");
}
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }
  
  char *word1 = argv[1];
  char *word2 = argv[2];
  int length1 = strlen(word1);
  int length2 = strlen(word2);
  int onePos = 0;
  int twoPos = 0;
  int check = 0;
  for (int i = 0; i < length1; i++){
	  for (int j = 0; j < length2; j++){
		if (word1[i] == word2[j]){
		onePos = i;
		twoPos = j;
		check = 1;
		break;
		}	
	if (check == 1){
	break;
	}				     }
  }
if (check == 0){
printf("\nNo common Letter!\n");
	return 0;
}
int rows = length1;
int collums = length2;

char **crossword = malloc(rows * sizeof(char*));
for (int m = 0; m < rows; m++){
crossword[m] = malloc(collums * sizeof(char));
memset(crossword[m], '.', collums);
}

for (int f = 0; f < length1; f++){
crossword[f][twoPos] = word1[f];
}

for (int g = 0; g < length2; g++){
crossword[onePos][g] = word2[g];
}

printCrossword(crossword, rows, collums);

for (int n = 0; n < rows; n++){
free(crossword[n]);
}

free(crossword);

  return 0;
}


