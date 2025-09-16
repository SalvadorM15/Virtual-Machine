#include "VM.h"
#include <stdio.h>
#include <stdlib.h>



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
void div_op(int opa, int opb, MaquinaVirtual *mv, int Toperando){
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
            aux = (unsigned int) mv->registros[opa];
            aux = aux<<opb;
            mv->registros[opa] = (int)aux;
        }
        else {
            aux = (unsigned int) get_valor_mem(opa,mv);
            aux = aux << opb;
            set_valor_mem(opa,(int)aux,mv);
        }
        evaluarCC((int)aux,mv);
    }
    else {
        error_handler(INVINS);
    }
}

void shr(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    if (opb >= 0 && opb < 32) {
        unsigned int aux;  // usamos unsigned para que sea lógico (rellena con 0s)
        if (Toperando == 1) {
            aux = (unsigned int) mv->registros[opa];
            aux = aux >> opb;
            mv->registros[opa] = (int) aux;  // casteo de vuelta a int si tus registros son signed
        }
        else {
            aux = (unsigned int) get_valor_mem(opa, mv);
            aux = aux >> opb;
            set_valor_mem(opa, (int) aux, mv);
        }
        evaluarCC((int)aux, mv);
    }
    else {
        // Error: desplazamiento fuera de rango
        error_handler(INVINS);
    }
}

