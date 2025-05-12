#include <stdio.h>
#include "bmp8.h"
#include <stdlib.h>
#include "bmp24.h"
#include "string.h"

//this is a test

/*int main(void) {
    t_bmp8 * new_image = bmp8_loadImage("../lena_gray.bmp");
    float gaussian_blur[3][3] = {
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
    // Allocate memory for every row of the matrix
    for (int i = 0; i < 3; i++) {
        filter[i] = (float *)malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            filter[i][j] = gaussian_blur[i][j];
        }
    }
    bmp8_printInfo(new_image);
    bmp8_applyFilter(new_image,filter,3);
    bmp8_saveImage("../img.bmp",new_image);
    bmp8_free(new_image);
    return 0;
}*/

//with only the code above, main works

float gaussian_blur[3][3] = {
    {1.0f/16, 2.0f/16, 1.0f/16},
    {2.0f/16, 4.0f/16, 2.0f/16},
    {1.0f/16, 2.0f/16, 1.0f/16}
};
float Outline[3][3] = {
    {-1, -1, -1},
    {-1, 8, -1},
    {-1, -1, -1}
};
float box_blur[3][3] = {
    {1.0f/9, 1.0f/9, 1.0f/9},
    {1.0f/9, 1.0f/9, 1.0f/9},
    {1.0f/9, 1.0f/9, 1.0f/9}
};
float sharpen[3][3] = {
    {0,-1,0},
    {-1,5,-1},
    {0,-1,0}
};
float Emboss[3][3] = {
    {-2,-1,0},
    {-1,1,1},
    {0,1,2}
};

void open_file(t_bmp8** image8, t_bmp24** image24) {
  char* file_name = (char* )malloc(256);
  printf("File path :");
  scanf("%s", file_name);
    int option;
    printf("\t 1. Bmp8 image\n");
    printf("\t 2. Bmp24 image\n");
    scanf("%d", &option);
    switch (option) {
        case 1:
            char* complete_name = malloc(strlen(file_name) + 5);
            strcpy(complete_name, "../");
            strcat(complete_name, file_name);
            printf("File name:%s\n", complete_name);
            *image8 = bmp8_loadImage(complete_name);
            if (*image8 == NULL) {
                printf("Error opening file\n");
                return;
            }
            printf("Image loaded succesfully\n");
            break;
        case 2:
            char* complete_name24 = malloc(strlen(file_name) + 5);
            strcpy(complete_name24, "../");
            strcat(complete_name24, file_name);
            printf("File name:%s\n", complete_name24);
            *image24 = bmp24_loadImage(complete_name24);
            if (*image24 == NULL) {
                printf("Error opening file\n");
                return;
            }
            printf("Image loaded succesfully\n");
            break;
    }
}

void save_file(t_bmp8* image8, t_bmp24* image24) {
    char* file_name = malloc(256);
    printf("File path :");
    scanf(" %s", file_name);
    char* complete_name = malloc(strlen(file_name) + 5);
    strcpy(complete_name, "../");
    strcat(complete_name, file_name);
    printf("File name:%s\n", complete_name);
    int option;
    printf("\t 1. Bmp8 image\n");
    printf("\t 2. Bmp24 image\n");
    scanf("%d", &option);
    switch (option) {
        case 1:
            bmp8_saveImage(complete_name, image8);
            bmp8_free(image8);
            break;
        case 2:
            bmp24_saveImage(image24, complete_name);
            bmp24_free(image24);
    }
    printf("Image saved succesfully\n");
}

