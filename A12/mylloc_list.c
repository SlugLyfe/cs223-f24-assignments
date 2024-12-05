/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 5/12/24
 * Description: Creating and implementing my 
 * own malloc and free based on a free list 
 * and a first-fit strategy, reusing memory 
 * that is freed . 
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
	int size;      
	int used;      
	struct chunk *next;
};

struct chunk *flist = NULL; 

void *malloc(size_t size) {
	if (size == 0) {
		return NULL;
	}

	struct chunk *next = flist;
	struct chunk *prev = NULL;

	while (next != NULL) {
		if (next->size >= size) {
			if (prev != NULL){
				prev->next = next->next;
			}
			else{
				flist = next->next;
			}
			next->used = size;
			return (void *)(next + 1);
		}
		prev = next;
		next = next->next;
	}

	void *memory = sbrk(size + sizeof(struct chunk));
	if (memory == (void *)-1) {
		return NULL;
	}

	struct chunk *new_chunk = (struct chunk *)memory;
	new_chunk->size = size;
	new_chunk->used = size;
	new_chunk->next = NULL;
	return (void *)(new_chunk + 1);
}

void free(void *memory) {
	if (memory == NULL) {
		return;
	}

	struct chunk *f_chunk = (struct chunk *)((char *)memory - sizeof(struct chunk));
	//f_chunk->used = 0;
	f_chunk->next = flist;
	flist = f_chunk;
}

