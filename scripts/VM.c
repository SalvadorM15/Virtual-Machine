#include "VM.h"
#include <stdio.h>
#include <time.h>



void main(){
    srand(time(NULL));
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
        res = get_valor_mem(opa,mv) + opb;
        set_valor_mem(opa, res , mv);
    }
    evaluarCC(res,mv);
}


void sub(int opa , int opb, MaquinaVirtual *mv, int Toperando){

    add(opa,-1*opb,mv,Toperando); // es lo mismo que sumar el negado

}

void mul(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int res;
    if(Toperando == 1){ // es un registro
        res = mv->registros[opa]*opb;
        mv->registros[opa] = res;
    }
    else{ // es un espacio de memoria
        res = get_valor_mem(opa,mv);
        res = res*opb;
        set_valor_mem(opa,res,mv);
    }
    evaluarCC(res,mv);
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
        error_handler(DIV0);
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
        int aux;
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

void and(int opa, int opb, MaquinaVirtual *mv, int Toperando){
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

void or(int opa, int opb, MaquinaVirtual *mv, int Toperando){
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

void xor(int opa, int opb, MaquinaVirtual *mv, int Toperando){
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

void swap(int opa, int opb, MaquinaVirtual *mv, int Toperando){
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

void ldh(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    // defino dos variables para construir el valor final del OperandoA
    int parteB = (opb & 0x0000FFFF) << 16; //utilizo una mascara para obtener los dos bytes menos significativos del OPB
    int parteA;
    if (Toperando==1){ 
        parteA=mv->registros[opa]&0x0000FFFF; //utilizo una mascara para obtener los dos bytes menos significativos del OPa
        mv->registros[opa]= parteB | parteA;
    }
    else {
        int aux = get_valor_mem(opa,mv);
        parteA= aux & 0x0000FFFF;
        set_valor_mem(opa, parteA | parteB,mv); 
    }

}

void ldl(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int parteB = (opb & 0x0000FFFF) ;
    int parteA;
    if (Toperando==1){
        parteA =mv->registros[opa] & 0xFFFF0000;
        mv->registros[opa] = parteB | parteA;
    }
    else {
        int aux = get_valor_mem(opa,mv);
        parteA= aux & 0xFFFF0000;
        set_valor_mem(opa, parteA | parteB,mv);
    }
}

void rnd(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    int valor = rand() % (opb+1); // obtengo un valor random entre 0 y el opb
    if (Toperando==1){
        mv->registros[opa]=valor;
    }
    else {
        set_valor_mem(opa,valor,mv);
    }
}

//fin instrucciones de 2 operandos

//instrucciones de 1 operando


void jmp(int op, MaquinaVirtual *mv, int Toperando){
    int dir;
    if (Toperando == 1)
        dir = mv->registros[op];
    else
        dir = get_valor_mem(op,mv);
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

int get_valor_mem(int operandoM, MaquinaVirtual *mv){


    mv->registros[LAR] = get_logical_dir(*mv, operandoM); // busco la direccion logica

    // paso la direccion logica a fisica

    int direccion = logical_to_physical(mv->registros[LAR], mv->seg , MEM);
    mv->registros[MAR] = direccion; // guardo la direccion fisica en los 2 bytes menos significativos
    mv->registros[MAR] +=(3<<32); //quedan los 2 bits mas significativos diciendo que vna a guardar 3 bytes

    if(direccion == -1){
    //aca deberia tirar algun error de segmentation foult
    }
    else{
        mv->registros[MBR] = mv->ram[direccion] + (mv->ram[direccion+1] >> 16) + (mv->ram[direccion+2] >> 32) + (mv->ram[direccion+3] >> 64); 
        return mv->registros[MBR];
    }
}

void set_valor_mem(int operandoM, int valor, MaquinaVirtual *mv){
    // busco la direccion logica

    mv->registros[LAR] = get_logical_dir(*mv, operandoM);
    mv->registros[MBR] = valor;

    // paso la direccion logica a fisica

    int direccion = logical_to_physical(mv->registros[LAR] , mv->seg , MEM);
    mv->registros[MAR] = direccion; // guardo la direccion fisica en los 2 bytes menos significativos
    mv->registros[MAR] +=(3<<32); //quedan los 2 bits mas significativos diciendo que vna a guardar 3 bytes


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

    *op = (int)(instruccion & 0x0000001F);
    if((instruccion & 0x30) != 0){
        *topB = (instruccion >> 6) &0x03;
        *topA = (instruccion >> 4) &0x03;
    }
    else{
        *topB = 0;
        if(instruccion & 0xF0 == 0){
            *topA = 0;
        }
        else{
            *topA = (instruccion >> 6) & 0x03;
        }
    }
}

void lee_operandos(int topA, int topB, MaquinaVirtual *mv){
    int i;
    mv->registros[OP1] = 0;
    mv->registros[OP2] = 0;
    for(i = mv->registros[IP]; i < mv->registros[IP] + topB; i++){
        mv->registros[OP2] = mv->registros[OP2] << 8;
        mv->registros[OP2] += mv->ram[i];
    }
    mv->registros[IP] += topB;

    for(i = mv->registros[IP]; i < mv->registros[IP] + topA; i++){
        mv->registros[OP1] = mv->registros[OP1] << 8;
        mv->registros[OP1] += mv->ram[i];
    }
    mv->registros[IP] += topA;
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


void instruction_handler(int opA, int opB, int operacion, MaquinaVirtual *mv, int ToperandoA){ // seguimos con la idea de pasar el opb por el valor que tiene y no como operando en si

    switch (operacion)
    {
    case MOV:
            mov(opA,opB,mv,ToperandoA);
        break;
    case ADD:
            add(opA,opB,mv,ToperandoA);
        break;
    case SUB:
            sub(opA,opB,mv,ToperandoA);
    case MUL:
            mul(opA,opB,mv,ToperandoA);
    case DIV:
            div(opA,opB,mv,ToperandoA);
    case CMP:
            cmp(opA,opB,mv,ToperandoA);
    case SHL:
            shl(opA,opB,mv,ToperandoA);
    case SHR:
            shr(opA,opB,mv,ToperandoA);
    case SAR:
            sar(); // mnemonico no codeado todavia
    case AND:
            and(opA,opB,mv,ToperandoA);
    case OR:
            or(opA,opB,mv,ToperandoA);
    case XOR:
            xor(opA,opB,mv,ToperandoA);
    case SWAP:
            swap(opA,opB,mv,ToperandoA);
    case LDL:
            ldl(opA,opB,mv,ToperandoA);
    case LDH:
            ldh(opA,opB,mv,ToperandoA);
    case RND:
            rnd(opA,opB,mv,ToperandoA);
    case SYS:
            sys(); // mnemonico no codeado todavia
    case JMP:
            jmp(opA,mv,ToperandoA);
    case JZ:
            jz(opA,mv,ToperandoA);
    case JP:
            jp(opA,mv,ToperandoA);
    case JN:
            jn(opA,mv,ToperandoA);
    case JNZ:
            jnz(opA,mv,ToperandoA);
    case JNP:
            jnp(opA,mv,ToperandoA);
    case JNN:
            jnn(opA,mv,ToperandoA);
    case NOT:
            not(opA,mv,ToperandoA);
    case STOP:
            mv->registros[IP] = -1;
    default:
            error_handler(INVINS);
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

void iniciaMV(MaquinaVirtual *mv, int codSize){ // codsize leido de la cabecera

    //inicio la tabla de segmentos
    mv->seg[0][0] = 0;
    mv->seg[0][1] = codSize;
    mv->seg[1][0] = MEM-codSize;
    mv->seg[1][1] = MEM;
    
    //inicializo los registros de segmento
    mv->registros[CS] = 0; // ya se que no esta bien inicializarlo asi, a preguntar
    mv->registros[DS] = 1<<16;

    //inicializo el ip al principio del codigo

    mv->registros[IP] = mv->registros[CS];

}

void step(MaquinaVirtual *mv){

    //primer paso: leer la instruccion del registro IP

    int ToperandoA,ToperandoB,operacion;
    char instruccion = mv->ram[mv->registros[IP]];

    //leo los valores del cs y muevo el IP

    procesaOperacion(instruccion,&ToperandoA,&ToperandoB,&operacion); // desarma la instruccion codificada
    lee_operandos(ToperandoA,ToperandoB,mv); // lee los siguientes bytes de los operandos A y B y mueve el ip
    mv->registros[OPC] = operacion;

    //lo valores operando 1 y 2 quedan guardados en los registros OP1 Y OP2 respectivamente

    //ejecuto la operacion llamada
    int opA = (mv->registros[OP1]) & 0x00FFFFFF; // le saco el byte de tipo de operando
    int opB = (mv->registros[OP2]) & 0x00FFFFFF;
    // del operando B me quedo el valor, para eso uso el byte del tipo de operando
    if(mv->registros[OP2] & 0xFF000000 == 1)
        opB = mv->registros[opB];
    else
        if(mv->registros[OP2] & 0xFF00000 == 3)
            opB = get_valor_mem(opB,mv);
        
        //en el otro caso no modifico al opB ya que seria un inmediato que es valor que ya almacena

    instruction_handler(opA,opB,mv->registros[OPC],mv,ToperandoA);

}