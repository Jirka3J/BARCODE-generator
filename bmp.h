//
// Created by jirka on 02.05.2026.
//

#ifndef BMP_H
#define BMP_H

#define RCOLOR 0
#define GCOLOR 0
#define BCOLOR 0

#include "barcode.h"

void save_bar_bmp(const char *filename, const char *pattern, int scale, int height,enum barcodeType);

#endif //BMP_H
