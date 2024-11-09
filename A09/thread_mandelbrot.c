/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 09/11/24
 * Description: Implementing a multi-threaded 
 * version of a program that outputs 
 * PPM image of the mandelbrot set with the 
 * fixed image size and locations.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <sys/time.h>

struct ThreadData {
    int start_row;
    int end_row;
    int start_col;
    int end_col;
    int maxIterations;
    float xmin, xmax, ymin, ymax;
    int size;
    struct ppm_pixel *pixels;
    struct ppm_pixel *palette;
    pthread_t thread_id;
};

void* mandelbrot_thread(void* arg){
	struct ThreadData *thread_data0 = (struct ThreadData*)arg;
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
			if (iter < thread_data0->maxIterations){
				thread_data0->pixels[row * thread_data0->size + col] = thread_data0->palette[iter];
			} else{
				thread_data0->pixels[row * thread_data0->size + col] = (struct ppm_pixel){0,0,0};
			}
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
  // generate pallet
  // compute image
  
  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (!pixels){
        perror("Error allocating image memory\n");
        return -1;
  }
  struct ppm_pixel* palette = malloc(maxIterations * sizeof(struct ppm_pixel));
  if (!palette){
        perror("Error allocating palette memory\n");
        free(pixels);
        return -1;
  }

  srand(time(0));
  for (int i = 0; i < maxIterations; i++){
        palette[i].red = rand() % 255;
        palette[i].green = rand() % 255;
        palette[i].blue = rand() % 255;
  }
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  
  struct ThreadData *thread_data = malloc(numProcesses * sizeof(struct ThreadData));
  pthread_t *threads = malloc(numProcesses * sizeof(pthread_t));
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
          thread_data[i].palette = palette;

	  pthread_create(&threads[i], NULL, mandelbrot_thread, &thread_data[i]);
	  thread_data[i].thread_id = threads[i];
  }
  for (int i = 0; i < numProcesses; i++){
	  pthread_join(threads[i], NULL);
  }	



  gettimeofday(&tend, NULL);
  double time_done = (tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6);
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, time_done);
  // compute image
  char filename[64];
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  printf("Writing file:  %s\n", filename);
  write_ppm(filename, pixels, size, size);
  free(pixels);
  free(palette);
  free(thread_data);
  free(threads);
  return 0;
}



