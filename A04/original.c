/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon
 * Date: 22/09/24
 * Description: 
 * ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	typedef struct song{
        char title[128];
        char artist[128];
        int duration;
        float danceability;
        float energy;
        float tempo;
        float valence;
}song;

typedef struct node {
	struct song thisSong;
	struct node * next;
} node_t;

void addToList(struct node **head, struct song newSong){
struct node *newNode = (struct node*)malloc(sizeof(struct node));
	if (newNode == NULL){
		printf("Memory error");
		exit (1);
	}
	newNode->thisSong = newSong;
	newNode->next = *head;
	*head = newNode;
}


void printfunc(struct node *songList, int numSongs) {
	int length = numSongs;
	struct node *currentNode = songList;
	song currentSong = currentNode->thisSong;
	for (int i = 0; i < length; i++){
		if (((currentSong.duration % 60000)/1000) < 10){
			printf("\n%-4d) %-30s artist: %-30s duration: %d:0%-2d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
					(i), currentSong.title, currentSong.artist,
					(currentSong.duration/60000), ((currentSong.duration % 60000)/1000),
					currentSong.danceability,currentSong.energy,currentSong.tempo,currentSong.valence);
		}
		else {

		 printf("\n%-4d) %-30s artist: %-30s duration: %d:%-2d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
                                        (i), currentSong.title, currentSong.artist,
                                        (currentSong.duration/60000), ((currentSong.duration % 60000)/1000),
                                        currentSong.danceability,currentSong.energy,currentSong.tempo,currentSong.valence);
}
currentSong = currentNode->next->thisSong;
                        currentNode = currentNode->next;
	}
		}

/**void deleteMostDanceable(struct node** head, struct song toBeDeleted){
        struct node *current = *head;
        printf("\n %s\n\n", current->thisSong.title);
	struct node *prev = NULL;
        while (current != NULL && strcmp(current->thisSong.title, toBeDeleted.title)!=0){
                prev = current;
                current = current->next;
        }

        printf("\n %s\n\n", current->thisSong.title);
        if (current == NULL){
                printf("Song not found");
                return;
        }
	if (prev == NULL){
		*head = current->next;
		printf("PREVISNULL");
		printf("%sremoving eye of tiger - new head\n", head->thisSong.title);
	}
        else{
		printf("PREVISNOTNULL\n");
        prev->next = current->next;
        }
	
	
        //printf("\n %sugkjkjhvkjjkjk\n\n", prev->next->thisSong.title);
	printf("Deleting song: %s\n", current->thisSong.title);
        free(current);
}*/

void deleteMostDanceable(node_t **head) {
    if (*head == NULL) {
        printf("List is empty. Nothing to delete.\n");
        return;
    }

    node_t *current = *head;
    node_t *prev = NULL;
    node_t *mostDanceableNode = current;
    node_t *mostDanceablePrev = NULL;

    // Find the most danceable song
    while (current != NULL) {
        if (current->thisSong.danceability > mostDanceableNode->thisSong.danceability) {
            mostDanceableNode = current;
            mostDanceablePrev = prev;
        }
        prev = current;
        current = current->next;
    }

    // If the most danceable song is the head
    if (mostDanceablePrev == NULL) {
        *head = mostDanceableNode->next; // Update head
    } else {
        mostDanceablePrev->next = mostDanceableNode->next; // Bypass the most danceable node
    }

    printf("Deleting song: %s\n", mostDanceableNode->thisSong.title);
    free(mostDanceableNode); // Free the memory of the deleted node
}



void printMostDanceable(struct node *songList){
	float danceCounter = 0.0;
	struct song currentSong ;
	for (struct node* n = songList; n->next!= NULL; n = n->next){
			printf("Title: %-30s Artist: %-30s Danceability: %.2f\n", n->thisSong.title, n->thisSong.artist, 
					n->thisSong.danceability);
			if (n->thisSong.danceability > danceCounter){
			currentSong = n->thisSong;
			danceCounter = n->thisSong.danceability;	
			
	}
	}
		
	printf("The Most Danceable Song\n");
	if (((currentSong.duration % 60000)/1000) < 10){
                        printf("\n %-30s artist: %-30s duration: %d:0%-2d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
                                        currentSong.title, currentSong.artist,
                                        (currentSong.duration/60000), ((currentSong.duration % 60000)/1000),
                                        currentSong.danceability,currentSong.energy,currentSong.tempo,currentSong.valence);
                }
                else {

                 printf("\n %-30s artist: %-30s duration: %d:%-2d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
                                         currentSong.title, currentSong.artist,
                                        (currentSong.duration/60000), ((currentSong.duration % 60000)/1000),
                                        currentSong.danceability,currentSong.energy,currentSong.tempo,currentSong.valence);
}

//	deleteMostDanceable(&songList, currentSong);
	deleteMostDanceable(&songList);
}


int main() {
	FILE *infile;
	char row[100];
	char *token;

	infile = fopen("songlist.csv", "r");
	if(infile == NULL){
		printf("Error: unable to open file %s\n", "songList.cvs");
		exit(1);
	}
	
	node_t * head = NULL;
	head = (node_t *) malloc(sizeof(node_t));
	if (head == NULL){
		return 1;
		}
	head->next = NULL;

	int count = 0;
	int firstEntryInt = -1;

	while (fgets(row, 100, infile)!= NULL){
	firstEntryInt += 1;
	}
	
	if (firstEntryInt < 0){
		free(head);
		printf("No Valid Entries.\n");
		fclose(infile);
		exit(1);
		}

	fseek(infile, 0, SEEK_SET);
	
	fgets(row, 100, infile);//reads first line

	int count1 = 0;
	
	while (count1 < firstEntryInt){
		fgets(row, 100, infile);
		token = strtok(row, ",");
		struct song newSong;
		int count2=0;
		while(token != NULL){
			int intSub = 0;
                	float floatSub = 0.0;

			if (count2 == 0){
				strcpy(newSong.title, token);
			}
			if (count2 == 1){
				strcpy(newSong.artist, token);
			}

			if (count2 == 2){
				intSub = atoi(token);
				newSong.duration = intSub;
			}
			if (count2 == 3){
				floatSub = atof(token);
				newSong.danceability = floatSub;
			}
			if (count2 == 4){
				floatSub = atof(token);
				newSong.energy = floatSub;
                        }
			if (count2 == 5){
				floatSub = atof(token);
				newSong.tempo = floatSub;
                        }
			if (count2 == 6){
				floatSub = atof(token);
				newSong.valence = floatSub;
                        }
			count2 += 1;
			token = strtok(NULL, ",");

		} 
		count1 += 1;
		addToList(&head, newSong);
	}
	printfunc(head, firstEntryInt);

	printf("\nDataset contains %d songs\n",firstEntryInt);
	int placeholder = 0;
	while (placeholder == 0){
		char userInput;
		printf("Press 'd' to show the most danceable song (any other key to quit): ");
		scanf(" %c", &userInput);
		if (userInput == 'd'){
			if (firstEntryInt ==0){
				printf("\nDataset contains 0 songs\n");
			}
			else{
			printMostDanceable(head);
			firstEntryInt = firstEntryInt-1;
	//		printfunc(head, firstEntryInt);
			printf("\nDataset contains %d songs\n", firstEntryInt);
			}
			
		}	
		else{
		       	placeholder+=1;
		}
	}


	fclose(infile);
	for (node_t * n = head; n !=NULL;){
		node_t *x = n;
		n = n->next;
		free(x);
		}
  return 0;
}



