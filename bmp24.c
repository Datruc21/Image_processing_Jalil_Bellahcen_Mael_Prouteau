#include "bmp24.h"

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void* buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


void reverse_rows(t_pixel** M, int width, int height) {
    t_pixel** A = bmp24_allocateDataPixels(width,height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            A[i][j] = M[i][j];
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            M[i][j] = A[height-i-1][j];
        }
    }
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
    image -> header_info = header_info;
    bmp24_readPixelData(image,f);
    fclose(f);
    return image;
}


void bmp24_readPixelValue(t_bmp24* image, int x, int y, FILE * file) {
    int stride = ((image->width * 3 + 3) / 4) * 4;
    file_rawRead(HEADER_SIZE + INFO_SIZE + x * stride + y * 3,&(image -> data[x][y]),3,1,file);
    uint8_t tmp = (image -> data[x][y]).red;
    (image -> data[x][y]).red = (image -> data[x][y]).blue;
    (image -> data[x][y]).blue = tmp;
}

void bmp24_readPixelData(t_bmp24* image, FILE* file) {
    for (int i=0; i < (image -> height) ; i++) {
        for (int j = 0; j<image->width; j++) {
            bmp24_readPixelValue(image, i, j, file);
        }
    }
    reverse_rows(image -> data, image -> width, image -> height);
}

void bmp24_writePixelValue(t_bmp24* image, int x, int y, FILE * file) {
    uint8_t tmp = (image -> data[x][y]).red;
    (image -> data[x][y]).red = (image -> data[x][y]).blue;
    (image -> data[x][y]).blue = tmp;
    int stride = ((image->width * 3 + 3) / 4) * 4;
    file_rawWrite(HEADER_SIZE + INFO_SIZE + x * stride + y * 3,&(image -> data[x][y]),3,1,file);
}

void bmp24_writePixelData(t_bmp24* image, FILE* file) {
    reverse_rows(image -> data, image -> width, image -> height);
    for (int i=0; i < (image -> height); i++) { //we begin at last row and go up
        for (int j = 0; j<image->width; j++) {
            bmp24_writePixelValue(image, i, j, file);
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
            (img -> data[i][j].red) += value;
            (img -> data[i][j].green) += value;
            (img -> data[i][j].blue) += value;
            int test_r = (img -> data[i][j].red);
            int test_g = (img -> data[i][j].green);
            int test_b = (img -> data[i][j].blue);
            if (test_r<0)
                img -> data[i][j].red = 0;
            else if (test_r>255)
                img -> data[i][j].red = 255;
            if (test_g <0)
                img -> data[i][j].green = 0;
            else if (test_g >255)
                img -> data[i][j].green = 255;
            if (test_b <0)
                img -> data[i][j].blue = 0;
            else if (test_b >255)
                img -> data[i][j].blue = 255;
        }
    }
}

void bmp24_convolution(t_bmp24 * img,float ** kernel, int kernelSize) {
    int center = kernelSize / 2;
    t_pixel** A = bmp24_allocateDataPixels(img -> width, img -> height);
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++) {
            A[i][j] = img->data[i][j];
        }
    }
    for (int i = 1; i < img-> height-1; i++) {
        for (int j = 1; j<img -> width-1; j++) {
            float sum_b = 0;
            float sum_g = 0;
            float sum_r = 0;
            for (int k = -center; k <= center; k++) {
                for (int l = -center; l <= center; l++) {
                    sum_b += A[i + k][j + l].blue * kernel[center - k][center - l];
                    sum_g += A[i + k][j + l].green * kernel[center - k][center - l];
                    sum_r += A[i + k][j + l].red * kernel[center - k][center - l];
                }
            }
            if (sum_b < 0)
                sum_b = 0;
            if (sum_b > 255)
                sum_b = 255;
            (img -> data)[i][j].blue = sum_b;
            if (sum_g < 0)
                sum_g = 0;
            if (sum_g > 255)
                sum_g = 255;
            (img -> data)[i][j].green = sum_g;
            if (sum_r < 0)
                sum_r = 0;
            if (sum_r > 255)
                sum_r = 255;
            (img -> data)[i][j].red = sum_r;

        }

    }
    bmp24_freeDataPixels(A, img -> height);
}







