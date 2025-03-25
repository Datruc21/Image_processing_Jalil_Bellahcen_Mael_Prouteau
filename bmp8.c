#include "bmp8.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_bmp8 * bmp8_loadImage(const char* filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL){printf("Error while reading the file!\n");return NULL;}
    t_bmp8 * new_image = (t_bmp8 *) malloc(sizeof(t_bmp8));
    if (new_image == NULL ) {printf("Problems in memory allocation\n");free(new_image);fclose(f);return NULL;}
    unsigned char header[54];
    if (fread(header, 1, 54, f) != 54) {
        printf("Error reading BMP header!\n");
        free(new_image);
        fclose(f);
        return NULL;
    }
    unsigned char colorTable[1024];
    unsigned int width = *(unsigned int *)&header[18];
    unsigned int height = *(unsigned int *)&header[22];
    unsigned int colorDepth = *(unsigned int *)&header[28];
    unsigned int dataSize = *(unsigned int *)&header[34];

    for (int i = 0; i<54;i++) {
        *(new_image ->  header + i) = header[i];
    }
    new_image -> width = width;
    new_image -> height = height;
    new_image -> colorDepth = colorDepth;
    new_image -> dataSize = dataSize;
    if (new_image -> colorDepth != 8) {
        printf("Image is not 8 bits deep!\n");
        free(new_image);
        fclose(f);
        return NULL;
    }
    new_image -> data = (unsigned char *)malloc(new_image -> dataSize);
    if (new_image -> data == NULL) {printf("Problems in memory allocation\n");free(new_image -> data);free(new_image);fclose(f);return NULL;}
    fread(new_image -> data,1,new_image -> dataSize,f);
    fclose(f);
    return new_image;
}


void bmp8_saveImage(const char * filename, t_bmp8 * img){
    FILE *f = fopen(filename, "wb");
    if (f == NULL){printf("Error while opening the file!\n");return;}
    fwrite(img -> header,1,54,f);
    fwrite(img -> data,1,img -> dataSize,f);
    fclose(f);
}

void bmp8_free(t_bmp8 * img){
    free(img -> data);
    free(img);
}

void bmp8_printInfo(t_bmp8 * img){
    if (img != NULL) {
        printf("Image info :\n");
        printf("\tWidth:%u\n",img -> width);
        printf("\tHeight:%u\n",img -> height);
        printf("\tColor Depth:%u\n",img -> colorDepth);
        printf("\tData size:%u\n",img -> dataSize);
    }
}