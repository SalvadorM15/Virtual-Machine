#include <stdio.h>
#define REG 32
#define MEM 16384
#define MAX 2

#define LAR 0
#define MAR 1
#define MBR 2
#define IP 3
#define OPC 4
#define OP1 5
#define OP2 6
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15
#define AC 16
#define CC 17
#define CS 26
#define DS 27

//FIN DE LAS CONSTANTES DE REGISTROS

typedef struct{
    int registros[REG];
    char  ram[MEM];
    int seg[MAX][2];
 } MaquinaVirtual;
