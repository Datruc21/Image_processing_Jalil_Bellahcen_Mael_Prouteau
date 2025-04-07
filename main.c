#include <stdio.h>
#include "bmp8.h"
#include <stdlib.h>

int main(void) {
    t_bmp8 * new_image = bmp8_loadImage("../lena_gray.bmp");

    float** filter = (float **)malloc(3 * sizeof(float *));
    // Allouer de la mémoire pour chaque ligne de la matrice
    for (int i = 0; i < 3; i++) {
        filter[i] = (float *)malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            filter[i][j] = -1;
        }
    }
    filter[1][1] = -8; // creation of the filter
    bmp8_printInfo(new_image);
    bmp8_applyFilter(new_image,filter,3);
    bmp8_saveImage("../img.bmp",new_image);
    bmp8_free(new_image);
    return 0;
}