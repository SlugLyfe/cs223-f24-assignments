/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 10/10/24
 * Description: Reads in a PPM file and outputs 
 * the message stored in the least significant 
 * bit of each color. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  int width;
  int height;
  struct ppm_pixel* image = read_ppm(argv[1], &width, &height);

  if (!image){
  	printf("Error Reading File\n");
	return 0;
  }

  printf("Reading %s with width %d and height %d\n", argv[1], width, height);
  int max_characters = (width * height * 3) / 8;
  printf("Max number of characters in the image: %d\n", max_characters);
  unsigned char* secret_message = malloc((max_characters + 1) * sizeof(unsigned char));
  for (int x = 0; x <= max_characters; x++){
  	secret_message[x] = 0;
  }

  int bit_i = 0;

  for (int i = 0; i < width * height; i++){
  	for (int j = 0; j < 3; j++){
		if (bit_i >= max_characters * 8){
		break;
		}
		secret_message[bit_i / 8] <<= 1;
		if (j == 0) {
		secret_message[bit_i / 8] |= (image[i].red & 1);
		}
		else if (j == 1) {
                secret_message[bit_i / 8] |= (image[i].green & 1);
                }
		else if (j == 2) {
                secret_message[bit_i / 8] |= (image[i].blue & 1);
                }

		bit_i++;
	}
	if (bit_i >= max_characters * 8){
	break;
	}
  }
	secret_message[bit_i / 8] = '\0';
	printf("Decoded message: %s\n",secret_message);
	free(secret_message);
	free(image);

  return 0;
}

