/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 10/10/24
 * Description: Reads in our binary 
 * file into a flat (2d) array of ppm_pixel comprising
 * of the dimensions given by the third line of
 * the file.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"
// Choose *one* to implement (do not remove the other one!)
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h){
        FILE* file = fopen(filename, "rb");
        if (!file) {
                printf("Error opening file");
                return NULL;
        }
        char line[256];
        fgets(line, 3, file);
        //printf("\nline 1: %s", line);
        fgets(line, 256, file);
        //printf("\nline 2:%s", line);
        fgets(line, 256, file);
        //printf("\nline 2:%s", line);
        fgets(line, 256, file);
        //printf("\nline 3:%s", line);
        sscanf(line, "%d %d", w, h);
        fgets(line, 256, file);
        int value;
        sscanf(line, "%d", &value);
        struct ppm_pixel* pic = malloc((*w) * (*h) * sizeof(struct ppm_pixel));
        if (!pic){
                fclose(file);
                return NULL;
        }
        fread(pic, sizeof(struct ppm_pixel), (*w) * (*h), file);
        fclose(file);
        return pic;
}
struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
        return NULL;
}

