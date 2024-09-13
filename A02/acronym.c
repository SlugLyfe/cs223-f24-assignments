/***************************************************
 * acronym.c
 * Author: Kabir Alvaro Hinduja-Obregon
 * Creates an Acronym based on the (begining) capital letters of the words
 *  in the phrase passed to us.
 */

#include <stdio.h>
#include <string.h>

int main() {
	char phrase[1024];
	char acronym[512];//acronym string to store the first Capital Letters
	int count = 0;

	printf("Enter a phrase: ");
	scanf("%[^\n]%*c", phrase);

	for (int i = 0;i < strlen(phrase);i++){
		int character = phrase[i];
		if (character > 64 && character < 91){//capital letter range
			acronym[count] = phrase[i];//populating acronym string
			count+=1;		
		}
	}
	
	acronym[count] = '\0'; //adding null character to the end of acronym 
	printf("Your acronym is %s", acronym);
	return 0;
}