void choose_filter(t_bmp8* image8, t_bmp24* image24) {
    int option;
    printf("\t 1. Bmp8 image\n");
    printf("\t 2. Bmp24 image\n");
    scanf("%d", &option);
    switch (option) {
        case 1:
         printf("Choose a filter\n");
         int filter;
         printf("1. Negative\n");
         printf("2. Brightness\n");
         printf("3. Black and White\n");
         printf("4. Box Blur\n");
         printf("5. Gaussian Blur\n");
         printf("6. Sharpness\n");
         printf("7. Outline\n");
         printf("8. Emboss\n");
         scanf("%d", &filter);
         switch (filter) {
             case 1:
             bmp8_negative(image8);
             break;
             case 2:
             printf("Select a brightness \n");
             int brightness;
             scanf(" %d", &brightness);
             bmp8_brightness(image8, brightness);
             break;
             case 3:
             printf("Select a threshold \n");
             int threshold;
             scanf(" %d", &threshold);
             bmp8_threshold(image8, threshold);
             break;
             case 4:
             float** boxBlur = (float **)malloc(3 * sizeof(float *));
                // Allocate memory for every row of the matrix
                for (int i = 0; i < 3; i++) {
                    boxBlur[i] = (float *)malloc(3 * sizeof(float));
                    for (int j = 0; j < 3; j++) {
                        boxBlur[i][j] = box_blur[i][j];
                    }
                }
             bmp8_applyFilter(image8,boxBlur,3);
             break;
             case 5:
             float** gaussianBlur = (float **)malloc(3 * sizeof(float *));
                // Allocate memory for every row of the matrix
                for (int i = 0; i < 3; i++) {
                    gaussianBlur[i] = (float *)malloc(3 * sizeof(float));
                    for (int j = 0; j < 3; j++) {
                        gaussianBlur[i][j] = gaussian_blur[i][j];
                    }
                }
             bmp8_applyFilter(image8,gaussianBlur,3);
             break;
             case 6 :
             float** sharpness = (float **)malloc(3 * sizeof(float *));
                // Allocate memory for every row of the matrix
                for (int i = 0; i < 3; i++) {
                    sharpness[i] = (float *)malloc(3 * sizeof(float));
                    for (int j = 0; j < 3; j++) {
                        sharpness[i][j] = sharpen[i][j];
                    }
                }
             bmp8_applyFilter(image8,sharpness,3);
             break;
             case 7:
             float** outline = (float **)malloc(3 * sizeof(float *));
                // Allocate memory for every row of the matrix
                for (int i = 0; i < 3; i++) {
                    outline[i] = (float *)malloc(3 * sizeof(float));
                    for (int j = 0; j < 3; j++) {
                        outline[i][j] = Outline[i][j];
                    }
                }
             bmp8_applyFilter(image8,outline,3);
             break;
             case 8:
             float** emboss = (float **)malloc(3 * sizeof(float *));
                // Allocate memory for every row of the matrix
                for (int i = 0; i < 3; i++) {
                    emboss[i] = (float *)malloc(3 * sizeof(float));
                    for (int j = 0; j < 3; j++) {
                        emboss[i][j] = Emboss[i][j];
                    }
                }
             bmp8_applyFilter(image8,emboss,3);
             break;
         }
        break;
        case 2:
            printf("Choose a filter\n");
             printf("1. Negative\n");
             printf("2. Brightness\n");
             printf("3. Gray Scale\n");
             printf("4. Box Blur\n");
             printf("5. Gaussian Blur\n");
             printf("6. Sharpness\n");
             printf("7. Outline\n");
             printf("8. Emboss\n");
             scanf("%d", &filter);
             switch (filter) {
                 case 1:
                 bmp24_negative(image24);
                 break;
                 case 2:
                 printf("Select a brightness \n");
                 int brightness;
                 scanf("%d", &brightness);
                 bmp24_brightness(image24, brightness);
                 break;
                 case 3:
                 bmp24_grayscale(image24);
                 break;
                 case 4:
                 float** boxBlur = (float **)malloc(3 * sizeof(float *));
                    // Allocate memory for every row of the matrix
                    for (int i = 0; i < 3; i++) {
                        boxBlur[i] = (float *)malloc(3 * sizeof(float));
                        for (int j = 0; j < 3; j++) {
                            boxBlur[i][j] = box_blur[i][j];
                        }
                    }
                 bmp24_convolution(image24,boxBlur,3);
                 break;
                 case 5:
                 float** gaussianBlur = (float **)malloc(3 * sizeof(float *));
                    // Allocate memory for every row of the matrix
                    for (int i = 0; i < 3; i++) {
                        gaussianBlur[i] = (float *)malloc(3 * sizeof(float));
                        for (int j = 0; j < 3; j++) {
                            gaussianBlur[i][j] = gaussian_blur[i][j];
                        }
                    }
                 bmp24_convolution(image24,gaussianBlur,3);
                 break;
                 case 6 :
                 float** sharpness = (float **)malloc(3 * sizeof(float *));
                    // Allocate memory for every row of the matrix
                    for (int i = 0; i < 3; i++) {
                        sharpness[i] = (float *)malloc(3 * sizeof(float));
                        for (int j = 0; j < 3; j++) {
                            sharpness[i][j] = sharpen[i][j];
                        }
                    }
                 bmp24_convolution(image24,sharpness,3);
                 break;
                 case 7:
                 float** outline = (float **)malloc(3 * sizeof(float *));
                    // Allocate memory for every row of the matrix
                    for (int i = 0; i < 3; i++) {
                        outline[i] = (float *)malloc(3 * sizeof(float));
                        for (int j = 0; j < 3; j++) {
                            outline[i][j] = Outline[i][j];
                        }
                    }
                 bmp24_convolution(image24,outline,3);
                 break;
                 case 8:
                 float** emboss = (float **)malloc(3 * sizeof(float *));
                    // Allocate memory for every row of the matrix
                    for (int i = 0; i < 3; i++) {
                        emboss[i] = (float *)malloc(3 * sizeof(float));
                        for (int j = 0; j < 3; j++) {
                            emboss[i][j] = Emboss[i][j];
                        }
                    }
                 bmp24_convolution(image24,emboss,3);
                 break;
             }
        break;
    }

}

