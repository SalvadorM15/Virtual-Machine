#include<stdio.h>
#include "VM.h"
#include<stdlib.h>
#include <string.h>




char* identificarMnemonico(int codigo){
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

char* identificarRegistro(int op){
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
            printf("[%s + %d]", identificarRegistro((op >> 16) & 0x000000FF), (op & 0x0000FFFF)); 
            break;
    }
}
void escribirInstruccion(MaquinaVirtual *mv,int opA, int opB,int ToperandoA, int ToperandoB, char instruccion, int direccion){
    printf("[%x]", direccion);
    printf(" %x", instruccion); //Imprime instruccion y tipos de operandos
    printf("\t|%s", identificarMnemonico(mv->registros[OPC]));
    printf("\t%x", opA & 0x00FFFFFF); //Imprime operando A
    printf("\t%x", opB & 0x00FFFFFF); //Imprime operando B

    if (ToperandoA>0 && ToperandoB>0){
        imprimir_operador(opA,ToperandoA);
        imprimir_operador(opB,ToperandoB);
    }
    else
        if (ToperandoA>0){
            imprimir_operador(opA,ToperandoA);
    }

}

//extraido del codigo del step
/*
if (flag_disassembler){
        escribirInstruccion(mv, instruccion, opA,opB,ToperandoA,ToperandoB);
}
*/


void disassembler(MaquinaVirtual *mv, short int tamSeg){
    int ToperandoA,ToperandoB,operacion;

    while (mv->registros[IP] < tamSeg){
        char instruccion = mv->ram[mv->registros[IP]];
        int dir = mv->registros[IP];
        procesaOperacion(instruccion,&ToperandoA,&ToperandoB,&operacion); // desarma la instruccion codificada
        lee_operandos(ToperandoA,ToperandoB,mv); // lee los siguientes bytes de los operandos A y B y mueve el ip
        escribirInstruccion(mv,mv->registros[OP1],mv->registros[OP2],ToperandoA,ToperandoB, instruccion, dir);
        printf("\n");
    }
}