#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "VM.h"


const char* identificarMnemonico(int codigo){
    switch (codigo){
    case SYS: return "SYS";
            break;
    case JMP: return "JMP";
            break;
    case JZ: return "JZ";
            break;
    case JP: return "JP";
            break;
    case JN: return "JN";
            break;
    case JNZ: return "JNZ";
            break;
    case JNP: return "JNP";
            break;
    case JNN: return "JNN";
            break;
    case NOT: return "NOT";
            break;
    case STOP: return "STOP";
            break;
    case MOV: return "MOV";
            break;
    case ADD: return "ADD";
            break;
    case SUB: return "SUB";
            break;
    case MUL: return "MUL";
            break;
    case DIV: return "DIV";
            break;
    case CMP: return "CMP";
            break;
    case SHL: return "SHL";
            break;
    case SHR: return "SHR";
            break;
    case SAR: return "SAR";
            break;
    case AND: return "AND";
            break;
    case OR: return "OR";
            break;
    case XOR: return "XOR";
            break;
    case SWAP: return "SWAP";
            break;
    case LDL: return "LDL";
            break;
    case LDH: return "LDH";
            break;
    case RND: return "RND";
            break;
    default: return "instruccion no identificada";
            break;
    }
}

const char* identificarRegistro(int op){
    switch (op){
    case LAR: return "LAR";
              break;
    case MAR: return "MAR";
            break;
    case MBR: return "MBR";
            break;
    case IP: return "IP";
            break;
    case OPC: return "OPC";
            break;
    case OP1: return "OP1";
            break;
    case OP2: return "OP2";
            break;
    case EAX: return "EAX";
            break;
    case EBX: return "EBX";
            break;
    case ECX: return "ECX";
            break;
    case EDX: return "EDX";
            break;
    case EFX: return "EFX";
            break;
    case EEX: return "EEX";
            break;
    case AC: return "AC";
            break;
    case CC: return "CC";
            break;
    case CS: return "CS";
            break;
    case DS: return "DS";
            break;
    default: "registro no encontrado";
            break;
    }
}



void imprimir_operador(int op, int Toperando){
    switch (Toperando){
    case 1: printf("%s", identificarRegistro(op & 0x00FFFFFF));
            break;
    case 2: printf("%d", op & 0x00FFFFFF);
            break;
    case 3:
                if ((op& 0X0000FFFF) & 0x00008000) // si el bit 15 del offset es 1, es negativo
                    printf("[%s - %d]", identificarRegistro((op >> 16) & 0x0000001F), ((~(op & 0x0000FFFF) + 1) & 0x0000FFFF));
                else 
                    printf("[%s + %d]", identificarRegistro((op >> 16) & 0x0000001F), (op & 0x0000FFFF)); // lo extiendo a 32 bits y le saco el signo
            break;
    }
}
void escribirInstruccion(MaquinaVirtual *mv,int opA, int opB,int ToperandoA, int ToperandoB, char instruccion, int direccion, int operacion){

    printf("[%04x]", direccion);
    printf(" %02X  %02X  %02X  %02X  %02X  %02X  %02X    ", (instruccion & 0x000000FF),(opA>>16)&0x000000FF,(opA>>8)&0x000000FF,(opA)&0x000000FF, (opB>>16)&0x000000FF, (opB>>8)&0x000000FF, (opB)&0x000000FF); //Imprime instruccion y tipos de operandos
    printf("\t|%s", identificarMnemonico(operacion));
    printf("\t");


    if (ToperandoA>0 && ToperandoB>0){
        imprimir_operador(opA,ToperandoA);
        printf("\t");
        imprimir_operador(opB,ToperandoB);
    }
    else
        if (ToperandoA>0){
            imprimir_operador(opA,ToperandoA);
    }

}

void lee_operandos2(int topA, int topB, MaquinaVirtual *mv, int *ip){
    int i;
    mv->registros[OP1] = 0;
    mv->registros[OP2] = 0;
    for(i = ((*ip)+1); i < ((*ip) + topB+1); i++){
        mv->registros[OP2] = mv->registros[OP2] << 8;
        mv->registros[OP2] += mv->ram[i];
    }
    (*ip) += topB;

    for(i = ((*ip)+1); i < ((*ip) + topA+1); i++){
        mv->registros[OP1] = mv->registros[OP1] << 8;
        mv->registros[OP1] += mv->ram[i];
    }
    (*ip) += topA;
    (*ip)++; // avanzo el ip al proximo byte de instruccion porque sino queda en el ultimo operando
    //agrego el tipo de operando en el byte mas significativo
    mv->registros[OP1] += (topA << 24);
    mv->registros[OP2] += (topB << 24);
}
//extraido del codigo del step
/*
if (flag_disassembler){
        escribirInstruccion(mv, instruccion, opA,opB,ToperandoA,ToperandoB);
}
*/


void disassembler(MaquinaVirtual *mv, short int tamSeg){
    int ToperandoA,ToperandoB,operacion, ip;
    ip = mv->registros[IP];
    while (ip < tamSeg && operacion != STOP){
        char instruccion = mv->ram[ip];
        int dir = ip;
        procesaOperacion(instruccion,&ToperandoA,&ToperandoB,&operacion); // desarma la instruccion codificada
        lee_operandos2(ToperandoA,ToperandoB,mv, &ip); // lee los siguientes bytes de los operandos A y B y mueve el ip
        escribirInstruccion(mv,mv->registros[OP1],mv->registros[OP2],ToperandoA,ToperandoB, instruccion, dir, operacion);
        printf("\n");
    }
}

//fin dissasembler