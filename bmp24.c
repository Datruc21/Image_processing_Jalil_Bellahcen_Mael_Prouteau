#include "bmp24.h"

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void* buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

t_pixel** bmp24_allocateDataPixels(int width, int height) {
    t_pixel** matrix = (t_pixel**)malloc(sizeof(t_pixel*)*height);
    if (!matrix) {
        printf("Error in allocation for the matrix!\n");
        return NULL;
    }
    for (int i = 0; i<height; i++) {
        t_pixel* row = (t_pixel*)malloc(sizeof(t_pixel)*width);
        if (!row) {
            printf("Error in allocation for the pixels!\n");
            return NULL;
        }
        matrix[i] = row;
    }
    return matrix;
}

void bmp24_freeDataPixels(t_pixel ** pixels, int height) {
    for (int i = 0; i<height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}


t_bmp24* bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24* image = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (!image) {
        printf("Error in allocation for the image!\n");
        return NULL;
    }
    image -> width = width;
    image -> height = height;
    image -> colorDepth = colorDepth;
    image -> data = bmp24_allocateDataPixels(image -> width, image -> height);
    if (!(image -> data)) {
        free(image);
        return NULL;
    }
    return image;
}


void bmp24_free(t_bmp24 * img) {
    bmp24_freeDataPixels(img -> data, img -> height);
    free(img);
}

t_bmp24* bmp24_loadImage(const char * filename) {
    FILE* f;
    f = fopen(filename, "rb");
    if (f == NULL){printf("Error while reading the file!\n");return NULL;}

    int width,height,colorDepth; // Parameters for the dynamic allocation
    file_rawRead(BITMAP_WIDTH,&width,sizeof(int),1,f);
    file_rawRead(BITMAP_HEIGHT,&height,sizeof(int),1,f);
    file_rawRead(BITMAP_DEPTH,&colorDepth,sizeof(int),1,f);
    if (colorDepth != DEFAULT_DEPTH) {
        printf("This image is not 24 bits deep!");
        fclose(f);
        return NULL;
    }
    t_bmp24* image = bmp24_allocate(width,height,colorDepth);
    if (!image) {
        fclose(f);
        return NULL;
    }
    t_bmp_header header;
    t_bmp_info header_info;
    file_rawRead(BITMAP_MAGIC, &header, HEADER_SIZE, 1, f); // Header of the file
    file_rawRead(14,&header_info,INFO_SIZE,1,f);
    image -> header = header;
    image -> width = width;
    image -> height = height;
    image -> colorDepth = colorDepth;
    image -> data = bmp24_allocateDataPixels(width, height);
    image -> header_info = header_info;
    fclose(f);
    return image;
} // need to add the pixels data and review the header info



void bmp24_readPixelValue(t_bmp24* image, int x, int y, FILE * file) { //pixel data is a reversed matrix, the first row is at the bottom
    t_pixel new;
    file_rawRead(BITMAP_OFFSET+x+((image -> height) - y) * (image -> width),&new,3,1,file);
    return;

}

void bmp24_readPixelData(t_bmp24* image, FILE* file) {

    return;

}