void apply_histogram(t_bmp8* image8, t_bmp24* image24) {
    int option;
    printf("\t 1. Bmp8 image\n");
    printf("\t 2. Bmp24 image\n");
    scanf("%d", &option);
    switch (option) {
        case 1:
            unsigned int* values = bmp8_computeHistogram(image8);
        unsigned int* histogram = bmp8_computeCDF(values);
        bmp8_equalize(image8, histogram);
        break;
        case 2:
            // for bmp24 usage
            break;
    }
}

void main_menu(t_bmp8* image8, t_bmp24* image24) {
  int option;
  printf("Please choose an option :\n");
  printf("\t 1. Open an image\n");
  printf("\t 2. Save an image\n");
  printf("\t 3. Apply a filter\n");
  printf("\t 4. Display image information\n");
    printf("\t 5. Histogram\n");
  printf("\t 6. Quit\n");
  scanf("%d", &option);
  switch (option) {
      case 1:
          printf("Your choice: 1\n");
      open_file(&image8, &image24);
      main_menu(image8, image24);
      break;
      case 2:
          printf("Your choice: 2\n");
      save_file(image8, image24);
      main_menu(image8, image24);
      break;
      case 3:
          printf("Your choice: 3\n");
      choose_filter(image8, image24);
      printf("Filter applied ...\n");
      main_menu(image8, image24);
      break;
      case 4:
          printf("Your choice: 4\n");
      bmp8_printInfo(image8);
      main_menu(image8, image24);
      break;

      case 5:
          printf("Your choice: 5\n");
      //calculate the histogram and create the new image
      apply_histogram(image8, image24);
      main_menu(image8, image24);
      break;
  }
}

int main(void) {
    t_bmp8* image8 = NULL;
    t_bmp24* image24 = NULL;
    main_menu(image8, image24);
    return 0;
  }


