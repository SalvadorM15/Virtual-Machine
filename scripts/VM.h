#define REG 32
#define MAXMEM 16384
#define MAX 6
#define VERSION 1

#define LAR 0
#define MAR 1
#define MBR 2
#define IP 3
#define OPC 4
#define OP1 5
#define OP2 6
#define SP 7
#define BP 8
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
#define ES 28
#define SS 29
#define KS 30
#define PS 31

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
#define PUSH 11
#define POP 12
#define CALL 13
#define RET 14
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
#define MEMINS 4
#define STACKOVER 5
#define STACKUNDER 6
#define NOFILE 7

typedef struct{
    int registros[REG];
    char  ram[MAXMEM];
    short int seg[8][2];
    char vmiFileName[20];
    int MemSize;
 } MaquinaVirtual;





//prototipos VM.C

void mov(int opa, int opb, MaquinaVirtual *mv);
void add(int opa, int opb, MaquinaVirtual *mv);
void sub(int opa, int opb, MaquinaVirtual *mv);
void mul(int opa, int opb, MaquinaVirtual *mv);
void div_op(int opa, int opb, MaquinaVirtual *mv);
void cmp(int opa, int opb, MaquinaVirtual *mv);
void shl(int opa, int opb, MaquinaVirtual *mv);
void shr(int opa, int opb, MaquinaVirtual *mv);
void and(int opa, int opb, MaquinaVirtual *mv);
void or(int opa, int opb, MaquinaVirtual *mv);
void xor(int opa, int opb, MaquinaVirtual *mv);
void swap(int opa, int opb, MaquinaVirtual *mv);
void ldh(int opa, int opb, MaquinaVirtual *mv);
void ldl(int opa, int opb, MaquinaVirtual *mv);
void rnd(int opa, int opb, MaquinaVirtual *mv);

void jmp(int op, MaquinaVirtual *mv);
void jz(int op, MaquinaVirtual *mv);
void jp(int op, MaquinaVirtual *mv);
void jnz(int op, MaquinaVirtual *mv);
void jnp(int op, MaquinaVirtual *mv);
void jnn(int op, MaquinaVirtual *mv);
void not(int op, MaquinaVirtual *mv);
void jn_op(int op, MaquinaVirtual *mv);

int get_logical_dir(MaquinaVirtual mv, int operandoM);
int logical_to_physical(int logical_dir, short int seg_table[MAX][2], int cant_bytes);
int get_valor_mem(int operandoM, MaquinaVirtual *mv, int cant_bytes);

void set_valor_mem(int operandoM, int valor, MaquinaVirtual *mv, int cant_bytes);
void evaluarCC(int res, MaquinaVirtual *mv);

void procesaOperacion(char instruccion, int *topA, int *topB, int *op);
void lee_operandos(int topA, int topB, MaquinaVirtual *mv);

void imprimirBinarioCompacto(int n);

void instruction_handler(int opA, int opB, int operacion, MaquinaVirtual *mv);
void error_handler(int error);

void lectura_arch(MaquinaVirtual *mv, char nombre_arch[], unsigned short int *codeSeg, unsigned short int *dataSeg, unsigned short int *extraSeg, unsigned short int *stackSeg, unsigned short int *constSeg, unsigned short int *offsetEP, int *version);
void iniciaMV(MaquinaVirtual *mv, unsigned short int codeSeg,unsigned short int dataSeg,unsigned short int extraSeg,unsigned short int stackSeg,unsigned short int constSeg,unsigned short int paramSeg, int offsetEP);
void creaTablaSegmentos(MaquinaVirtual *mv,int param, int code, int data, int extra, int stack, int constant);
void step(MaquinaVirtual *mv);
void leerBinario(int *entrada);

const char* identificarMnemonico(int codigo);
const char* identificarRegistro(int codigo);
void imprimir_operador(int op, int Toperando);
void escribirInstruccion(MaquinaVirtual *mv,int opA, int opB,int ToperandoA, int ToperandoB, char instruccion, int direccion, int operacion);
void lee_operandos2(int topA, int topB, MaquinaVirtual *mv, int *ip);
void disassembler(MaquinaVirtual *mv, short int tamSeg);

void set_valor_operando(int operando, int valor, MaquinaVirtual *mv);
int get_valor_operando(int operando, MaquinaVirtual *mv);
void manejaArgumentos(int argc, char *argv[], char vmx[], char vmi[], int *d, int *p, int *argcMV, int argvMV[], unsigned short int *param, MaquinaVirtual *mv);
void breakPoint(MaquinaVirtual *mv, char vmiFileName[]);

void leeImg(MaquinaVirtual *mv, char vmi[]);
void leeHeaderImg(char *version, int *tamMem, char vmi[]);
void leeRegistrosImg(MaquinaVirtual *mv, char vmi[]);
void leeTablaSegmentosImg(MaquinaVirtual *mv, char vmi[]);
void leeMemoriaImg(MaquinaVirtual *mv, char vmi[], int tamMem);

void escribeImg(MaquinaVirtual mv, char vmi[], char version, short int tamMem);
void escribeHeaderImg(char version, short int tamMem, char vmi[]);
void escribeRegistrosImg(MaquinaVirtual mv, char vmi[]);
void escribeTablaSegImg(MaquinaVirtual mv, char vmi[]);
void escribeMemoriaImg(MaquinaVirtual mv, short int tamMem, char vmi[]);
void breakPoint(MaquinaVirtual *mv, char vmiFileName[]);
void iniciaPila(MaquinaVirtual *mv, int argc, int argv[]);

//fin vm.h