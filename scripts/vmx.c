#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "VM.h"
#define SIZE 30



void main(int argC, char *argV[]){

    MaquinaVirtual mv;
    unsigned short int  codeSeg, dataSeg, extraSeg, stackSeg, constSeg, offsetEP,paramSeg;
    char fileName[SIZE];
    srand(time(NULL));

    strcpy(fileName,argV[1]);
    lectura_arch(&mv,fileName, &codeSeg, &dataSeg, &extraSeg, &stackSeg, &constSeg, &offsetEP);
    iniciaMV(&mv,codeSeg,dataSeg,extraSeg,stackSeg,constSeg,paramSeg);

    if(strcmp(argV[3],"-d") == 0)
        disassembler(&mv, codeSeg);


    do{
        step(&mv);
    }while(mv.registros[IP] > -1 && mv.registros[IP]<);
}
//fin vmx.c


