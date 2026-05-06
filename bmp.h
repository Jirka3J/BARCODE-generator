//
// Created by jirka on 02.05.2026.
//

#ifndef BMP_H
#define BMP_H

#define RCOLOR 0
#define GCOLOR 0
#define BCOLOR 0

#include "barcode.h"

/**
 *
 * @param filename - name of the output file, should end with .bmp
 * @param pattern - string of binary data
 * @param scale - int, pixel multiplier for rendering bmp
 * @param height - pixel height of bitmap, will be multiplied with scale
 * @param type - type of the barcode, used for rendering EAN-13 guard bars
 */

void save_bar_bmp(const char *filename, const char *pattern, int scale, int height,enum barcodeType type);

#endif //BMP_H
