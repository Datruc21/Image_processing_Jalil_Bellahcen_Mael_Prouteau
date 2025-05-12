cmake_minimum_required(VERSION 3.30)
project(Image_processing_Jalil_Bellahcen_Mael_Prouteau C)

set(CMAKE_C_STANDARD 11)

add_executable(Image_processing_Jalil_Bellahcen_Mael_Prouteau main.c
        bmp8.c
        bmp8.h
        bmp24.c
        bmp24.h)

target_link_libraries(Image_processing_Jalil_Bellahcen_Mael_Prouteau m)