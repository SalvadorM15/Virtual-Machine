#include "VM.h"
#include <stdio.h>




void main(){

}


//-------------------------------------------------SECCION DE MNEMONICOS---------------------------------------------------------------------------------

//instrucciones de 2 operandos

//la idea del mov es que le pase el operandoA y el valor del operando b, el valor del operando b se consigue con los getters de valor antes de llamar a la funcion

void mov(int opa , int opb , MaquinaVirtual *mv , int Toperando){

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
        res = get_valor_mem(opa,*mv) + opb;
        set_valor_mem(opa, res , mv);
    }
    evaluarCC(res,mv);
}


void sub(int opa , int opb, MaquinaVirtual *mv, int Toperando){

    add(opa,-1*opb,mv,Toperando); // es lo mismo que sumar el negado

}

void mul(int opa, int opb, MaquinaVirtual *mv, int Toperando){

    if(Toperando == 1){ // es un registro
        mv->registros[opa] = mv->registros[opa]*opb;
    }
    else{ // es un espacio de memoria
        int res = get_valor_mem(opa,*mv);
        res = res*opb;
        set_valor_mem(opa,res,mv);
    }
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
            int v;
            int aux = get_valor_mem(opa,*mv);
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
        int aux = get_valor_mem(opa,*mv);
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
            aux = get_valor_mem(opa,*mv);
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
        int aux;
        if (Toperando==1){
            aux = mv->registros[opa]>>opb;
            mv->registros[opa] = aux;
        }
        else {
            int aux = get_valor_mem(opa,*mv);
            aux = aux>>opb;
            set_valor_mem(opa,aux,mv);
        }
        evaluarCC(aux,mv);
    }
    else {
        //ERROR DE VALIDACION OPB
    }
}

void and(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando==1){
        mv->registros[opa]&=opb;
        aux = mv->registros[opa];
    }
    else {
        aux = get_valor_mem(opa,*mv);
        aux &= opb;
        set_valor_mem(opa,aux,mv);
    }
    evaluarCC(aux,mv);
}

void or(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando==1){
        mv->registros[opa]|=opb;
        aux = mv->registros[opa];
    }
    else {
        aux = get_valor_mem(opa,*mv);
        aux |= opb;
        set_valor_mem(opa,aux,mv);
    }
    evaluarCC(aux,mv);
}

void xor(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando==1){
        mv->registros[opa]^=opb;
        aux = mv->registros[opa];
    }
    else {
        aux = get_valor_mem(opa,*mv);
        aux ^= opb;
        set_valor_mem(opa,aux,mv);
    }
    evaluarCC(aux,mv);
}

void swap(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int aux=opb;
    if (Toperando==1){
        opb=mv->registros[opa];
        mv->registros[opa]=aux;
    }
    else {
        opb=get_valor_mem(opa,*mv);
        set_valor_mem(opa,aux,mv);
    }
}

//fin instrucciones de 2 operandos

//instrucciones de 1 operando


void jmp(int op, MaquinaVirtual *mv, int Toperando){
    int dir;
    if (Toperando == 1)
        dir = mv->registros[op];
    else
        dir = get_valor_mem(op,*mv);
    mv->registros[IP]=dir;
}

void jz(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC] & 0x01)
        jmp(op,mv,Toperando);
}

void jp(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC]==0x00)
        jmp(op,mv,Toperando);
}
void jn(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC] & 0x02)
        jmp(op,mv,Toperando);
}
void jnz(int op, MaquinaVirtual *mv, int Toperando){
    if (!mv->registros[CC] & 0x01)
        jmp(op,mv,Toperando);
}
void jnp(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC] & 0x03)
        jmp(op,mv,Toperando);
}
void jnn(int op, MaquinaVirtual *mv, int Toperando){
    if (!mv->registros[CC] & 0x02)
        jmp(op,mv,Toperando);
}
void not(int op, MaquinaVirtual *mv, int Toperando){
    int aux;
    if (Toperando == 1){
        mv->registros[op] = ~mv->registros[op];
        aux = mv->registros[op];
    }
    else {
        aux = get_valor_mem(op, *mv);
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
    return mv.ram[direccion] + (mv.ram[direccion+1] >> 16) + (mv.ram[direccion+2] >> 32) + (mv.ram[direccion+3] >> 64);
}
}

