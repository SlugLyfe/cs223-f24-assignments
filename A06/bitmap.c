/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 10/10/24
 * Description: Reads in a single 64-bit unsigned 
 * integer and outputs it as an 8x8 1bpp sprite
 * where & is black and space is white. 
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  for (int row = 0; row < 8; row++){
  	for (int collumn = 0; collumn < 8; collumn++){
	int bit_pos = 63 - (row * 8 + collumn);
	unsigned long mask = 0x1ul << bit_pos;
	if (img & mask){
	printf("@");
	}
	else {
	printf(" ");
	}
	}
	printf("\n");
  }
  return 0;
}
