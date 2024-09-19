/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon
 * Date: 18/09/24
 * Description: in this code we turn regular speech into
 * nice and slow ent speech by adding a specified pause as (.) between
 * the users entered text.  
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	int pauseLength=0;
	printf("Pause length: ");
	scanf("%d", &pauseLength);
	if (pauseLength < 0){
		printf("negative pause length can not be handled");
		return 1;
	}

	while (getchar() != '\n');

	char text[32];
	printf("Text: ");
	scanf("%[^\n]",text);

	char *text_slow;
	int text_length = strlen(text);
	text_slow = malloc((sizeof(char)*text_length*(pauseLength+1)+1));
	if (text_slow == NULL){
		printf("Error: malloc failed!\n");
		exit(1);
	}

	int count = 0;
	for (int i = 0; i<(text_length); i++){
		text_slow[count] = text[i];
		for (int j = 1; j < (pauseLength+1); j++){
			text_slow[count+j] = '.';
		} 
		count = count+(pauseLength+1);
	}
	text_slow[count] = '\0';
	printf("%s",text_slow);


	free(text_slow);
	text_slow = NULL;

	return 0;
}
