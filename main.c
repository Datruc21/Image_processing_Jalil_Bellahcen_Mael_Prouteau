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

t_bmp8* open_file() {
  char* file_name = (char* )malloc(256);
  printf("File path :");
  scanf("%s", file_name);
  t_bmp8* image;
  char* complete_name = malloc(strlen(file_name) + 5);
  strcpy(complete_name, "../");
  strcat(complete_name, file_name);
  printf("File name:%s\n", complete_name);
  image = bmp8_loadImage(complete_name);
  if (image == NULL) {
  printf("Error opening file\n");
  return NULL;
  }
  printf("Image loaded succesfully\n");
  return image;
}

void save_file(t_bmp8* image) {
    char* file_name = malloc(256);
    printf("File path :");
    scanf(" %s", file_name);
    char* complete_name = malloc(strlen(file_name) + 5);
    strcpy(complete_name, "../");
    strcat(complete_name, file_name);
    printf("File name:%s\n", complete_name);
    bmp8_saveImage(complete_name, image);
    bmp8_free(image);
    printf("Image saved succesfully\n");
}

t_bmp8* choose_filter(t_bmp8* image) {
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
 bmp8_negative(image);
 break;
 case 2:
 printf("Select a brightness \n");
 int brightness;
 scanf("%d", &brightness);
 bmp8_brightness(image, brightness);
 break;
 case 3:
 printf("Select a threshold \n");
 int threshold;
 scanf("%d", &threshold);
 bmp8_threshold(image, threshold);
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
 bmp8_applyFilter(image,boxBlur,3);
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
 bmp8_applyFilter(image,gaussianBlur,3);
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
 bmp8_applyFilter(image,sharpness,3);
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
 bmp8_applyFilter(image,outline,3);
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
 bmp8_applyFilter(image,emboss,3);
 break;
 }
 return image;
}

void main_menu(t_bmp8* image) {
  int option;
  printf("Please choose an option :\n");
  printf("\t 1. Open an image\n");
  printf("\t 2. Save an image\n");
  printf("\t 3. Apply a filter\n");
  printf("\t 4. Display image information\n");
  printf("\t 5. Quit\n");
  scanf("%d", &option);
  switch (option) {
  case 1:
    printf("Your choice: 1\n");
    image = open_file();
    main_menu(image);
    break;
  case 2:
    printf("Your choice: 2\n");
    save_file(image);
    main_menu(image);
    break;
  case 3:
    printf("Your choice: 3\n");
    image = choose_filter(image);
    printf("Filter applied ...\n");
    main_menu(image);
    break;
  case 4:
    printf("Your choice: 4\n");
    bmp8_printInfo(image);
    main_menu(image);
    break;
  }
    printf("Filter chosen");
  return;
}

int main(void) {
    t_bmp8* image;
    main_menu(image);
    return 0;
  }


