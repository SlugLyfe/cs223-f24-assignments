/***************************************************
 * songs.c
 * Author: Kabir Alvaro Hinduja-Obregon
 * Creates and prints a list of songs with a title, artist, duration, and
 *  danceability, any four (atributes) of which can be chosen and edited
 *   by our user, for any of the three songs in our list!
 */

#include <stdio.h>
#include <string.h>

struct songType{
	char title[32];
	char artist[32];
	int duration[2];
	float danceability;
	int songID;
};

void printSongs(struct songType songList[]){
	int length = 3;
	for (int i = 0; i < length; i++){
		if (songList[i].duration[1] < 10){
			printf("\n%d) %-20s artist: %-20s duration: %d:0%d   danceability: %.02f\n",
					songList[i].songID, songList[i].title, songList[i].artist,
					songList[i].duration[0], songList[i].duration[1],
					songList[i].danceability);
		}
		else {
			printf("\n%d) %-20s artist: %-20s duration: %d:%d   danceability: %.02f\n",
					songList[i].songID, songList[i].title, songList[i].artist,
					songList[i].duration[0], songList[i].duration[1],
					songList[i].danceability);
		}

	}

}

int main() {
	struct songType listOfSongs[3];
	struct songType song1, song2, song3;

	strcpy(song1.title, "Hello");
	strcpy(song1.artist, "Adele");
	song1.duration[0] = 3;
	song1.duration[1] = 30;
	song1.danceability = 0.5;
	song1.songID = 0;

	strcpy(song2.title, "Baby");
	strcpy(song2.artist, "Justin Beiber");
	song2.duration[0] = 4;
	song2.duration[1] = 0;
	song2.danceability = 0.9;
	song2.songID = 1;

	strcpy(song3.title, "Pink Pony Club");
	strcpy(song3.artist, "Chappele Roan");
	song3.duration[0] = 2;
	song3.duration[1] = 45;
	song3.danceability = 0.8;
	song3.songID = 2;

	listOfSongs[0] = song1;
	listOfSongs[1] = song2;
	listOfSongs[2] = song3;

	printf("Welcome to Kabir's Song List\n");
	printSongs(listOfSongs);

	int userSongID;
	printf("Enter a song ID to edit [0,1,2]: ");
	scanf("%d", &userSongID);
	if (userSongID != 0 && userSongID != 1 && userSongID != 2){
		printf("That is an Invalid ID number ");
		return 0;
	}

	char attribute[13];
	printf("Which attribute do you wish to edit? [artist, title, duration,"
			" danceability] (note that your answer should be all"
			"lower case.)\n");
	scanf("%s", attribute);

	if (strcmp(attribute, "artist") == 0){
		char userArtist[32];
		printf("Enter an artist: ");
		scanf(" %[^\n]%*c", userArtist);
		strcpy(listOfSongs[userSongID].artist, userArtist);
		printSongs(listOfSongs);
	}

	else if (strcmp(attribute, "title") == 0){
		char userTitle[32];
		printf("Enter a title: ");
		scanf(" %[^\n]%*c", userTitle);
		strcpy(listOfSongs[userSongID].title, userTitle);
		printSongs(listOfSongs);
	}

	else if (strcmp(attribute, "duration") == 0){
		int userDurationMinutes;
		int userDurationSeconds;
		printf("Enter a duration (minutes): ");
		scanf("%d", &userDurationMinutes);
		printf("\nEnter a duartion (seconds): ");
		scanf("%d", &userDurationSeconds);
		listOfSongs[userSongID].duration[0] = userDurationMinutes;
		listOfSongs[userSongID].duration[1] = userDurationSeconds;
		printSongs(listOfSongs);
	}

	else if (strcmp(attribute, "danceability") == 0){
		float userDanceability;
		printf("Enter danceability: ");
		scanf("%f", &userDanceability);
		listOfSongs[userSongID].danceability = userDanceability;
		printSongs(listOfSongs);
	}	

	else {
		printf("Invalid attribute entry");
		return 0;
	}








	return 0;
}
