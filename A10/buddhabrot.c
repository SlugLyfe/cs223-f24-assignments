 /*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 13/11/24
 * Description: Computes a program that outputs
 *  a PPM image of the Buddhabrot using multiple 
 *  threads. The color of each pixel represents 
 *  the probability of a point visitng it as the 
 *  point escamples.
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <math.h>
#include <sys/time.h>


struct ThreadData {
    int start_row;
    int end_row;
    int start_col;
    int end_col;
    int maxIterations;
    float xmin, xmax, ymin, ymax;
    int size;
    int **visCounts;
    int *max_count;
    pthread_t thread_id;
    struct ppm_pixel *pixels;
    pthread_barrier_t *barrier;
    pthread_mutex_t *count_mutex;
};
void* buddhabrot_thread(void* arg){
        struct ThreadData *thread_data0 = (struct ThreadData*)arg;
	thread_data0->thread_id = pthread_self();
        printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n", thread_data0->thread_id, thread_data0->start_col, thread_data0->end_col, thread_data0->start_row, thread_data0->end_row);
        for (int row = thread_data0->start_row; row < thread_data0-> end_row; row++){
                for (int col = thread_data0-> start_col; col<thread_data0-> end_col; col++){
                        float x0 = thread_data0->xmin + col * (thread_data0->xmax - thread_data0->xmin)/thread_data0->size;
                        float y0 = thread_data0->ymin + row * (thread_data0->ymax - thread_data0->ymin)/thread_data0->size;
                        float x = 0, y = 0;
                        int iter = 0;
                        while(iter < thread_data0->maxIterations && (x * x + y * y < 4 )){
                                float xp = (x * x - y * y + x0);
                                y = (2 * x * y + y0);
                                x = xp;
                                iter++;
                        }
                        if (iter == thread_data0 -> maxIterations){
				continue;
			}
			x = 0;
			y = 0;
			while (x * x + y * y < 4){
				float xp = x * x - y * y + x0;
				y = 2 * x * y + y0;
				x = xp;
				int yrow = round(thread_data0->size * (y - thread_data0->ymin) / (thread_data0->ymax - thread_data0->ymin));
				int xcol = round(thread_data0->size * (x - thread_data0->xmin) / (thread_data0->xmax - thread_data0->xmin));
				if (yrow < 0 || yrow >= thread_data0->size || xcol < 0 || xcol >= thread_data0->size){
				       	continue;}
				pthread_mutex_lock(thread_data0->count_mutex);
				thread_data0->visCounts[yrow][xcol]++;
				if (thread_data0->visCounts[yrow][xcol] > *thread_data0->max_count){
					*thread_data0->max_count = thread_data0->visCounts[yrow][xcol];
				}
				 pthread_mutex_unlock(thread_data0->count_mutex);
			}
		}
        }
	pthread_barrier_wait(thread_data0->barrier);

	float gamma = 0.681;
        float factor = 1.0 / gamma;
        for (int row = thread_data0->start_row; row < thread_data0->end_row; row++) {
        for (int col = thread_data0->start_col; col < thread_data0->end_col; col++) {
        int count = thread_data0->visCounts[row][col];
	float value;
	if (count > 0){
		value = log(count) / (log(*thread_data0->max_count));
		value = pow(value, factor);
	}
	else{
		value = 0.0;
	}
	thread_data0->pixels[row * thread_data0->size + col].red = value * 255;
	thread_data0->pixels[row * thread_data0->size + col].green = value * 255;
	thread_data0->pixels[row * thread_data0->size + col].blue = value * 255;
	}
	}
        printf("Thread %lu finished\n", thread_data0->thread_id);
        return NULL;
}


int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image
  
  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (!pixels){
        perror("Error allocating image memory\n");
        return -1;
  }
  int **visCounts = malloc(size * sizeof(int*));
  for (int i = 0; i < size; i++) {
        visCounts[i] = calloc(size, sizeof(int));
    }
 
  struct ThreadData *thread_data = malloc(numProcesses * sizeof(struct ThreadData));
  pthread_t *threads = malloc(numProcesses * sizeof(pthread_t));
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  int max_count = 0;
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, numProcesses);
  pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

  int rows_per_thread = size/2;
  int col_per_thread = size/2;
  for (int i = 0; i< numProcesses; i++){
          int row_start = (i/2) * rows_per_thread;
          int col_start = (i % 2) * col_per_thread;
          thread_data[i].start_row = row_start;
          thread_data[i].end_row = row_start + rows_per_thread;
          thread_data[i].start_col = col_start;
          thread_data[i].end_col = col_start + col_per_thread;
          thread_data[i].maxIterations = maxIterations;
          thread_data[i].xmin = xmin;
          thread_data[i].xmax = xmax;
          thread_data[i].ymin = ymin;
          thread_data[i].ymax = ymax;
          thread_data[i].size = size;
          thread_data[i].pixels = pixels;
	  thread_data[i].visCounts = visCounts;
	  thread_data[i].barrier = &barrier;
	  thread_data[i].count_mutex = &count_mutex;
	  thread_data[i].max_count = &max_count;
          pthread_create(&threads[i], NULL, buddhabrot_thread, &thread_data[i]);
  }

  for (int i = 0; i < numProcesses; i++){
          pthread_join(threads[i], NULL);
  }

  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&count_mutex);

  gettimeofday(&tend, NULL);
  double time_done = (tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6);
  printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, time_done);
  
  char filename[64];
  sprintf(filename, "buddhabrot-%d-%ld.ppm", size, time(0));
  printf("Writing file:  %s\n", filename);
  write_ppm(filename, pixels, size, size);
  free(pixels);
  for (int i = 0; i < size; i++) {
        free(visCounts[i]);
    }
  free(visCounts);
  free(thread_data);
  free(threads);
  return 0;
}


