//
// Created by jirka on 02.05.2026.
//

#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void save_bar_bmp(const char *filename, const char *pattern, int scale, int height,enum barcodeType type) {
    int width = strlen(pattern)*scale;
    int paddedWidth = (width*3+3)&(~3);      //width * 24bit colorspace převedení na násobek 4
    height*=scale;


    // BITMAP HEADERS
    int filesize = 54 + height * paddedWidth;          // Header + height *
    unsigned char fileHeader[14] = {
        'B','M',              // 0–1: podpis BITMAP
        filesize,filesize>>8,filesize>>16,filesize>>24,     // 2–5: velikost souboru
        0,0, 0,0,             // 6–9: rezervováno
        54,0,0,0              // 10–13: offset pixelů
    };
    unsigned char infoHeader[40] = {
        40,0,0,0,             // velikost této hlavičky
        width,width>>8,width>>16,width>>24,                 // šířka in px
        height,height>>8,height>>16,height>>24,             // výška in px
        1,0,                  // počet rovin (vždy 1)
        24,0                  // 24-bit (RGB)
    };

    FILE *f = fopen(filename,"wb");
    fwrite(fileHeader, 1, 14, f);
    fwrite(infoHeader, 1, 40, f);


    unsigned char *row = (unsigned char*)calloc(paddedWidth, 1);
    int rcolor, gcolor, bcolor;
    int patternLen = strlen(pattern);
    int x_pixel=0;
    switch (type) {
        case EAN13:
            x_pixel=0;
        for(int i=0;i<patternLen;i++) {
            if(pattern[i]=='1'&&(i<3||(i>43&&i<50)||i>92)) {
                rcolor=RCOLOR;gcolor=GCOLOR;bcolor=BCOLOR;
            }else{rcolor=255;gcolor=255;bcolor=255;}
            for (int s = 0; s < scale; s++) {
                row[x_pixel * 3 + 1] = gcolor;
                row[x_pixel * 3 + 2] = rcolor;
                row[x_pixel * 3 + 0] = bcolor;
                x_pixel++;
            }
        }
        for(int y=0; y<3*scale; y++) {
            fwrite(row,1,paddedWidth,f);
        }

        case BIN:
            x_pixel=0;
            for(int i=0;i<patternLen;i++) {
                if(pattern[i]=='1') {
                    rcolor=RCOLOR;gcolor=GCOLOR;bcolor=BCOLOR;
                }else{rcolor=255;gcolor=255;bcolor=255;}
                for (int s = 0; s < scale; s++) {
                    row[x_pixel * 3 + 0] = bcolor;
                    row[x_pixel * 3 + 1] = gcolor;
                    row[x_pixel * 3 + 2] = rcolor;
                    x_pixel++;
                }
            }
            for(int y=3*scale*type; y<height; y++) {
                fwrite(row,1,paddedWidth,f);
            }
            break;
    }



    for(int y=0; y<height-3*scale; y++) {
        fwrite(row,1,paddedWidth,f);
    }



    free(row);
    fclose(f);
}
