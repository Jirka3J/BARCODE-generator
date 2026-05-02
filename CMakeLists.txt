cmake_minimum_required(VERSION 3.28)
project(barcode C)

set(CMAKE_C_STANDARD 23)

add_executable(barcode main.c
        barcode.c
        barcode.h
        bmp.c
        bmp.h
        EAN13.c
        EAN13.h
)
