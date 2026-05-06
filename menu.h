//
// Created by jirka on 02.05.2026.
//


#ifndef MENU_H
#define MENU_H

#include "barcode.h"

typedef struct arguments {
    char* file;
    enum barcodeType btype;
    char* data;
    int scale;
}arguments_t;

typedef enum  {up,down,odejit,enter,unknown} action_t; //výčtový typ pro ovládání akcí rozhraní konzoly


/**
 *
 * @param args - arguments_t structure to fill with user input
 * @return
 */
int runTerminal(arguments_t* args);

#endif //MENU_H

