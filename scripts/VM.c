#include <stdio.h>
#include "VM.h"


void main(){

}


//-------------------------------------------------SECCION DE MNEMONICOS---------------------------------------------------------------------------------

//instrucciones de 2 operandos

//la idea del mov es que le pase el operandoA y el valor del operando b, el valor del operando b se consigue con los getters de valor antes de llamar a la funcion

void mov(int opa , int opb , MaquinaVirtual mv , int Toperando){

if(Toperando == 1){ // es un registro
mv.registros[opa] = opb;
}
else{ // es un espacio de memoria porque no se puede asignar a un inmediato
set_valor_mem(opa, opb, mv);
}
}

void add(int opa, int opb, MaquinaVirtual mv , int Toperando){

if(Toperando == 1){ // es un registro
mv.registros[opa]+= opb;
}
else{ // es un espacio de memoria
set_valor_mem(opa, get_valor_mem(opa,mv) + opb, mv);
}

}


void sub(int opa , int opb, MaquinaVirtual mv, int Toperando){

add(opa,-1*opb,mv,Toperando); // es lo mismo que sumar el negado


}

void mul(int opa, int opb, MaquinaVirtual mv, int Toperando){

if(Toperando == 1){ // es un registro
mv.registros[opa] = mv.registros[opa]*opb;
}
else{ // es un espacio de memoria
int aux = get_valor_mem(opa,mv);
for(int i = 0; i<opb ; i++) //sumo opb cantidad de veces el valor de opa
set_valor_mem(opa, get_valor_mem(opa,mv)+aux, mv);
}
}

void div(int opa, int opb, MaquinaVirtual mv){


}



// continua...







//---------------------------------------------FIN MNEMONICOS----------------------------------------------------------------------------------------------


//funciones utiles para resolver otros problemas


int get_logical_dir(MaquinaVirtual mv, int operandoM){
    int segmento = mv.registros[(operandoM >> 16)];
    int offset = operandoM & 0x0000FFFF;
    int direccion = (segmento << 16)+offset;

    return direccion;
}

int logical_to_physical(int logical_dir ,int seg_table[MAX][2], int cant_bytes){
    int physical_dir;
    int segment = (logical_dir & 0xFFFF0000) >> 16;
    int segment_limit = seg_table[segment][0] + seg_table[segment][1];

    if(segment < MAX){
        physical_dir = seg_table[segment][0];
        physical_dir += (logical_dir & 0x0000FFFF);
        if(physical_dir > segment_limit || physical_dir + cant_bytes > segment_limit)
        physical_dir = -1;
    }
    else
    physical_dir = -1;


    return physical_dir;
}



int get_valor_reg(int operandoR, MaquinaVirtual mv){

return mv.registros[operandoR];

}

int get_valor_mem(int operandoM, MaquinaVirtual mv){

// busco la direccion logica

int direccion = get_logical_dir(mv, operandoM);

// paso la direccion logica a fisica

direccion = logical_to_physical(direccion , mv.seg , MEM);
if(direccion == -1){
    //aca deberia tirar algun error de segmentation foult
}
else{
    return mv.ram[direccion];
}
}

void set_valor_mem(int operandoM, int valor, MaquinaVirtual mv){
    // busco la direccion logica

    int direccion = get_logical_dir(mv, operandoM);

    // paso la direccion logica a fisica

    direccion = logical_to_physical(direccion , mv.seg , MEM);

    if(direccion == -1){
        //devuelve error de segmentation foul
    }
    else{
        mv.ram[direccion] = valor;
    }
}





// Funciones para leer instrucciones

void procesaOperacion(int instruccion, int *topA, int *topB, int *op){

    *topB = (instruccion & 0x000000C0) >> 6;
    *topA = (instruccion & 0x00000030) >> 4;
    *op = (instruccion & 0x0000001F);
}

void imprimirBinarioCompacto(int n) {
    if (n == 0) {
        printf("0\n");
        return;
    }

    int inicio = 0;
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        if ((n >> i) & 1) inicio = 1; // cuando encuentra el primer 1, empieza a imprimir
        if (inicio) printf("%d", (n >> i) & 1);
    }
    printf("\n");
}
