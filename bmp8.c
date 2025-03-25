#include "bmp8.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_bmp8 * bmp8_loadImage(const char* filename){
    t_bmp8 * new_image = (t_bmp8 *) malloc(sizeof(t_bmp8));
    if (new_image == NULL ) {printf("Problems in memory allocation");free(new_image);return NULL;}
    FILE *f = fopen(filename, "rb");
    unsigned char header[54];
    fread(header, 1, 54, f);
    unsigned char colorTable[1024];
    unsigned int width = *(unsigned int *)&header[18];
    unsigned int height = *(unsigned int *)&header[22];
    unsigned int colorDepth = *(unsigned int *)&header[28];
    unsigned int dataSize = *(unsigned int *)&header[34];
    strcpy(new_image -> header, header);
    new_image -> width = width;
    new_image -> height = height;
    new_image -> colorDepth = colorDepth;
    new_image -> dataSize = dataSize;
    if (new_image -> colorDepth != 8) {
        printf("Image is not 8 bits deep!");
        free(new_image);
        return NULL;
    }
    new_image -> data = (unsigned char *)malloc(sizeof(unsigned char));
    if (new_image -> data == NULL) {printf("Problems in memory allocation");free(new_image -> data);free(new_image);return NULL;}
    return new_image;
    // Ne pas oublier de fermer le fichier et de remplir data
}

void bmp8_saveImage(const char * filename, t_bmp8 * img){


}

void bmp8_free(t_bmp8 * img){


}

void bmp8_printInfo(t_bmp8 * img){


}