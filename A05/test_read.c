/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 1/10/24
 * Description: Calls the read_ppm function and
 * prints out the the values stored from the 
 * binary file in the form of the 4x4 matrix 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  // todo: print out the grid of pixels
  if (pixels == NULL){
  printf("Unable to read PPM file :(\n ");
		  return 0;
  }

  printf("Testing file feep-raw.ppm: %d %d\n", w, h);
  for (int i = 0; i < h; i++){
  	for (int j = 0; j < w; j++){
		struct ppm_pixel p = pixels[i * w + j];
		printf("(%d, %d, %d)  ", p.red, p.green, p.blue);
	}
	printf("\n");
  }


  free(pixels);
  return 0;
}

