#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "VM.h"
#define SIZE 30



void main(int argC, char *argV[]){

    MaquinaVirtual mv;
    unsigned short int  codeSeg, dataSeg, extraSeg, stackSeg, constSeg, offsetEP,paramSeg;
    char vmx[32];
    char vmi[32];
    unsigned int memoria;
    int d, p, version,cantPar, argcMV, argvMV[100];
    char fileName[SIZE];
    srand(time(NULL));

    manejaArgumentos(argC, argV, vmx, vmi, &d, &p, &argcMV, argvMV, &paramSeg &mv);
    printf(".vmx: %s\n", vmx);
    printf(".vmi: %s\n", vmi);
    printf("dissasembler: %d\n", d);

    if(vmx[0] != '\0'){
        //viene un .vmx y puede venir o no un .vmi
        strcpy(fileName,vmx);
        lectura_arch(&mv,fileName, &codeSeg, &dataSeg, &extraSeg, &stackSeg, &constSeg, &offsetEP, &version);
        iniciaMV(&mv,codeSeg,dataSeg,extraSeg,stackSeg,constSeg,paramSeg, offsetEP);
        if(version == 2)
            iniciaPila(&mv, argcMV, argvMV);
        
    }else{
        //SOLO VIENE UN ARCHIVO .VMI
        leeImg(&mv, vmi);
    }
    
    if(vmi[0] != '\0'){
        
    }

    if(d == 1){
        printf("entre en el disassembler");
        disassembler(&mv, codeSeg);
    }

    do{
        printf("ejecuto 1 paso\n");
        step(&mv);
    }while(mv.registros[IP] > -1 && mv.registros[IP] < mv.registros[CS] + codeSeg);
}
//fin vmx.c