void set_valor_mem(int operandoM, int valor, MaquinaVirtual *mv){
    // busco la direccion logica

    int direccion = get_logical_dir(*mv, operandoM);

    // paso la direccion logica a fisica

    direccion = logical_to_physical(direccion , mv->seg , MEM);

    if(direccion == -1){
        //devuelve error de segmentation foul
    }
    else{
        // 1er byte:
        mv->ram[direccion] = valor & 0xFF000000;
        // 2do byte:
        mv->ram[direccion + 1] = (valor & 0x00FF0000)<<16;
        // 3er byte:
        mv->ram[direccion+2] = (valor & 0x0000FF00)<<32;
        // 4to byte:
        mv->ram[direccion+3] = (valor & 0x000000FF)<<64;

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

}



// Funciones para leer instrucciones

void procesaOperacion(char instruccion, int *topA, int *topB, int *op){

    *topB = (int)(instruccion & 0x000000C0) >> 6;
    *topA = (int)(instruccion & 0x00000030) >> 4;
    *op = (int)(instruccion & 0x0000001F);
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


void instruction_handler(int opA, int opB, int operacion){ // seguimos con la idea de pasar el opb por el valor que tiene y no como operando en si

    switch (operacion)
    {
    case MOV:

        break;
    case ADD:

        break;
    default:
        break;
    }
}

//handler a terminar tuve un problema con el compilador

void error_handler(int error){

    switch(error){
    case SEGFAULT:
        printf("ERROR: SEGMENTATION FAULT");
        break;
    case DIV0:
        printf("ERROR: DIVISION POR 0 NO ESTA PERMITIDA");
        break;
    case INVINS:
        printf("ERROR: INSTRUCCION INVALIDA");
    case INVVER:
        printf("ERROR: VERSION INVALIDA");

    }

    exit(1);
}

void lectura_arch(MaquinaVirtual *mv){
    FILE *arch;
    char num;
    int i;
    short int tamSeg;

    arch = fopen(ARCHNAME, "rb");
    if(arch != NULL){

        fread(&num, sizeof(char), 1, arch);
        i = 0;
        while(!feof(arch) && i<5){
            printf("%c", num);
            fread(&num, sizeof(char), 1, arch);
            i++;
        }

        if(!feof(arch)){
            if(num != VERSION)
                error_handler(INVVER);
            else{
                fread(&tamSeg, sizeof(short int), 1, arch);
                if(!feof(arch))
                    (mv->seg)[0][1] = tamSeg;

            }

        }

        i = 0;
        if(!feof(arch)){
            fread(&num, sizeof(char), 1, arch);
            while(!feof(arch) && i < tamSeg){
                (mv->ram)[i] = num;
                i++;
                fread(&num, sizeof(char), 1, arch);
            }

            if(i == tamSeg){
                error_handler(SEGFAULT);
            }
        }

        fclose(arch);
    }

}


void iniciarMV(MaquinaVirtual *mv, int codeSegment){

    mv->seg[0][0] = mv->registros[CS] = 0;; //insico que el codesegment arranca al inicio de la memoria
    mv->seg[0][1] = codeSegment; // indico que el codesegment termina en el valor leido en la cabecera (codeSegment)
    mv->seg[1][0] = mv->registros[DS] = MEM - codeSegment; // indico que el data segment empieza una celda despues del code segment
    mv->seg[1][1] = MEM; // indico que el data segment termina al final de la memoria

    mv->registros[IP] = mv->registros[CS];

}

int get_tipoOperacion(char instruccion){
    int r=0;
    int op = instruccion & 31;
    if(op >= 0 && op<=8)
        r = 1;
    else
        if(op>=16 && op<=31)
            r = 2;

    return r;
}
void step(MaquinaVirtual *mv){

    //primero leo la instruccion apuntada por el IP
    int ToperandoA,ToperandoB;
    char instruccion = mv->ram[mv->registros[IP]];
    int opA;
    int Toperacion = get_tipoOperacion(instruccion);

    if(Toperacion == 1){
        
    }
    //dependiendo del valor de los operandos que tipo de operacion es 1operando , 2 operandos o stop

    
    

}
