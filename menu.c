//
// Created by jirka on 02.05.2026.
//

#include "menu.h"
#include "EAN13.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "barcode.h"

void print_header(void) {
    printf("\t\t\t\tBARCODE GENERATOR\n");
    printf("\t\t\t     -----------------------\n");
    printf ("------------------------------------------------------------------------------\n");
}

// get user input, if it's an arrow key return the corresponding action, if it's q return odejit, if it's enter return enter, otherwise return unknown
action_t selection()
{
    int ch1=0, ch2 =0;
    fflush(stdin);
    ch1 = getch();
    ch2 = 0;
    if (ch1 == 0xE0)
    {
        ch2 = getch();

        // determine what it was
        switch(ch2)
        {
            case 72: return up;//sipka nahoru
            case 80: return down;//sipka dolu
        };

    }
    else
    {
        switch (ch1)
        {
            case 'q': return odejit;
            case 13: return enter;
        }
    }

    fflush(stdin);
    return unknown;
}


// get filename from user, if it doesn't end with .bmp add it
int get_filename(arguments_t* args) {
    char filename[300];
    printf("Name of the output bmp file will be used to save the generated barcode the .bmp extension is optional.\n");
    printf("Enter output filename: ");
    scanf("%s", filename);

    size_t len = strlen(filename);
    // check if filename ends with .bmp, if not add it
    if (len >= 4 && strcmp(filename + len - 4, ".bmp") == 0) {
        args->file = malloc(len+1);
        strncpy(args->file, filename, len);
        args->file[len+1] = '\0';
        return 0;
    }
    args->file = malloc(len + 6);
    strcpy(args->file, filename);
    strcat(args->file, ".bmp\0");
    return 0;
}


// get barcode type from user, use arrow keys to select and enter to confirm
int get_barcode_type(arguments_t* args) {
    printf("Use arrow keys to select barcode type and press Enter to confirm.\n");
    printf("Enter barcode type: ");
    args->btype = BIN;
    printf ("   BIN");
    action_t action;
    do
    {
        action=selection();
        if ((action==up)&&(args->btype>BIN)) args->btype--;
        if ((action==down)&&(args->btype<EAN13)) args->btype++;
        switch (args->btype)
        {
            case BIN: printf ("\b\b\b\b\b\b   BIN"); break;
            case EAN13: printf ("\b\b\b\b\b\bEAN-13"); break;
        }
    }
    while (action!=enter);
    return 0;
}

// get data to encode from user, if it's binary check if it's valid, if it's EAN-13 generate the binary code and check if it's valid
void get_data(arguments_t* args) {
    char* data=NULL;
    switch (args->btype)
        {
        case BIN:
            printf("Enter binary data to encode (only 0 and 1): ");
            data = malloc(300);
            scanf("%s", data);
            if(binary_check(data)==-2) {
                fprintf(stderr,"Invalid binary data\n");
                free(data);
                get_data(args);
            }
            args->data = data;
            break;
        case EAN13:
            printf("Enter 12-digit EAN-13 code to encode: ");
            data = malloc(13);
            scanf("%s", data);
            if(generateEAN(data, &args->data)!=0) {
                fprintf(stderr,"Invalid EAN-13 code\n");
                free(data);
                get_data(args);
            }else {
                free(data);
            }
            break;
    }
}

// gets scale from user input
int get_scale(arguments_t* args) {
    args->scale=4;
    printf("Use arrow keys to select scale of the barcode and press Enter to confirm.\n");
    printf("Enter scale: %3d", args->scale);
    action_t action;
    do
    {
        action=selection();
        if ((action==down)&&(args->scale>0)) args->scale--;
        if ((action==up)&&(args->scale<100)) args->scale++;
        printf("\b\b\b%3d", args->scale);
    }while(action!=enter);
    return 0;
}

// displays generated barcode in terminal via ASCII
int display_barcode(arguments_t* args) {
    printf("Generated barcode:\n");
    for (int i=0; i<8; i++) {
        int len =strlen(args->data);
        for (int j=0; j<len; j++) {
            if (args->data[j] == '1') {
                printf("%c",219);
            } else {
                printf(" ");
            }
        }
        putchar('\n');
    }
    printf("press any key to continue");
    getc(stdin);
}

int runTerminal(arguments_t* args) {
    action_t action;
    print_header();
    printf("q - quit\n");
    printf("Enter - generate barcode\n");
    switch(selection()) {
        case odejit:
            return 1;
        case enter:
            break;
        default:
            system("cls");
            printf("Unknown action, try again\n");
        return -1;
    }
    system("cls");
    print_header();
    get_filename(args);
    system("cls");
    print_header();
    get_barcode_type(args);
    system("cls");
    print_header();
    get_data(args);
    system("cls");
    print_header();
    get_scale(args);
    system("cls");
    printf("Barcode is saved to %s\n",args->file);
    display_barcode(args);


    system("cls");
    return 0;
}
