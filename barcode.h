//
// Created by jirka on 02.05.2026.
//

#ifndef BARCODE_H
#define BARCODE_H

enum barcodeType {BIN,EAN13};


/**
 *
 * @param data - data for check
 * @return - errors
 */
int binary_check(const char* data);

#endif //BARCODE_H
