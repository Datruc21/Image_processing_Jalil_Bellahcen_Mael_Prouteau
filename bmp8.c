#include "bmp8.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
    if (fread(new_image->colorTable, 1, 1024, f) != 1024) {
        printf("Error reading BMP color table!\n");
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
    printf("Trying to save to %s\n", filename);
    FILE *f = fopen(filename, "wb");
    if (f == NULL){
        printf("Error while writing the file!\n");
        return;}
    fwrite(img -> header,1,54,f);
    fwrite(img->colorTable, 1, 1024, f);
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

void bmp8_negative(t_bmp8 * img){
    for (int i = 0;i<img -> dataSize; i++){
        img -> data[i] = 255 - img -> data[i];
    }
}
void bmp8_brightness(t_bmp8 * img, int value){
    for (int i = 0;i<img -> dataSize; i++){
        if (img -> data[i] + value <0)
            img -> data[i] = 0;
        else if (img -> data[i] + value >255)
            img -> data[i] = 255;
        else
            img -> data[i] = img -> data[i] + value;
    }
}

void bmp8_threshold(t_bmp8 * img, int threshold){
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] < threshold) ? 0 : 255;
    }
}

float** buildMatrix(unsigned char* array,int m, int n) {
    // Allouer la mémoire pour un tableau de n pointeurs (lignes)
    float **matrix = (float **)malloc(n * sizeof(float *));

    // Allouer de la mémoire pour chaque ligne de la matrice
    for (int i = 0; i < m; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
    }
    // Remplir la matrice avec les valeurs du tableau 1D
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = array[i * n + j];  // Mappage tableau 1D -> matrice 2D
        }
    }
    return matrix;
}


void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    int center = kernelSize/2;
    float** M1 = buildMatrix(img -> data, img -> height, img -> width); // The one used to store the data
    float** M2 = buildMatrix(img -> data, img -> height, img -> width); // The one used for calculations


    for (int i = 1; i < img -> height - 1; i++) {
        for (int j = 1; j < img -> width - 1; j++) {
            float sum = 0;
            for (int k = -center; k <= center; k++) {
                for (int l = -center; l <= center; l++) {
                    sum += M2[i + k][j + l] * kernel[center - k][center - l];  // Accès aux voisins
                }
            }
            if (sum < 0)
                sum = 0;
            if (sum > 255)
                sum = 255;
            M1[i][j] = sum;
        }
    }
    for (int i = 0;  i < img -> height;i++){
        for (int j = 0; j < img -> width; j++ ) {
            img -> data[i * img -> width + j] = (unsigned char)M1[i][j];
        }
        free(M1[i]);
        free(M2[i]);
    }
    free(M1);
    free(M2);
}

unsigned int* bmp8_computeHistogram(t_bmp8* img) {
    unsigned int* histogram = (unsigned int *)malloc(256 * sizeof(unsigned int));
    for (int i = 0; i < 256; i++) {//initialize all values to 0
        histogram[i] = 0;
    }
    for (int j = 0; j < img->dataSize; j++) {//add 1 each time
        histogram[img->data[j]] += 1;
    }
    return histogram;
}

unsigned int* bmp8_computeCDF(unsigned int* hist) {
    unsigned int* cdf = (unsigned int *)malloc(256 * sizeof(unsigned int));
    unsigned int* hist_eq = (unsigned int *)malloc(256 * sizeof(unsigned int));
    int min = 300;cdf[0] = hist[0];
    int sum = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
        if (min > cdf[i] && cdf[i]) {
            min = cdf[i];
        }
        sum += hist[i];
    }
    for (int i = 0; i < 256; i++) {
        float tmp =  255.0*((float)(cdf[i] - min)/(sum - min));
        hist_eq[i] = round(tmp);
    }
    free(cdf);
    return hist_eq;
}

void bmp8_equalize(t_bmp8* img, unsigned int* hist_eq) {
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = hist_eq[img->data[i]];
    }
}
