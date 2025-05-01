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
    file_rawRead(HEADER_SIZE,&header_info,INFO_SIZE,1,f);
    image -> header = header;
    bmp24_readPixelData(image,f);
    image -> header_info = header_info;
    fclose(f);
    return image;
}


void bmp24_readPixelValue(t_bmp24* image, int x, int y, FILE * file) {
    file_rawRead(BITMAP_OFFSET+(x*(image -> height) + y),&(image -> data[image -> height-x][y]),3,1,file);
    uint8_t tmp = (image -> data[x-image -> height][y]).red;
    (image -> data[image -> height-x][y]).red = (image -> data[image -> height-x][y]).blue;
    (image -> data[image -> height-x][y]).blue = tmp;
}

void bmp24_readPixelData(t_bmp24* image, FILE* file) {
    for (int i=1; i <= (image -> height) ; i++) {
        for (int j = 0; j<image->width; j++) {
            bmp24_readPixelValue(image, (image -> height)-i, j, file);
        }
    }
}


void bmp24_writePixelValue(t_bmp24* image, int x, int y, FILE * file) {
    uint8_t tmp = (image -> data[image -> height-x][y]).red;
    (image -> data[image -> height-x][y]).red = (image -> data[image -> height-x][y]).blue;
    (image -> data[image -> height-x][y]).blue = tmp;
    file_rawWrite(BITMAP_OFFSET+x+((image -> height) - y) * (image -> width),&(image -> data[image -> height-x][y]),3,1,file);
}

void bmp24_writePixelData(t_bmp24* image, FILE* file) {
    for (int i=1; i < (image -> height); i++) { //we begin at last row and go up
        for (int j = 0; j<image->width; j++) {
            bmp24_writePixelValue(image, (image-> height)-i, j, file);
        }
    }
}


void bmp24_saveImage(t_bmp24* img, const char* filename) {
    FILE* f;
    f = fopen(filename, "wb");
    if (f == NULL){printf("Error while reading the file!\n");return;}
    file_rawWrite(BITMAP_MAGIC,&(img -> header),HEADER_SIZE,1,f);
    file_rawWrite(HEADER_SIZE,&(img -> header_info),INFO_SIZE,1,f);
    bmp24_writePixelData(img,f);
    fclose(f);
}


void bmp24_negative (t_bmp24* img) {
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++) {
            (img -> data[i][j]).red = 255 - (img -> data[i][j]).red;
            (img -> data[i][j]).green = 255 - (img -> data[i][j]).green;
            (img -> data[i][j]).blue = 255 - (img -> data[i][j]).blue;
        }
    }
}
void bmp24_grayscale (t_bmp24* img) {
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++) {
            int avg = ((img -> data[i][j]).blue + (img -> data[i][j]).green + (img -> data[i][j]).red)/3;
            (img -> data[i][j]).red = avg;
            (img -> data[i][j]).green = avg;
            (img -> data[i][j]).blue = avg;
        }
    }
}
void bmp24_brightness (t_bmp24* img, int value) {
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++){
            if ((img -> data[i][j].red) + value <0)
                img -> data[i][j].red = 0;
            else if ((img -> data[i][j].red) + value >255)
                img -> data[i][j].red = 255;
            else
                (img -> data[i][j].red) + value;
            if ((img -> data[i][j].green) + value <0)
                img -> data[i][j].green = 0;
            else if ((img -> data[i][j].green) + value >255)
                img -> data[i][j].green = 255;
            else
                (img -> data[i][j].green) + value;
            if ((img -> data[i][j].blue) + value <0)
                img -> data[i][j].blue = 0;
            else if ((img -> data[i][j].blue) + value >255)
                img -> data[i][j].blue = 255;
            else
                (img -> data[i][j].blue) + value;
        }
    }
}

