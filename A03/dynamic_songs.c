/*----------------------------------------------
 * Author: Kabir 
 * Date: 18/09/24
 * Description : This program reads songs from a
 * text file into an array/list of songs whose
 * memory is dynamically stored in the heap. The 
 * Songs (Song struct) contains attributes of
 * Title, Artist, Duration, Danceability, Energy,
 * Tempo, and Valence. All of which are neatly
 * printed
 ---------------------------------------------*/
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

void printfunc(struct song songList[], int numSongs) {
	int length = numSongs;
	for (int i = 0; i < length; i++){
		if (((songList[i].duration % 60000)/1000) < 10){
			printf("\n%-4d) %-30s artist: %-30s duration: %d:0%-2d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
					i, songList[i].title, songList[i].artist,
					(songList[i].duration/60000), ((songList[i].duration % 60000)/1000),
					songList[i].danceability,songList[i].energy,songList[i].tempo,songList[i].valence);
		}
		else {
		
		 printf("\n%-4d) %-30s artist: %-30s duration: %d:%-2d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
                                        i, songList[i].title, songList[i].artist,
                                        (songList[i].duration/60000), ((songList[i].duration % 60000)/1000),
                                        songList[i].danceability,songList[i].energy,songList[i].tempo,songList[i].valence);
}
	}
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

	int count = 0;
	char firstEntry[100];
	int firstEntryInt;

	fgets(row, 100, infile);//reads first line
	token = strtok(row,",");
	strcpy(firstEntry, token);	
	firstEntryInt = atoi(firstEntry);
	if (firstEntryInt < 1){
		printf("No Valid Entries.\n");
		fclose(infile);
		exit(1);
		}
	song *songList = NULL;
		
	songList = malloc(sizeof(song) * firstEntryInt);
	fgets(row, 100, infile);//reads second line

	int count1 = 0;
	while (count1 < firstEntryInt){
		fgets(row, 100, infile);
		token = strtok(row, ",");
		
		int count2=0;
		while(token != NULL){
			int intSub = 0;
                	float floatSub = 0.0;

			if (count2 == 0){
				strcpy(songList[count1].title, token);
			}
			if (count2 == 1){
				strcpy(songList[count1].artist, token);
			}
		
			if (count2 == 2){
				intSub = atoi(token);
				songList[count1].duration = intSub;
			}
			if (count2 == 3){
				floatSub = atof(token);
				songList[count1].danceability = floatSub;
			}
			if (count2 == 4){
				floatSub = atof(token);
				songList[count1].energy = floatSub;
                        }
			if (count2 == 5){
				floatSub = atof(token);
				songList[count1].tempo = floatSub;
                        }
			if (count2 == 6){
				floatSub = atof(token);
				songList[count1].valence = floatSub;
                        }
			count2 += 1;
			token = strtok(NULL, ",");

		}
		                count1 += 1;
	}
	printfunc(songList, firstEntryInt);


	fclose(infile);
	free(songList);
  return 0;
}
