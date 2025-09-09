
#define REG 32
#define MEM 16384
#define MAX 2
#define ARCHNAME "program.vmx"
#define VERSION 1

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

#define SYS 0
#define JMP 1
#define JZ 2
#define JP 3
#define JN 4
#define JNZ 5
#define JNP 6
#define JNN 7
#define NOT 8
#define STOP 15
#define MOV 16
#define ADD 17
#define SUB 18
#define MUL 19
#define DIV 20
#define CMP 21
#define SHL 22
#define SHR 23
#define SAR 24
#define AND 25
#define OR 26
#define XOR 27
#define SWAP 28
#define LDL 29
#define LDH 30
#define RND 31

// FIN DE LAS CONSTANTES DE OPERACION

#define SEGFAULT 0
#define DIV0 1
#define INVINS 2
#define INVVER 3

typedef struct{
    int registros[REG];
    char  ram[MEM];
    int seg[MAX][2];
 } MaquinaVirtual;
