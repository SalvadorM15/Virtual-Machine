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

    manejaArgumentos(argC, argV, vmx, vmi, &d, &p, &argcMV, argvMV, &paramSeg, &mv);

    printf("VALOR DE LA MEMORIA: %d\n", mv.MemSize);
    
    if(vmx[0] != '\0'){
        //viene un .vmx y puede venir o no un .vmi
        strcpy(fileName,vmx);
        lectura_arch(&mv,fileName, &codeSeg, &dataSeg, &extraSeg, &stackSeg, &constSeg, &offsetEP, &version);
        iniciaMV(&mv,codeSeg,dataSeg,extraSeg,stackSeg,constSeg,paramSeg, offsetEP);
        if(argcMV > 0){
            iniciaPila(&mv, argcMV, argvMV);
        }
        
        printf("fin inicia PIla\n");
    }else{
        //SOLO VIENE UN ARCHIVO .VMI
        leeImg(&mv, vmi);
    }
    
    if(vmi[0] != '\0'){
        strcpy(mv.vmiFileName,vmi);
        printf("entre en leeImg\n");
    }
    
    if(d == 1){
        disassembler(&mv, mv.seg[mv.registros[CS] >> 16][1]);
    }

    do{
        //printf("IP: %x\n", mv.registros[IP]);
        step(&mv);
    }while(mv.registros[IP] > -1 && mv.registros[IP] < mv.registros[CS] + mv.seg[mv.registros[CS] >> 16][1]);
}
//fin vmx.c


