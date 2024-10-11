/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon
 * Date: 10/10/24
 * Description: Reads in a PPM file and writes 
 * a new file with a secret message embeded in
 * the mostly similar PPM file.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
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
  int max_characters = (((width * height * 3) / 8)-1);
  printf("Max number of characters in the image: %d\n", max_characters);

  char secret_message[100];
  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", secret_message);

  int message_length = strlen(secret_message);
  if (message_length > max_characters){
  printf("Error: Message is too long to fit in the image.\n");
  free(image);
  return 0;
  }
	
  for (int bit_i = 0; bit_i < message_length * 8; bit_i++){
  	unsigned char current_bit = (secret_message[bit_i/8] >> (7 - (bit_i % 8))) & 1;

	if ((bit_i % 3) == 0){
		image[bit_i / 3].red = (image[bit_i / 3].red & ~1) | current_bit;
	}
	else if ((bit_i % 3) == 1){
                image[bit_i / 3].green = (image[bit_i / 3].green & ~1) | current_bit;
	}
	else if ((bit_i % 3) == 2){
                image[bit_i / 3].blue = (image[bit_i / 3].blue & ~1) | current_bit;
        }
  }

  char output_filename[256];
  strcpy(output_filename, argv[1]);
  strcat(output_filename, "-encoded.ppm");
  write_ppm(output_filename, image, width, height);

  printf("Writing file %s\n", output_filename);
  free(image);
  return 0;
}

