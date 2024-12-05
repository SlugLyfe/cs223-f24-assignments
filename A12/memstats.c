/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 5/12/24
 * Description: Prints the total number of blocks 
 * that are currently free, and in use respectively. 
 * Also prints the total amount of memory allocated, 
 * (in use and free). Finally, it also prints the 
 * total amount of underutilized memory. (% of 
 * memory that has been allocated by sbrk but not 
 * being used).  
 ---------------------------------------------*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
	int size;
	int used;
	struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
	int tot_blocks = 0;
	int free_blocks = 0;
	int used_blocks = 0;
	int tot_memory_alloc = 0;
	int used_memory = 0;
	int free_memory = 0;
	int unused_memory = 0;

	struct chunk *c = freelist;
	while (c != NULL){
		tot_blocks++;
		free_blocks++;
		free_memory += c->size;
		c = c->next;
	}

	for (int i = 0; i < len; i++){
		if (buffer[i] != NULL){
			struct chunk *chunk = (struct chunk *)((char *)buffer[i] - sizeof(struct chunk));
			tot_blocks++;
			used_blocks++;
			used_memory += chunk->size;
			tot_memory_alloc += chunk->size;
			unused_memory += (chunk->size) - (chunk->used);
		}
	}
	float  underutilized_memory = (float)unused_memory/tot_memory_alloc;
	printf("Total blocks: %d   free blocks: %d   used blocks: %d\n", tot_blocks, free_blocks, used_blocks);
	printf("Total memory allocated: %d   free memory: %d   used memory: %d\n",tot_memory_alloc, free_memory, used_memory);
	printf("Underutilized Memory: %.2f\n", underutilized_memory);
}

int main ( int argc, char* argv[]) {

	printf("Starting test..\n");

	srand(100);

	double timer;
	struct timeval tstart, tend;

	gettimeofday(&tstart, NULL);

	void *buffer[BUFFER];
	for (int i = 0; i < BUFFER; i++) {
		buffer[i] = NULL;
	}

	void *init = sbrk(0);
	void *current;
	printf("The initial top of the heap is %p.\n", init);
	for (int j = 0 ; j < ROUNDS; j++) {
		printf("---------------\n%d\n" , j);

		for (int i= 0 ; i < LOOP ; i++) {
			int index = rand() % BUFFER;
			if (buffer[index] != NULL) {
				free(buffer[index]);
				buffer[index] = NULL;
				printf("Freeing index %d\n", index);
			} 
			else {
				size_t size = (size_t) randExp(8, 4000); 
				int *memory = NULL;
				memory = malloc(size);

				if (memory == NULL) {
					fprintf(stderr, "malloc failed\n");
					return(1);
				} 
				*memory = 123;
				buffer[index] = memory;
				printf("Allocating %d bytes at index %d\n", (int) size, index);
			}
		}
		extern struct chunk* flist;
		current = sbrk(0);
		int allocated = current - init;
		init = current;

		printf("The new top of the heap is %p.\n", current);
		printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
		memstats(flist, buffer, BUFFER);
	}

	for (int i = 0; i < BUFFER; i++) {
		free(buffer[i]); 
	}
	gettimeofday(&tend, NULL);
	timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
	printf("Time is %g\n", timer);

	return 0 ;
}
