//
// Created by jirka on 02.05.2026.
//

#include "EAN13.h"

#include <stdlib.h>



int write_pattern(char* dest, const char* pattern) {
    int i = 0;
    while (pattern[i]) {
        dest[i] = pattern[i];
        i++;
    }
    return i;
}
// tabulka pro určení číslic levé strany
int checkG(int firstDigit, int position) {
    int parity[10][6] = {
        {0,0,0,0,0,0},
        {0,0,1,0,1,1},
        {0,0,1,1,0,1},
        {0,0,1,1,1,0},
        {0,1,0,0,1,1},
        {0,1,1,0,0,1},
        {0,1,1,1,0,0},
        {0,1,0,1,0,1},
        {0,1,0,1,1,0},
        {0,1,1,0,1,0}
    };
    return parity[firstDigit][position - 1];
}

int write_digit_left(char* dest, int digit, int useG) {
    // kódování s lichou paritou
    const char *L[10] = {
        "0001101","0011001","0010011","0111101","0100011",
        "0110001","0101111","0111011","0110111","0001011"
    };
    // kódování se sudou paritou
    const char *G[10] = {
        "0100111","0110011","0011011","0100001","0011101",
        "0111001","0000101","0010001","0001001","0010111"
    };
    if(useG==1) {
        return write_pattern(dest,G[digit]);
    }else if(useG==0) {
        return write_pattern(dest,L[digit]);
    }return 0;
}


// zápis pravé strany, konstantí kódování
int write_digit_right(char* dest, int digit) {
    const char *R[10] = {
        "1110010","1100110","1101100","1000010","1011100",
        "1001110","1010000","1000100","1001000","1110100"
    };
    return write_pattern(dest,R[digit]);
}

int checkEAN(const char* code,unsigned int *ncode) {
    int sum=0;
    for(int i=11;i>=0;i--) {
        ncode[i]=code[i]-'0';
        if (ncode[i]>9) {
            return -1;
        }else if(i%2) {
            sum+=ncode[i]*3;
        }else {
            sum+=ncode[i];
        }
    }
    if(code[12]!='\0'){return -1;}
    ncode[12]=10-sum%10;
    return ncode[12];
}

int generateEAN(const char* code,char** bin_code) {
    unsigned int ncode[13];
    if(checkEAN(code,ncode)==-1){return -1;};

    *bin_code=calloc(95+1,1);
    int pos = 0;
    // zápis start patternu
    pos += write_pattern(*bin_code + pos, "101");
    // zápis levé poloviny
    for (int i = 1; i <= 6; i++) {
        pos += write_digit_left(*bin_code + pos, ncode[i],checkG(ncode[0],i));
    }
    // zápis středového patternu
    pos += write_pattern(*bin_code + pos, "01010");
    // zápis pravé poloviny
    for (int i = 7; i <= 12; i++) {
        pos += write_digit_right(*bin_code + pos, ncode[i]);
    }
    // zápis koncového patternu
    pos += write_pattern(*bin_code + pos, "101");
    (*bin_code)[pos]='\0';

    return 0;
}

