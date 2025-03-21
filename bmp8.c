#include "bmp8.h"
#include <stdlib.h>

#include <stdio.h>

t_bmp8 * bmp8_loadImage(const char* filename){
    FILE *f = fopen(filename, "rb");
    unsigned char header[54];
    fread(header, 1, 54, f);
    unsigned int width = *(unsigned int *)&header[18];
    unsigned int height = *(unsigned int *)&header[22];
    unsigned int colorDepth = *(unsigned int *)&header[28];
    unsigned int dataSize = *(unsigned int *)&header[34];


    t_bmp8 * new_image = (t_bmp8 *) malloc(sizeof(t_bmp8));
    if (new_image == NULL ) return NULL;

    return NULL;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img){


}

void bmp8_free(t_bmp8 * img){


}

void bmp8_printInfo(t_bmp8 * img){


}