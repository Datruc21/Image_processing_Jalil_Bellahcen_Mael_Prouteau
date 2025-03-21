#include "bmp8.h"
#include <stdlib.h>

#include <stdio.h>

t_bmp8 * bmp8_loadImage(const char* filename){
    FILE *f = fopen(filename, "rb");
    unsigned char header[54];



    t_bmp8 * new_image = (t_bmp8 *) malloc(sizeof(t_bmp8));
    if (new_image == NULL ) return NULL;
    unsigned char *header = malloc(sizeof(unsigned char)*54);
    if (header == NULL)
        printf("Error with memory location");

    unsigned char* colorTable = (unsigned char*)malloc(sizeof(unsigned char)*1024);
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char));

    unsigned int width = *(unsigned int *)&header[18];
    unsigned int height = *(unsigned int *)&header[22];
    unsigned int colorDepth = *(unsigned int *)&header[28];
    unsigned int dataSize = *(unsigned int *)&header[34];

    //find those in the header file
    }

void bmp8_saveImage(const char * filename, t_bmp8 * img){


}

void bmp8_free(t_bmp8 * img){


}

void bmp8_printInfo(t_bmp8 * img){


}