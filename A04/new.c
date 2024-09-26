/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon
 * Date: 22/09/24
 * Description: This Code reads in a lost list from
 * a csv and stores and uses this song list in the form
 * of a linked list. We can also print out the most
 * danceable song in this list, and return the
 * linked list (/song list) without the recently
 * printed most dancable song. This cycle can be repeated
 *  or exited at any moment!
 * ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char title[128];
    char artist[128];
    int duration; // in milliseconds
    float danceability;
    float energy;
    float tempo;
    float valence;
    struct Node *next;
} Node;

void addToList(Node **head, Node newSong) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory error");
        exit(1);
    }
    *newNode = newSong;
    newNode->next = *head;
    *head = newNode;
}

void printSongs(Node *head) {
    if (head == NULL) {
        printf("No songs in the list.\n");
        return;
    }

    int index = 0;
    Node *current = head;
    while (current != NULL) {
        printf("%-4d) %-30s artist: %-30s duration: %d:%02d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
               index++, current->title, current->artist,
               current->duration / 60000, (current->duration % 60000) / 1000,
               current->danceability, current->energy, current->tempo, current->valence);
        current = current->next;
    }
}

void deleteMostDanceable(Node **head) {
    if (*head == NULL) {
        printf("List is empty. Nothing to delete.\n");
        return;
    }

    Node *current = *head;
    Node *prev = NULL;
    Node *mostDanceableNode = current;
    Node *mostDanceablePrev = NULL;

    while (current != NULL) {
        if (current->danceability > mostDanceableNode->danceability) {
            mostDanceableNode = current;
            mostDanceablePrev = prev;
        }
        prev = current;
        current = current->next;
    }

    if (mostDanceablePrev == NULL) {
        *head = mostDanceableNode->next; // Update head
    } else {
        mostDanceablePrev->next = mostDanceableNode->next;
    }

    printf("Most danceable song: %-30s artist: %-30s duration: %d:%02d   danceability: %.02f  energy: %.02f  tempo: %.02f  valence %.02f\n",
           mostDanceableNode->title, mostDanceableNode->artist,
           mostDanceableNode->duration / 60000, (mostDanceableNode->duration % 60000) / 1000,
           mostDanceableNode->danceability, mostDanceableNode->energy, mostDanceableNode->tempo, mostDanceableNode->valence);

    free(mostDanceableNode); // Free memory
}

void readSongsFromFile(Node **head, const char *filename) {
    FILE *infile = fopen(filename, "r");
    if (infile == NULL) {
        printf("Error: unable to open file %s\n", filename);
        exit(1);
    }

    char row[256];
    fgets(row, sizeof(row), infile);//skipping first line

    while (fgets(row, sizeof(row), infile) != NULL) {
        Node newSong;
        char *token = strtok(row, ",");
        int count = 0;

        while (token != NULL) {
            switch (count) {
                case 0:
                    strncpy(newSong.title, token, sizeof(newSong.title) - 1);
                    newSong.title[sizeof(newSong.title) - 1] = '\0'; 
                    break;
                case 1:
                    strncpy(newSong.artist, token, sizeof(newSong.artist) - 1);
                    newSong.artist[sizeof(newSong.artist) - 1] = '\0'; 
                    break;
                case 2:
                    newSong.duration = atoi(token);
                    break;
                case 3:
                    newSong.danceability = atof(token);
                    break;
                case 4:
                    newSong.energy = atof(token);
                    break;
                case 5:
                    newSong.tempo = atof(token);
                    break;
                case 6:
                    newSong.valence = atof(token);
                    break;
            }
            count++;
            token = strtok(NULL, ",");
        }

        addToList(head, newSong);
    }

    fclose(infile);
}

void handleUserInput(Node **head) {
    char choice;
    while (1) {
        printf("\npress d to show the most dancable song, anything else to quit:\n");
        scanf(" %s", &choice); 

        if (choice == 'd') {
            deleteMostDanceable(head);
            printf("Songs remaining: ");
            int count = 0;
            Node *current = *head;
            while (current != NULL) {
                count++;
                current = current->next;
            }
            printf("%d\n", count);
	    printSongs(*head);
        } else
            break;
        }
        }


int main() {
    Node *head = NULL;

    readSongsFromFile(&head, "songlist.csv");

    // Print all songs in list
    printSongs(head);

    handleUserInput(&head);

    // Free remaining songs in the list
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}


