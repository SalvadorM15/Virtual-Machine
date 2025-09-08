#include<stdio.h>
#include "VM.h"



//-------------------------------------------------SECCION DE MNEMONICOS---------------------------------------------------------------------------------

//instrucciones de 2 operandos

//la idea del mov es que le pase el operandoA y el valor del operando b, el valor del operando b se saca con los getters de valor antes de llamar a la funcion

void mov(int opa , int opb , MaquinaVirtual mv){

    if(sizeof(opa) == 1){ // es un registro
        mv.registros[opa] = opb;
    }
    else{ // es un espacio de memoria porque no va a asignar a un inmediato
        set_valor_mem(opa, opb, mv);
    }
}

void add(int opa, int opb, MaquinaVirtual mv){
    
    if(sizeof(opa) == 1){ // es un registro
        mv.registros[opa]+= opb;
    }
    else{ // es un espacio de memoria
        set_valor_mem(opa, get_valor_mem(opa,mv) + opb, mv);
    }

}


void sub(int opa , int opb, MaquinaVirtual mv){

    add(opa,-opb,mv); // es lo mismo que sumar el negado
}

void mul(int opa, int opb, MaquinaVirtual mv){
    
    if(sizeof(opa) == 1){ // es un registro
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
    int segmento = mv.registros[(operandoM >> 16)];
    int offset = operandoM & 0x0000FFFF;
    int direccion = (segmento << 16)+offset;

    // paso la direccion logica a fisica

    direccion = logical_to_physical(direccion , mv.seg , MEM);
    if(direccion == -1){
        //aca deberia tirar algun error de segmentation foul
    }
    else{
        return mv.ram[direccion];
    }
}

void set_valor_mem(int operandoM, int valor, MaquinaVirtual mv){
    // busco la direccion logica
    int segmento = mv.registros[(operandoM >> 16)];
    int offset = operandoM & 0x0000FFFF;
    int direccion = (segmento << 16)+offset;

    // paso la direccion logica a fisica

    direccion = logical_to_physical(direccion , mv.seg , MEM);

    if(direccion == -1){
        //devuelve error de segmentation foul
    }
    else{
        mv.ram[direccion] = valor;
    }
}
