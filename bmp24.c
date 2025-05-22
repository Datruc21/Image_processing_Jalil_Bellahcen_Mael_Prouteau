#include "bmp24.h"
#include "bmp8.h"

/*
    Module handling bmp24 images (creation, saving, applying filters,etc...)
    Done by Jalil Bellahcen and Maël Prouteau
*/

int limit(int a) {
    //This function caps the pixels values in case they exceed the values
    if (a > 255) {
        a = 255;
    }
    if (a < 0) {
        a = 0;
    }
    return a;
}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void* buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


void reverse_rows(t_pixel** M, int width, int height) {
    //Function used to reverse the matrix of pixels after reading and writing
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

t_yuv_pixel** yuv_allocateDataPixels(int width, int height) {
    //Creates dynamically an array of YUV pixels
    t_yuv_pixel** matrix = (t_yuv_pixel**)malloc(sizeof(t_yuv_pixel*) * height);
    if (!matrix) {
        printf("Error in allocation for the YUV matrix!\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        matrix[i] = (t_yuv_pixel*)malloc(sizeof(t_yuv_pixel) * width);
        if (!matrix[i]) {
            printf("Error in allocation for YUV row %d!\n", i);
            // Free already allocated rows
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

void yuv_freeDataPixels(t_yuv_pixel** pixels, int height) {
    //Free the YUV pixel matrix
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}


t_pixel** bmp24_allocateDataPixels(int width, int height) {
    //Same as YUV, but for RGB
    t_pixel** matrix = (t_pixel**)malloc(sizeof(t_pixel*)*height);
    if (!matrix) {
        printf("Error in allocation for the matrix!\n");
        return NULL;
    }
    for (int i = 0; i<height; i++) {
        t_pixel* row = (t_pixel*)malloc(sizeof(t_pixel)*width);
        if (!row) {
            printf("Error in allocation for the pixels!\n");
            free(matrix);
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
    //Creates dynamically a bmp24 structure
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
    //Free the image bmp24
    bmp24_freeDataPixels(img -> data, img -> height);
    free(img);
}

t_bmp24* bmp24_loadImage(const char * filename) {
    //Creates the image, and stores its informations
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
    //Reads the values of a single pixel
    int stride = ((image->width * 3 + 3) / 4) * 4;
    file_rawRead(HEADER_SIZE + INFO_SIZE + x * stride + y * 3,&(image -> data[x][y]),3,1,file);
    uint8_t tmp = (image -> data[x][y]).red;
    (image -> data[x][y]).red = (image -> data[x][y]).blue;
    (image -> data[x][y]).blue = tmp;
}

void bmp24_readPixelData(t_bmp24* image, FILE* file) {
    //Reads the values of the whole matrix
    for (int i=0; i < (image -> height) ; i++) {
        for (int j = 0; j<image->width; j++) {
            bmp24_readPixelValue(image, i, j, file);
        }
    }
    reverse_rows(image -> data, image -> width, image -> height);
}

void bmp24_writePixelValue(t_bmp24* image, int x, int y, FILE * file) {
    //Writes for one pixel
    uint8_t tmp = (image -> data[x][y]).red;
    (image -> data[x][y]).red = (image -> data[x][y]).blue;
    (image -> data[x][y]).blue = tmp;
    int stride = ((image->width * 3 + 3) / 4) * 4;
    file_rawWrite(HEADER_SIZE + INFO_SIZE + x * stride + y * 3,&(image -> data[x][y]),3,1,file);
}

void bmp24_writePixelData(t_bmp24* image, FILE* file) {
    //Writes for every pixels
    reverse_rows(image -> data, image -> width, image -> height);
    for (int i=0; i < (image -> height); i++) { //we begin at last row and go up
        for (int j = 0; j<image->width; j++) {
            bmp24_writePixelValue(image, i, j, file);
        }
    }
}


void bmp24_saveImage(t_bmp24* img, const char* filename) {
    //Saves the image by writing it
    FILE* f;
    f = fopen(filename, "wb");
    if (f == NULL){printf("Error while reading the file!\n");return;}
    file_rawWrite(BITMAP_MAGIC,&(img -> header),HEADER_SIZE,1,f);
    file_rawWrite(HEADER_SIZE,&(img -> header_info),INFO_SIZE,1,f);
    bmp24_writePixelData(img,f);
    fclose(f);
}


void bmp24_negative (t_bmp24* img) {
    //Negative filter
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++) {
            (img -> data[i][j]).red = 255 - (img -> data[i][j]).red;
            (img -> data[i][j]).green = 255 - (img -> data[i][j]).green;
            (img -> data[i][j]).blue = 255 - (img -> data[i][j]).blue;
        }
    }
}
void bmp24_grayscale (t_bmp24* img) {
    //Grayscale filter
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
    //Brigthness filer
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
    //Does a convolution with a filter, same method as bmp8, but 3 times
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


void bmp24_equalize(t_bmp24 * img) {
    /*We create a copy of the array of pixels that will store the YUV
    See bmp8 histo for more details
    */
    t_yuv_pixel** copy = yuv_allocateDataPixels(img -> width, img -> height);
    //Conversion of the copy in the YUV space
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++) {
            uint8_t R =  img -> data[i][j].red;
            uint8_t G =  img -> data[i][j].green;
            uint8_t B =  img -> data[i][j].blue;
            copy[i][j].Y = (float)(0.299*R + 0.587*G + 0.114*B);
            copy[i][j].U=  (float)(0.436*B - 0.14713*R - 0.28886*G);
            copy[i][j].V = (float)(0.615*R - 0.51499*G + 0.10001*B);
        }
    }
    //Now we do the whole histogram of copy.Y : need to convert it to 1D array then back into a matrix
    unsigned int* data = (unsigned int*)malloc(sizeof(unsigned int)*(img -> height*img -> width));
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++) {
            data[i*img -> width + j] = (unsigned int)copy[i][j].Y;
        }
    }
    unsigned int* histo = (unsigned int*)malloc(sizeof(unsigned int)*256);
    for (int i = 0; i<256; i++) {
        histo[i] = 0;
    }
    for (int i = 0; i<(img -> height)*(img -> width); i++) {
        histo[data[i]] += 1;
    }
    unsigned int* hist_eq = bmp8_computeCDF(histo); // The equalized histo

    //Here is the bmp8 equalize equivalent
    for (int i = 0; i < (img -> height*img -> width); i++) {
        data[i] = limit((int)hist_eq[data[i]]);

    }
    free(histo);
    //Now into a matrix
    for (int i = 0; i<(img -> height); i++) {
        for (int j = 0; j<img -> width; j++) {
            copy[i][j].Y = data[i*img -> width + j];
        }
    }
    free(data);
    free(hist_eq);
    //We convert it back
    for (int i = 0; i<img -> height; i++) {
        for (int j = 0; j<img -> width; j++) {
            float Y =  copy[i][j].Y;
            float U =  copy[i][j].U;
            float V =  copy[i][j].V;
            img->data[i][j].red = limit((int)(Y + 1.13983*V));
            img->data[i][j].green =  limit((int)(Y - 0.39465*U - 0.58060 * V));
            img->data[i][j].blue = limit((int)(Y + 2.03211 * U));
        }
    }
    yuv_freeDataPixels(copy, img -> height);
}