void sar(int opa, int opb, MaquinaVirtual *mv, int Toperando){
    if (opb >= 0 && opb < 32) {
        int aux;
        if (Toperando == 1) {
            // Operando A es un registro
            aux = mv->registros[opa] >> opb;
            mv->registros[opa] = aux;
        }
        else {
            // Operando A es memoria
            aux = get_valor_mem(opa, mv);
            aux = aux >> opb;  // mantiene el signo
            set_valor_mem(opa, aux, mv);
        }
        evaluarCC(aux, mv);
    }
    else {
        // Error: desplazamiento fuera de rango
        error_handler(INVINS);
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

void sys(int op, MaquinaVirtual *mv){
int cantCeldas = mv->registros[ECX] & 0x0000FFFF;
int tamanioCelda = (mv->registros[ECX] >> 16) & 0x0000FFFF;
int direccionInicial = get_logical_dir(*mv, EDX);
direccionInicial = logical_to_physical(direccionInicial, mv->seg, MEM); // obtengo la direccion fisica inicial
if (direccionInicial+cantCeldas*tamanioCelda>MEM){ // evaluo si me voy a quedar sin memoria
    error_handler(SEGFAULT);
    return;
}
else {
    for (int i=direccionInicial; i<direccionInicial+cantCeldas*tamanioCelda; i+=tamanioCelda){ //recorro todas las celdas a utilizar
            printf("[%d] :", i);
            if (op==1){
                int entrada=0;
                switch (mv->registros[EAX]){ // evaluo el tipo de dato de entrada
                case 4: leerBinario(&entrada);
                        break;
                case 3: scanf("%x" , &entrada);
                        break;
                case 2: scanf("%o", &entrada);
                        break;
                case 1: {
                        char ch=0;
                        scanf("%c", &ch);
                        entrada=(unsigned char) ch;
                        break;
                }
                case 0: scanf("%d" , &entrada);
                        break;
                }
                for (int n=0; n < tamanioCelda; n++){
                    mv->ram[i+n] = ((entrada >> 8*n)) & 0xFF; // empaqueto byte por byte el dato de entrada;
                }
            }
            else {
                int salida = 0;
                for (int n=0; n<tamanioCelda; n++)
                    salida |= ((int)mv->ram[i+n]) << (8*n); // rearmo el dato de salida, que estaba previamente guardado byte por byte
                switch (mv->registros[EAX]){ // evaluo el tipo de dato de salida
                case 4: imprimirBinarioCompacto(salida);
                        break;
                case 3: printf("%x \n", salida);
                        break;
                case 2: printf("%o \n", salida);
                        break;
                case 1: {
                        unsigned char ch =(salida & 0xFF);
                        if (ch>=32 && ch<=126) // evaluo si es caracter imprimible
                            printf("%c \n" , ch);
                            else
                                printf(". \n");
                        printf("%c \n", salida);
                        break;
                }
                case 0: printf("%d \n", salida);
                        break;
                }
            }
    }
}
}
void jmp(int op, MaquinaVirtual *mv, int Toperando){
    int dir;
    if (Toperando == 1)
        dir = mv->registros[op];
    else
        dir = get_valor_mem(op,mv);
    mv->registros[IP]=dir;
}

void jz(int op, MaquinaVirtual *mv, int Toperando){
    if ((mv->registros[CC] & 0x01)!=0)
        jmp(op,mv,Toperando);
}

void jp(int op, MaquinaVirtual *mv, int Toperando){
    if (mv->registros[CC]==0x00)
        jmp(op,mv,Toperando);
}
void jn_op(int op, MaquinaVirtual *mv, int Toperando){
    if ((mv->registros[CC] & 0x02) != 0)
        jmp(op,mv,Toperando);
}
void jnz(int op, MaquinaVirtual *mv, int Toperando){
    if ((mv->registros[CC] & 0x01) == 0)
        jmp(op,mv,Toperando);
}
void jnp(int op, MaquinaVirtual *mv, int Toperando){
    if ((mv->registros[CC] & 0x03)!=0)
        jmp(op,mv,Toperando);
}
void jnn(int op, MaquinaVirtual *mv, int Toperando){
    if ((mv->registros[CC] & 0x02)==0)
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
 // Sin operandos

 void stop(MaquinaVirtual *mv){
    mv->registros[IP] = -1;
 }



// continua...







//---------------------------------------------FIN MNEMONICOS----------------------------------------------------------------------------------------------


//funciones utiles para resolver otros problemas


int get_logical_dir(MaquinaVirtual mv, int operandoM){ //funcion creada para obtener la direccion logica de un operando de memoria

    int segmento = mv.registros[(operandoM >> 16)];
    int offset = operandoM & 0x0000FFFF;
    int direccion = (segmento << 16)+offset;

    return direccion;
}

int logical_to_physical(int logical_dir ,short int seg_table[MAX][2], int cant_bytes){ // funcion para pasar una direccion logica a una fisica
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
    mv->registros[OP1] = topA;
    mv->registros[OP2] = topB;
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

void leerBinario(int *entrada) {
    char buffer[64];  // guardamos el n�mero como string
    scanf("%63s", buffer);
    // strtol convierte string a entero, indicando base 2
    *entrada = (int) strtol(buffer, NULL, 2);
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
            break;
    case MUL:
            mul(opA,opB,mv,ToperandoA);
            break;
    case DIV:
            div_op(opA,opB,mv,ToperandoA);
            break;
    case CMP:
            cmp(opA,opB,mv,ToperandoA);
            break;
    case SHL:
            shl(opA,opB,mv,ToperandoA);
            break;
    case SHR:
            shr(opA,opB,mv,ToperandoA);
            break;
    case SAR:
            sar(opA,opB,mv,ToperandoA);
            break;
    case AND:
            and(opA,opB,mv,ToperandoA);
            break;
    case OR:
            or(opA,opB,mv,ToperandoA);
            break;
    case XOR:
            xor(opA,opB,mv,ToperandoA);
            break;
    case SWAP:
            swap(opA,opB,mv,ToperandoA);
            break;
    case LDL:
            ldl(opA,opB,mv,ToperandoA);
            break;
    case LDH:
            ldh(opA,opB,mv,ToperandoA);
            break;
    case RND:
            rnd(opA,opB,mv,ToperandoA);
            break;
    case SYS:
            sys(opA,mv);
            break;
    case JMP:
            jmp(opA,mv,ToperandoA);
            break;
    case JZ:
            jz(opA,mv,ToperandoA);
            break;
    case JP:
            jp(opA,mv,ToperandoA);
            break;
    case JN:
            jn_op(opA,mv,ToperandoA);
            break;
    case JNZ:
            jnz(opA,mv,ToperandoA);
            break;
    case JNP:
            jnp(opA,mv,ToperandoA);
            break;
    case JNN:
            jnn(opA,mv,ToperandoA);
            break;
    case NOT:
            not(opA,mv,ToperandoA);
            break;
    case STOP:
            stop(mv);
            break;
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

void lectura_arch(MaquinaVirtual *mv, short int *tamSeg, char nombre_arch[]){
    FILE *arch;
    char num;
    int i;

    arch = fopen(nombre_arch, "rb");
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
                fread(tamSeg, sizeof(short int), 1, arch);
            }

        }

        i = 0;
        if(!feof(arch)){
            fread(&num, sizeof(char), 1, arch);
            while(!feof(arch) && i < *tamSeg){
                (mv->ram)[i] = num;
                i++;
                fread(&num, sizeof(char), 1, arch);
            }

            if(i == *tamSeg){
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
    if((mv->registros[OP2] & 0xFF000000)>>24 == 1)
        opB = mv->registros[opB];
    else
        if((mv->registros[OP2] & 0xFF00000)>>24 == 3)
            opB = get_valor_mem(opB,mv);

        //en el otro caso no modifico al opB ya que seria un inmediato que es valor que ya almacena

    instruction_handler(opA,opB,mv->registros[OPC],mv,ToperandoA);
    
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