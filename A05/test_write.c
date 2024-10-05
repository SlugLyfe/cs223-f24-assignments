/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 2/10/24
 * Description: Writes into the new file created 
 * with the contents of the binary file read by 
 * read_ppm. 
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  if (pixels == NULL) {
  printf("Unable to read file :(");
  return 0;
  }

  // test writing the file to test.ppm, reload it, and print the contents
  write_ppm("test.ppm", pixels, w, h);

  struct ppm_pixel* pixels_test = read_ppm("test.ppm", &w, &h);
  if (pixels_test == NULL) {
   printf("Unable to read the written file");
   return 0;
  }

  printf("Testing file 'feep-raw.ppm': %d %d\n", w, h);
  for (int i = 0; i < h; i++){
  	for (int j = 0; j < w; j++){
		struct ppm_pixel p = pixels_test[i * w + j];
		printf("(%d, %d, %d) ", p.red, p.green, p.blue);
	}
	printf("\n");
  }


  free(pixels);
  free(pixels_test);
  return 0;
}
