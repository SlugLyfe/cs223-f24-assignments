/***************************************************
 * mathgame.c
 * Author: Kabir Alvaro Hinduja-Obregon
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("Welcome to Math Game!\n");
	int numQs=0;//num of Qs user wants
	printf("How many rounds do you want to play?: ");
	scanf(" %d",&numQs);
	int ansTotal =0; //total of correct answers

	for (int i = 0; i<numQs; i++) {
		int num1=rand()%(9)+1;
		int num2=rand()%(9)+1;
		int ans=num1+num2;//the answer to the arithmetic
		int userAns=0; //users answer
		printf("\n%d + %d = ? " , num1,num2);
		scanf("%d", &userAns);
		if (userAns==ans){
			ansTotal+=1;
			printf("Correct!\n");}
		else {printf("Incorrect :(\n");
		}
	}

	printf ("Completed! You answered %d / %d correctly.\n",ansTotal, numQs);




	return 0;
}



