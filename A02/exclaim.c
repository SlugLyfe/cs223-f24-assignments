/***************************************************
 * exclaim.c
 * Author: Kabir Alvaro Hinduja-Obregon
 * Converts non-capitalized letters from a word the user inputs into 'exclamations'
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(){
	srand(time(NULL));  
	char word[32];
	char explicit[4]= {'@','!','#','*'};

	printf("Enter a Word: ");
	scanf("%s", word);

	//for loop for i to search through different characters in word
	for (int i = 0;i<strlen(word);i++){ 
		int character = word[i];
		if (character > 96 && character < 123){
			word[i] = explicit[rand()%4];}}

	printf("OMG! %s",word);



	return 0;}
