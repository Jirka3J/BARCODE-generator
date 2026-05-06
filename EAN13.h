//
// Created by jirka on 02.05.2026.
//

#ifndef EAN13_H
#define EAN13_H

/**
 *
 * @param code - 12-digit EAN-13 code to encode
 * @param bin_code - pointer to char* where the generated binary code will be stored, should be freed by caller
 * @return
 */
int generateEAN(const char* code,char** bin_code);

#endif //EAN13_H
