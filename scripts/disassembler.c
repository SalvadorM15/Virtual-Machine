#include <stdio.h>
#include <stdlib.h>
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
    case PUSH: return "PUSH";
            break;
    case POP: return "POP"; 
            break;
    case CALL: return "CALL";
            break;
    case RET: return "RET";
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

const char *identificaAreaReg(char reg[], int op){
        int area = (op >> 6) & 0x00000003;
        char *salida = malloc(4*sizeof(char));
        switch(area){
                case 0:
                        strcpy(salida, reg);
                break;
                case 1:
                        strcat(salida, &(reg[1]));
                        strcat(salida, &(reg[2]));
                break;
                case 2:
                        strcat(salida, &(reg[1]));
                        strcat(salida, "H");
                break;
                case 3:
                        strcat(salida, &(reg[1]));
                        strcat(salida, "X");
                break;
        }
        return salida;
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
        case EAX: return identificaAreaReg("EAX", op);
                break;
        case EBX: return identificaAreaReg("EBX", op);
                break;
        case ECX: return identificaAreaReg("ECX", op);
                break;
        case EDX: return identificaAreaReg("EDX", op);
                break;
        case EFX: return identificaAreaReg("EFX", op);
                break;
        case EEX: return identificaAreaReg("EEX", op);
                break;
        case AC: return "AC";
                break;
        case CC: return "CC";
                break;
        case CS: return "CS";
                break;
        case DS: return "DS";
                break;
        case ES: return "ES";
                break;
        case PS: return "PS";
                break;
        case SS: return "SS";
                break;
        case KS: return "KS";
                break;
        case SP: return "SP";
                break;
        case BP: return "BP";
                break;
        default: return "registro no encontrado";
                break;
    }
}

void evaluaTamanio(int op){
        switch((op >> 22) & 0x00000003){
                case 2: printf("w");
                break;
                case 3: printf("b");
                break;
        }
}

void imprimir_operador(int op, int Toperando){
    switch (Toperando){
    case 1: printf("%s", identificarRegistro(op & 0x00FFFFFF));
            break;
    case 2: if(op & 0x00800000) // si el bit 23 del inmediato es 1, es negativo
                printf("%d", op|0xFFFF0000);
            else
                printf("%d", op & 0x00FFFFFF);
            break;
    case 3:
                evaluaTamanio(op);
                if ((op& 0X0000FFFF) & 0x00008000) // si el bit 15 del offset es 1, es negativo
                    printf("[%s - %d]", identificarRegistro((op >> 16) & 0x0000001F), ((~(op & 0x0000FFFF) + 1) & 0x0000FFFF));
                else 
                    printf("[%s + %d]", identificarRegistro((op >> 16) & 0x0000001F), (op & 0x0000FFFF)); // lo extiendo a 32 bits y le saco el signo
            break;
    }
}
void escribirInstruccion(MaquinaVirtual *mv,int opA, int opB,int ToperandoA, int ToperandoB, char instruccion, int direccion, int operacion){
    printf(" [%04x]", direccion);
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


void lee_operandos2(int topA, int topB, MaquinaVirtual *mv, short int *ip){
    int i;
    mv->registros[OP1] = 0;
    mv->registros[OP2] = 0;
    for(i = ((*ip)+1); i < ((*ip) + topB+1); i++){
        mv->registros[OP2] = ((mv->registros[OP2]) << 8);
        mv->registros[OP2] |= (mv->ram[i])&0x000000FF;
    }
    if(topB == 2){
        if(mv->registros[OP2] & 0x00008000) // si el bit 15 del inmediato es 1, es negativo
            mv->registros[OP2] = mv->registros[OP2] | 0x00FF0000; // lo extiendo a 32 bits
    }
    (*ip) += topB;


    for(i = ((*ip)+1); i < ((*ip) + topA+1); i++){
        mv->registros[OP1] = ((mv->registros[OP1]) << 8);
        mv->registros[OP1] |= mv->ram[i] & 0x000000FF;
    }   
    if(topA == 2){
        if(mv->registros[OP1] & 0x00008000) // si el bit 15 del inmediato es 1, es negativo
            mv->registros[OP1] = mv->registros[OP1] | 0x00FF0000; // lo extiendo a 32 bits
    }
    (*ip) += topA;
    (*ip)++; // avanzo el ip al proximo byte de instruccion porque sino queda en el ultimo operando
    //agrego el tipo de operando en el byte mas significativo
    mv->registros[OP1] += (topA << 24)&0x03000000;
    mv->registros[OP2] += (topB << 24)&0x03000000;
}
//extraido del codigo del step
/*
if (flag_disassembler){
        escribirInstruccion(mv, instruccion, opA,opB,ToperandoA,ToperandoB);
}
*/

void disassemblerConstantes(MaquinaVirtual *mv, short int tamSeg, short int inicio){
     short int i=0;
     char cadena[100] = "";
     int len=0;
     while (i< tamSeg){
        printf("  [%04x]: ", i + inicio);
        char c = mv->ram[i + inicio];
        len= 0;
        while (c!=0 && i<tamSeg){
            if (len<6){
                printf("%02X ", mv->ram[inicio + i]&0x000000FF);
            }
            else if (len==6){
                printf(".. ");
            }
            if (c >=32 && c <=126)
                cadena[len]=c;
            else
                cadena[len]='.';
            len++;
            i++;
            c = mv->ram[inicio + i];
        }
        i++;
        if (len<6){
        printf("%02X ", mv->ram[inicio + i]&0x000000FF); // imprimo el caracter nulo
        len++;
        }
        else if (len==6){
            printf(".. ");
        }
        cadena[len]='\0';
        /*
        for(int j = 0; j < len; j++){
                printf("%c", cadena[j]);
        }
        */
        printf("|   \"%s\"\n", cadena);
     }   
}

void disassembler(MaquinaVirtual *mv, short int tamSeg){
    int ToperandoA,ToperandoB,operacion;
    short int ip;
    ip = mv->seg[mv->registros[CS] >> 16][0];
    while (ip < mv->seg[mv->registros[CS] >> 16][1]){
        char instruccion = mv->ram[ip];
        int dir = ip;
        if(ip == mv->registros[IP])
                printf(">");
        else
                printf(" ");
        procesaOperacion(instruccion,&ToperandoA,&ToperandoB,&operacion); // desarma la instruccion codificada
        //HASTA ACA TODO BIEN
        lee_operandos2(ToperandoA,ToperandoB,mv, &ip); // lee los siguientes bytes de los operandos A y B y mueve el ip
        // EL ERROR ESTA EN LEE OPERANDOS 2.
        escribirInstruccion(mv,mv->registros[OP1],mv->registros[OP2],ToperandoA,ToperandoB, instruccion, dir, operacion);
        printf("\n");
    }
}


//fin dissasembler.c