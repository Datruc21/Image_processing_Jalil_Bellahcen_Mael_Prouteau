#include <stdio.h>
#include "bmp8.h"

int main(void) {
        t_bmp8 * new_image = bmp8_loadImage("../lena_gray.bmp");
    bmp8_printInfo(new_image);
    bmp8_brightness(new_image,70);
    bmp8_saveImage("../img.bmp",new_image);
    bmp8_free(new_image);
    return 0;
}