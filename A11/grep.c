 /*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 23/11/24
 * Description: Implements simplified version 
 * of the bash command 'grep'. We search through 
 * a list of files looking for and tallying up 
 * a given keyword. 
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
	int thread_id;
	int starting_file;
	int ending_file;
	char **files;
	char *keyword;
	int *find_count;
	pthread_mutex_t *mutex;
}
Thargs;

void *file_search(void *arg){
	Thargs *args = (Thargs *)arg;
	char line[1000];
	int count = 0;
	for (int i = args->starting_file; i < args->ending_file; i++){
		FILE *file = fopen(args->files[i], "r");
		if (!file) {
			perror(args->files[i]);
			continue;
		}

		while (fgets(line, 1000, file)){
			if (strstr(line, args->keyword)){
				pthread_mutex_lock(args->mutex);
				printf("%d. %s:%s", args->thread_id, args->files[i], line);
				pthread_mutex_unlock(args->mutex);
				count++;
			}
		}
		fclose(file);
	}
	pthread_mutex_lock(args->mutex);
	args->find_count[args->thread_id] = count;
	pthread_mutex_unlock(args->mutex);
	return NULL;
}


int main(int arg_count, char *arg_list[]) {
	if (arg_count < 4 || atoi(arg_list[1]) <= 0) {
        	fprintf(stderr,"Not enough arguments passed\n");
        	return EXIT_FAILURE;
    }
	int num_threads = atoi(arg_list[1]);
	char **files = &arg_list[3];
	char *keyword = arg_list[2];
	int num_files = arg_count - 3;
	pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    	Thargs *args = malloc(num_threads * sizeof(Thargs));
    	int *find_count = malloc(num_threads * sizeof(int));
    	pthread_mutex_t mutex;
	
	pthread_mutex_init(&mutex, NULL);
	memset(find_count, 0, num_threads * sizeof(int));
	printf("Searching %d files for keyword: %s\n", num_files, keyword);
	
	int files_per_thread = num_files / num_threads;
	int files_left = num_files % num_threads;
	int starting_file = 0;

	for (int i = 0; i<num_threads; i++){
		int ending_file = starting_file + files_per_thread + (i<files_left ? 1:0); 
		
		args[i].thread_id = i;
        	args[i].starting_file = starting_file;
        	args[i].ending_file = ending_file;
       		args[i].files = files;
        	args[i].keyword = keyword;
        	args[i].find_count = find_count;
        	args[i].mutex = &mutex;

		pthread_create(&threads[i], NULL, file_search , &args[i]);
		starting_file = ending_file;
	}
	for (int i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}
	int final_count = 0;
	for (int i = 0; i < num_threads; i++){
		printf("Thread %d found %d lines containing keyword- %s\n", i, find_count[i], keyword);
		final_count += find_count[i];
	}
	
	free(threads);
	free(args);
	free(find_count);
	pthread_mutex_destroy(&mutex);

	
  return 0;
}
