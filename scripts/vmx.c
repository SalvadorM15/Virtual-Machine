#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "VM.h"
#define SIZE 30



void main(int argC, char *argV[]){

    MaquinaVirtual mv;
    short int tamSeg;
    char fileName[SIZE];
    srand(time(NULL));
    
    strcpy(fileName,argV[1]);
    lectura_arch(&mv, &tamSeg,fileName);
    iniciaMV(&mv,tamSeg);

    if(argC == 3 && strcmp(argV[2],"-d") == 0){
        disassembler(&mv, tamSeg);
    }

    printf("tamanio del codigo: %d \n", tamSeg);
    do{
        step(&mv);
    }while(mv.registros[IP] > -1 && mv.registros[IP]<tamSeg);
}



