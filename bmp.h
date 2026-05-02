//
// Created by jirka on 02.05.2026.
//

#ifndef BMP_H
#define BMP_H

#define RCOLOR 166
#define GCOLOR 30
#define BCOLOR 15

enum barcodeType {BIN,EAN13};

void save_bar_bmp(const char *filename, const char *pattern, int scale, int height,enum barcodeType);

#endif //BMP_H
