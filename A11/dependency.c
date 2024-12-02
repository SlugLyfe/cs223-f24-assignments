/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 25/11/24
 * Description: Program that uses N threads to 
 * build a binary search tree of a given set of 
 * files. Can list the processed files in 
 * alphabetical order and display the dependencies 
 * of a given file by giving the filename.   
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "tree.h"

typedef struct {
	char filename[64];
	char deps[64][64];
	int deps_count;
} Fdata;

typedef struct {
        int thread_id;
        int starting_count;
        int ending_count;
        char **files;
        pthread_mutex_t *mutex;
	struct tree_node **root;
}
Thargs;

Fdata file_data_array[1000];
int file_data_count = 0;

void parse_files(char *filename, Fdata *file_data){
	memset(file_data, 0, sizeof(Fdata));
        FILE *file = fopen(filename, "r");
        if (!file){
                perror(filename);
                file_data->deps_count = 0;
                return;
        }
        strcpy(file_data->filename, filename);
        file_data->deps_count = 0;
        char line[200];
        while (fgets(line, 200, file)){
                char *include = strstr(line, "#include");
                if (include){
                        char *start = strchr(include,'"');
                        char *end = strrchr(include,'"');
                        if (start && end){
                                *end = '\0';
                                strcpy(file_data->deps[file_data->deps_count++], start + 1);
                        }
                }
        }
        fclose(file);
}

void *search_files(void *arg){
	Thargs *args = (Thargs *)arg;
	for (int i = args->starting_count; i < args->ending_count; i++){
		Fdata file_data = {0};
		parse_files(args->files[i], &file_data);
		pthread_mutex_lock(args->mutex);
		file_data_array[file_data_count++] = file_data;
		*args->root = insert(file_data.filename, *args->root);
		pthread_mutex_unlock(args->mutex);
	}
	return NULL;
}

int main(int arg_count, char *arg_list[]) {
        if (arg_count < 4) {
                fprintf(stderr,"Not enough arguments passed\n");
                return EXIT_FAILURE;
    }
        int num_threads = atoi(arg_list[1]);
        char **files = &arg_list[2];
        int num_files = arg_count - 2;

	if (num_files == 0){
		fprintf(stderr, "No real files provided.\n");
		return EXIT_FAILURE;
	}

        pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
        Thargs *args = malloc(num_threads * sizeof(Thargs));
	struct tree_node *root = NULL;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
	
	if(num_files == 0){
		fprintf(stderr, "No files provided.\n");
		return EXIT_FAILURE;	
	}
	if(num_threads < 1){
                fprintf(stderr, "num_threads must be positive.\n");  
                return EXIT_FAILURE;
        }

        int files_per_thread = num_files / num_threads;
        int files_left = num_files % num_threads;
        int starting_count = 0;

        for (int i = 0; i<num_threads; i++){
                int ending_count = starting_count + files_per_thread + (i<files_left ? 1:0);

                args[i].thread_id = i;
                args[i].starting_count = starting_count;
                args[i].ending_count = ending_count;
                args[i].files = files;
                args[i].root = &root;
                args[i].mutex = &mutex;

                pthread_create(&threads[i], NULL, search_files, &args[i]);
                starting_count = ending_count;
        }
        for (int i = 0; i < num_threads; i++){
                pthread_join(threads[i], NULL);
		printf("Thread %d finished executing.\n", i);
        }

        char user_input[200];
	while (fgets(user_input, 200, stdin)){
		user_input[strcspn(user_input, "\n")] = '\0';

		if (strcmp(user_input, "list") == 0){
			printSorted(root);
		}
		else if (strcmp(user_input, "quit") == 0){
			break;
		}
		else{
			struct tree_node *node = find(user_input, root);
			if (node) {
				Fdata *file_data = NULL;
				for (int i = 0; i < file_data_count; i++) { 
                   			 if (strcmp(file_data_array[i].filename, node->data.name) == 0) {
                        			file_data = &file_data_array[i];
                        			break;
                    }
                }
				if (file_data){
					printf("%s has the following dependencies:\n", file_data->filename);
					for (int i = 0; i < file_data->deps_count; i++){
						printf(" %s\n", file_data->deps[i]);
				}
			} 
			else {
				printf("%s not found\n", user_input);
			}
		} 
			 else{
                                printf("%s not found\n", user_input);
                        }

		}
	}
	
	clear(root);
        free(threads);
        free(args);
        pthread_mutex_destroy(&mutex);

  return 0;
}

