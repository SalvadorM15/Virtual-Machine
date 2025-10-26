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
    char version,vmi[32];
    unsigned int memoria;
    int d, p,cantPar, argcMV, argvMV;
    char fileName[SIZE];
    srand(time(NULL));
    argcMV = 0;
    argvMV = -1;
    manejaArgumentos(argC, argV, vmx, vmi, &d, &p, &argcMV, argvMV, &paramSeg, &mv);

    
    if(vmx[0] != '\0'){
        //viene un .vmx y puede venir o no un .vmi
        strcpy(fileName,vmx);
        lectura_arch(&mv,fileName, &codeSeg, &dataSeg, &extraSeg, &stackSeg, &constSeg, &offsetEP, &version);
        iniciaMV(&mv,codeSeg,dataSeg,extraSeg,stackSeg,constSeg,paramSeg, offsetEP);
        if(version == 2){
            //Argc y Argv lo pongo en la pila a mano porque no son operandos validos
            iniciaPila(&mv, argcMV, argvMV);
        }
    }else
        leeImg(&mv, vmi);
    
    
    
    if(vmi[0] != '\0'){
        strcpy(mv.vmiFileName,vmi);
    }

        

    printf("%d \n\n\n", d);

    if(d == 1){
        disassembler(&mv, codeSeg);
        }

    do{
        step(&mv);
    }while(mv.registros[IP] > -1 && mv.registros[IP] < mv.registros[CS] + codeSeg);
}
//fin vmx.c


