#include <stdio.h>
#include "bmp8.h"
#include <stdlib.h>

int main(void) {
    t_bmp8 * new_image = bmp8_loadImage("../lena_gray.bmp");
    float gaussian_blur_kernel[3][3] = {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };
    float outline[3][3] = {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}
    };

    float emboss[3][3] = {
        {-2,-1,0},
        {-1,1,1},
        {0,1,2}
    };

    float** filter = (float **)malloc(3 * sizeof(float *));
    // Allouer de la mémoire pour chaque ligne de la matrice
    for (int i = 0; i < 3; i++) {
        filter[i] = (float *)malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            filter[i][j] = outline[i][j];
        }
    }
    bmp8_printInfo(new_image);
    bmp8_applyFilter(new_image,filter,3);
    bmp8_saveImage("../img.bmp",new_image);
    bmp8_free(new_image);
    return 0;
}