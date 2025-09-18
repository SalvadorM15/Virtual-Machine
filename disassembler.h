#include "VM.h"

const char* identificarMnemonico(int codigo);
const char* identificarRegistro(int codigo);
void imprimir_operador(int op, int Toperando);
void escribirInstruccion(MaquinaVirtual *mv,int opA, int opB,int ToperandoA, int ToperandoB, char instruccion, int direccion);
void lee_operandos2(int topA, int topB, MaquinaVirtual *mv, int *ip);
void disassembler(MaquinaVirtual *mv, short int tamSeg);