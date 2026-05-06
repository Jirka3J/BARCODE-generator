#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "EAN13.h"
#include "menu.h"
#include "barcode.h"

#define ERR1 "Unknown type, try EAN13, BIN"
#define ERR2 "Invalid code, EAN13 has 12 digits, binary is limited"



char* ensure_bmp_extension(const char* filename) {
    size_t len = strlen(filename);
    if (len >= 4 && strcmp(filename + len - 4, ".bmp") == 0) {
        return (char*)filename;
    }
    char *result = malloc(len + 6);
    if (!result) return NULL;
    strcpy(result, filename);
    strcat(result, ".bmp\0");
    return result;

}

void printhelp(void) {
    printf("Usage:\n");
    printf("  barcode <file> <EAN13|BIN> <data> <scale>\n");
    printf("\nExample:\n");
    printf("  barcode.exe out EAN13 863462131723 3\n");
}

int runCLI(int argc, char *argv[]) {
    // help print
    if(strcmp(argv[1],"-h")==0) {
        printhelp();
        return 0;
    }

    // CLI Managment
    char *file = argv[1];
    const char *type = argv[2];
    enum barcodeType etype;
    const char *data = argv[3];
    int scale = 4;
    if (argc > 4 && argv[4] != NULL) {
        scale = atoi(argv[4]);
    }
    char *bin;


    // kontrola filename, doplneni .bmp
    file = ensure_bmp_extension(argv[1]);
    // GENERACE a KONTROLA EAN-13
    if (strcmp(type, "EAN13") == 0) {
        etype=EAN13;
        if (generateEAN(data, &bin) != 0) {
            fprintf(stderr,ERR2);
            return 2;
        }
    }
    // KONTROLA BINARNIHO KODU
    else if (strcmp(type, "BIN") == 0) {
        etype=BIN;
        if(binary_check(data)==-2) {
            fprintf(stderr,ERR2);
            return 2;
        };
    }
    else {
        fprintf(stderr,ERR1);
        return 1;
    }

    printf("generuji bitmapu...\n");
    save_bar_bmp(file,bin,scale,40,etype);
    printf("%s",file);
    return 0;
}

int main(int argc, char *argv[]){
    if (argc>1) {
        return runCLI(argc,argv);
    }else {
        arguments_t args;
        int return_code=0;
        while(return_code==0||return_code==-1) {
            return_code=runTerminal(&args);
            save_bar_bmp(args.file,args.data,args.scale,40,args.btype);
            if (return_code==0) {
                free(args.file);
                free(args.data);
            }
        }
    }
    return 0;
}
