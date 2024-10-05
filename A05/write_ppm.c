/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 2/10/24
 * Description: Opens a file to write into 
 * and prints out the header of the feep-raw file. 
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
	FILE* file = fopen(filename, "wb");
	if (!file) {
		printf("Error opening file for writing");
		return;
	}
fprintf(file, "P6\n");
fprintf(file, "# Created by GIMP version 2.10.24 PNM plug-in\n");
fprintf(file, "%d %d\n", w, h);
fprintf(file, "255\n");

size_t written = fwrite(pixels, sizeof(struct ppm_pixel), w * h, file);
if (written != w * h){
printf("\nError writing in pixel data\n");
	fclose(file);
	return;
}

fclose(file);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
