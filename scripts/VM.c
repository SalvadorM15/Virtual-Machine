#include <stdio.h>
#include "VM.h"


void main(){

}


//-------------------------------------------------SECCION DE MNEMONICOS---------------------------------------------------------------------------------

//instrucciones de 2 operandos

//la idea del mov es que le pase el operandoA y el valor del operando b, el valor del operando b se consigue con los getters de valor antes de llamar a la funcion

void mov(int opa , int opb , MaquinaVirtual *mv , int Toperando){

<<<<<<< HEAD
    if(Toperando == 1){ // es un registro
        mv->registros[opa] = opb;
    }
    else{ // es un espacio de memoria porque no va a asignar a un inmediato
        set_valor_mem(opa, opb, mv);
    }
}

void add(int opa, int opb, MaquinaVirtual *mv , int Toperando){
    
    int res;
    if(Toperando == 1){ // es un registro
        mv->registros[opa]+= opb;
        res = mv->registros[opa];
    }
    else{ // es un espacio de memoria
        res = get_valor_mem(opa,mv) + opb;
        set_valor_mem(opa, res , mv);
    }
    evaluarCC(res,mv);
=======
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

>>>>>>> ed9915bf1a66564e655b3183492e4f678080a377
}


void sub(int opa , int opb, MaquinaVirtual *mv, int Toperando){

add(opa,-1*opb,mv,Toperando); // es lo mismo que sumar el negado


}

<<<<<<< HEAD
void mul(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    
    if(Toperando == 1){ // es un registro
        mv->registros[opa] = mv->registros[opa]*opb;
    }
    else{ // es un espacio de memoria
        int aux = get_valor_mem(opa,mv);
        for(int i = 0; i<opb ; i++) //sumo opb cantidad de veces el valor de opa
            set_valor_mem(opa, get_valor_mem(opa,mv)+aux, mv);
    }
=======
void mul(int opa, int opb, MaquinaVirtual mv, int Toperando){

if(Toperando == 1){ // es un registro
mv.registros[opa] = mv.registros[opa]*opb;
}
else{ // es un espacio de memoria
int aux = get_valor_mem(opa,mv);
for(int i = 0; i<opb ; i++) //sumo opb cantidad de veces el valor de opa
set_valor_mem(opa, get_valor_mem(opa,mv)+aux, mv);
}
>>>>>>> ed9915bf1a66564e655b3183492e4f678080a377
}

void div(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    if (opb!=0){
        int cociente, resto;
        if (Toperando ==1){
            cociente = mv->registros[opa]/opb;
            resto = mv->registros[opa]%opb;
            mv->registros[opa]=cociente;
            mv->registros[AC]=resto;
        }
        else {
            int aux = get_valor_mem(opa,mv);
            cociente = v/opb;
            resto = v%opb;
            set_valor_mem(opa,cociente,mv);
            mv->registros[AC]=resto;
        }
        evaluarCC(cociente,mv);
    }
    else {
        //DIVISION POR CERO
    }
}

void cmp(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    
    int res;
    if (Toperando == 1){
        res = mv->registros[opa]-opb;
    }
    else {
        int aux = get_valor_mem(opa,mv);
        res = aux - opb;
    }
    evaluarCC(res, mv);
}

void shl(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (opb>=0  && opb<32 ) {
        if (Toperando==1){
            aux = mv->registros[opa]<<opb;
            mv->registros[opa] = aux;
        }
        else {
            aux = get_valor_mem(opa,mv);
            aux = aux << opb;
            set_valor_mem(opa,aux,mv);
        }
        evaluarCC(aux,mv);
    }
    else {
        //ERROR DE VALIDACION OPB
    }
}

void shr(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    if (opb>=0 && opb<32){
        if (Toperando==1){
            aux = mv->registros[opa]>>opb;
            mv->registros[opa] = aux;
        }
        else {
            int aux = get_valor_mem(opa,mv);
            aux = aux>>opb;
            set_valor_mem(opa,aux,mv);
        }
        evaluarCC(aux,mv);
    }
    else {
        //ERROR DE VALIDACION OPB
    }
}

void AND(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando==1){
        mv->registros[opa]&=opb;
        aux = mv->registros[opa];
    }
    else {
        aux = get_valor_mem(opa,mv);
        aux &= opb;
        set_valor_mem(opa,aux,mv);
    }
    evaluarCC(aux,mv);
}
    
void OR(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando==1){
        mv->registros[opa]|=opb;
        aux = mv->registros[opa];
    }
    else {
        aux = get_valor_mem(opa,mv);
        aux |= opb;
        set_valor_mem(opa,aux,mv);
    }
    evaluarCC(aux,mv);
}

void XOR(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando==1){
        mv->registros[opa]^=opb;
        aux = mv->registros[opa];
    }
    else {
        aux = get_valor_mem(opa,mv);
        aux ^= opb;
        set_valor_mem(opa,aux,mv);
    }
    evaluarCC(aux,mv);
}

void SWAP(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux=opb;
    if (Toperando==1){
        opb=mv->registros[opa];
        mv->registros[opa]=aux;
    }
    else {
        opb=get_valor_mem(opa,mv);
        set_valor_mem(opa,aux,mv);
    }
}

void JMP(int op, MaquinaVirtual *mv, int Toperando){
    int dir;
    if (Toperando == 1)
        dir = mv->registros[op];
    else
        dir = get_valor_mem(op,mv);
    mv->registros[IP]=dir;
}  

void JZ(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC] & 0x01)
        JMP(op,mv,Toperando);
}

void JP(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC]==0x00)
        JMP(op,mv,Toperando);
}
void JN(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC] & 0x02)
        JMP(op,mv,Toperando);
}
void JNZ(int op, MaquinaVirtual *mv, int Toperando){
    if (!mv->registros[CC] & 0x01)
        JMP(op,mv,Toperando);
}
void JNP(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC] & 0x03)
        JMP(op,mv,Toperando);
}
void JNN(int op, MaquinaVirtual *mv, int Toperando){
    if (!mv->registros[CC] & 0x02)
        JMP(op,mv,Toperando);
}
void NOT(int op, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando == 1){
        mv->registros[op] = ~mv->registros[op];
        aux = mv->registros[op];
    }
    else { 
        aux = get_valor_mem(op, mv);
        aux = ~aux;
        set_valor_mem(op, aux, mv);
    }
    evaluarCC(aux, mv); 
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

void evaluarCC(int res, MaquinaVirtual *mv){
    int valor = 0;
    if (res==0)
        valor |= (0x01);
        else {
            if (res<0)
                valor |= (0x02);
        }
    mv->registros[CC]=valor;

<<<<<<< HEAD
}
=======



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
>>>>>>> ed9915bf1a66564e655b3183492e4f678080a377
