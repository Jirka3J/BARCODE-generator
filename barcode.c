//
// Created by jirka on 02.05.2026.
//

#include "barcode.h"

#define BINARYCODELENGTH 100

int binary_check(const char* data) {
    for(int i=0;i<BINARYCODELENGTH;i++) {
        if(data[i]!='0'||data[i]!='1') {
            return -2;
        }if(data[i]=='\0'){return 0;}
    }
    return -2;
}
