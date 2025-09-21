#include "VM.h"
#include <stdio.h>
#include <stdlib.h>



//-------------------------------------------------SECCION DE MNEMONICOS---------------------------------------------------------------------------------

//instrucciones de 2 operandos

//la idea del mov es que le pase el operandoA y el valor del operando b, el valor del operando b se consigue con los getters de valor antes de llamar a la funcion

void mov(int opa , int opb , MaquinaVirtual *mv){

    int valorOPB = get_valor_operando(valorOPB,mv);
    set_valor_operando(opa,valorOPB,mv);
}

void add(int opa, int opb, MaquinaVirtual *mv){

     int valorOPB = get_valor_operando(opb,mv);
    int res;
    set_valor_operando(opa,get_valor_operando(opa,mv)+valorOPB,mv);
    evaluarCC(get_valor_operando(opa,mv),mv);
}


void sub(int opa , int opb, MaquinaVirtual *mv){

    add(opa,-1*opb,mv); // es lo mismo que sumar el negado

}

void mul(int opa, int opb, MaquinaVirtual *mv){
    int res;
     int valorOPB = get_valor_operando(opb,mv);
    set_valor_operando(opa,get_valor_operando(opa,mv)*valorOPB,mv);
    evaluarCC(get_valor_operando(opa,mv),mv);
}
void div_op(int opa, int opb, MaquinaVirtual *mv){
    int valorOPB = get_valor_operando(opb,mv);
    if (valorOPB!=0){
        int cociente, resto;
        cociente = get_valor_operando(opa,mv)/valorOPB;
        resto = get_valor_operando(opa,mv)%valorOPB;
        set_valor_operando(opa,cociente,mv);
        mv->registros[AC]=resto;
        evaluarCC(cociente,mv);
    }
    else {
        //DIVISION POR CERO
        error_handler(DIV0);
    }
}

void cmp(int opa, int opb, MaquinaVirtual *mv){
    int valorOPB = get_valor_operando(opb,mv);
    int res;
    res = get_valor_operando(opa,mv)-valorOPB;
    evaluarCC(res, mv);
}

void shl(int opa, int opb, MaquinaVirtual *mv){
    int aux;
    int valorOPB = get_valor_operando(opb,mv);
    if (valorOPB>=0  && valorOPB<32 ) {
        aux = get_valor_operando(opa,mv) << valorOPB;
        printf("valor despues del shl: %u \n", aux);
        set_valor_operando(opa,aux,mv);
    }
    else {
        error_handler(INVINS);
    }
}

void shr(int opa, int opb, MaquinaVirtual *mv){
     int valorOPB = get_valor_operando(opb,mv);
    if (valorOPB >= 0 && valorOPB < 32) {
        unsigned int aux;  // usamos unsigned para que sea lógico (rellena con 0s)
        aux = (unsigned int)get_valor_operando(opa,mv) >> valorOPB;
        set_valor_operando(opa, (int)aux, mv);
        evaluarCC((int)aux, mv);
    }
    else {
        // Error: desplazamiento fuera de rango
        error_handler(INVINS);
    }
}

void sar(int opa, int opb, MaquinaVirtual *mv){
     int valorOPB = get_valor_operando(opb,mv);
    if (valorOPB >= 0 && valorOPB < 32) {
        int aux;
        aux = get_valor_operando(opa,mv) >> valorOPB; // en C el corrimiento a la derecha de un int es aritmético
        set_valor_operando(opa, aux, mv);
        evaluarCC(aux, mv);
    }   
    else {
        // Error: desplazamiento fuera de rango
        error_handler(INVINS);
    }
}

void and(int opa, int opb, MaquinaVirtual *mv){
     int valorOPB = get_valor_operando(opb,mv);
    int aux;
    aux = get_valor_operando(opa,mv) & valorOPB;
    set_valor_operando(opa,aux,mv);
    evaluarCC(aux,mv);
}

void or(int opa, int opb, MaquinaVirtual *mv){
    int valorOPB = get_valor_operando(opb,mv);
    int aux;
    aux = get_valor_operando(opa,mv) | valorOPB;
    printf("valor despues del or: %u \n", aux);
    set_valor_operando(opa,aux,mv);
    evaluarCC(aux,mv);
}

void xor(int opa, int opb, MaquinaVirtual *mv){
    int valorOPB = get_valor_operando(opb,mv);
    int aux;
    aux = get_valor_operando(opa,mv) ^ valorOPB;
    set_valor_operando(opa,aux,mv);
    evaluarCC(aux,mv);
}

void swap(int opa, int opb, MaquinaVirtual *mv){ // en este opb no pasas como valor sino como operando
    int aux=opb;
    set_valor_operando(opb,get_valor_operando(opa,mv),mv);
    set_valor_operando(opa,aux,mv);
}

void ldh(int opa, int opb, MaquinaVirtual *mv){
    // defino dos variables para construir el valor final del OperandoA
     int valorOPB = get_valor_operando(opb,mv);
    int parteB = (valorOPB <<16 ) & 0xFFFF0000; //utilizo una mascara para obtener los dos bytes menos significativos del OPB
    int parteA;
    parteA = get_valor_operando(opa,mv) & 0x0000FFFF;
    set_valor_operando(opa, parteA | parteB,mv);
}

void ldl(int opa, int opb, MaquinaVirtual *mv){
     int valorOPB = get_valor_operando(opb,mv);
    int parteB = (valorOPB & 0x0000FFFF) ;
    int parteA;
    parteA = get_valor_operando(opa,mv) & 0xFFFF0000;
    set_valor_operando(opa, parteA | parteB,mv);
}

void rnd(int opa, int opb, MaquinaVirtual *mv){
     int valorOPB = get_valor_operando(opb,mv);
    int valor = rand() % (valorOPB+1); // obtengo un valor random entre 0 y el opb
    set_valor_operando(opa,valor,mv);
}
//fin instrucciones de 2 operandos

//instrucciones de 1 operando

void sys(int op, MaquinaVirtual *mv){
    int cantCeldas = (mv->registros[ECX] & 0x0000FFFF);
    int tamanioCelda = ((mv->registros[ECX] >> 16) & 0x0000FFFF);
    int direccionInicial = mv->registros[EDX]; // obtengo la direccion logica inicial
    printf("direcccion logica inicial: %d \n", direccionInicial);
    direccionInicial = logical_to_physical(direccionInicial, mv->seg, tamanioCelda*cantCeldas); // obtengo la direccion fisica inicial
    printf("direccion inicial fisica: %d \n", direccionInicial);
    if (direccionInicial+cantCeldas*tamanioCelda>MEM || direccionInicial == -1){ // evaluo si me voy a quedar sin memoria
        error_handler(SEGFAULT);
        return;
    }
    else {
        for (int i=direccionInicial; i<direccionInicial+cantCeldas*tamanioCelda; i+=tamanioCelda){ //recorro todas las celdas a utilizar
                printf("[%d] :", i);
                if (get_valor_operando(op,mv)==1){
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
                    printf("voy a escribir: %d \n", entrada);
                    printf("en la direccion fisica: %d \n", i);
                    for(int n=0; n<tamanioCelda;n++){
                        mv->ram[i+n] = ((entrada >> (24-(n*8))) & 0xFF);
                    }
                }
                else {
                    if(get_valor_operando(op,mv) == 2){
                        unsigned int salida = 0;
                        // rearmo el dato de salida, que estaba previamente guardado byte por byte   
                        for(int n=0; n<tamanioCelda;n++){
                            salida |= ((mv->ram[i+n] << (24-(n*8))));
                        } 
                        printf("voy a sacar: %d \n", salida);
                        printf("de la direccion fisica: %d \n", i);
                        switch (mv->registros[EAX]){ // evaluo el tipo de dato de salida
                        case 4: imprimirBinarioCompacto(salida);
                                break;
                        case 3: printf("salida: %x \n", salida);
                                break;
                        case 2: printf("salida: %o \n", salida);
                                break;
                        case 1: {
                                unsigned char ch =(salida & 0xFF);
                                if (ch>=32 && ch<=126) // evaluo si es caracter imprimible
                                    printf("salida: %c \n" , ch);
                                    else
                                        printf(". \n");
                                printf("salida: %c \n", salida);
                                break;
                        }
                        case 0: printf("salida: %u \n", salida);
                                break;
                        }
                    }
                    else
                        error_handler(INVINS);
                }
        }
    }
}
void jmp(int op, MaquinaVirtual *mv){

    mv->registros[IP] =  get_valor_operando(op,mv);
}

void jz(int op, MaquinaVirtual *mv){
    if ((mv->registros[CC] & 0x01)!=0)
        jmp(op,mv);
}

void jp(int op, MaquinaVirtual *mv){
    if (mv->registros[CC]==0x00)
        jmp(op,mv);
}
void jn_op(int op, MaquinaVirtual *mv){
    if ((mv->registros[CC] == 0x02))
        jmp(op,mv);
}
void jnz(int op, MaquinaVirtual *mv){
    if ((mv->registros[CC] != 0x01))
        jmp(op,mv);
}
void jnp(int op, MaquinaVirtual *mv){
    if ((mv->registros[CC] != 0x00))
        jmp(op,mv);
}
void jnn(int op, MaquinaVirtual *mv){
    if ((mv->registros[CC] != 0x02))
        jmp(op,mv);
}
void not(int op, MaquinaVirtual *mv){
    int aux;
    aux = ~get_valor_operando(op,mv);
    evaluarCC(aux, mv);
}
 // Sin operandos

 void stop(MaquinaVirtual *mv){
    mv->registros[IP] = -1;
 }





//---------------------------------------------FIN MNEMONICOS----------------------------------------------------------------------------------------------












//--------------------------------------------------HANDLERS DE ERRORES Y MNEMONICOS-----------------------------------------------------------------------------




void instruction_handler(int opA, int opB, int operacion, MaquinaVirtual *mv){ // seguimos con la idea de pasar el opb por el valor que tiene y no como operando en si
    switch (operacion)
    {
    case MOV:
            mov(opA,opB,mv);
        break;
    case ADD:
            add(opA,opB,mv);
        break;
    case SUB:
            sub(opA,opB,mv);
            break;
    case MUL:
            mul(opA,opB,mv);
            break;
    case DIV:
            div_op(opA,opB,mv);
            break;
    case CMP:
            cmp(opA,opB,mv);
            break;
    case SHL:
            shl(opA,opB,mv);
            break;
    case SHR:
            shr(opA,opB,mv);
            break;
    case SAR:
            sar(opA,opB,mv);
            break;
    case AND:
            and(opA,opB,mv);
            break;
    case OR:
            or(opA,opB,mv);
            break;
    case XOR:
            xor(opA,opB,mv);
            break;
    case SWAP:
            swap(opA,opB,mv);
            break;
    case LDL:
            ldl(opA,opB,mv);
            break;
    case LDH:
            ldh(opA,opB,mv);
            break;
    case RND:
            rnd(opA,opB,mv);
            break;
    case SYS:
            sys(opA,mv);
            break;
    case JMP:
            jmp(opA,mv);
            break;
    case JZ:
            jz(opA,mv);
            break;
    case JP:
            jp(opA,mv);
            break;
    case JN:
            jn_op(opA,mv);
            break;
    case JNZ:
            jnz(opA,mv);
            break;
    case JNP:
            jnp(opA,mv);
            break;
    case JNN:
            jnn(opA,mv);
            break;
    case NOT:
            not(opA,mv);
            break;
    case STOP:
            stop(mv);
            break;
    default:
            error_handler(INVINS);
            printf("detectador por el handler\n");
        break;
    }
}


void error_handler(int error){

    switch(error){
    case SEGFAULT:
        printf("ERROR: SEGMENTATION FAULT\n");
        break;
    case DIV0:
        printf("ERROR: DIVISION POR 0 NO ESTA PERMITIDA\n");
        break;
    case INVINS:
        printf("ERROR: INSTRUCCION INVALIDA\n");
        break;
    case INVVER:
        printf("ERROR: VERSION INVALIDA\n");
        break;
    }

    exit(1);
}

//--------------------------------------------------FIN DE LOS HANDLERS-----------------------------------------------------------------------------





//--------------------------------------------------FUNCIIONES PRINCIPALES DE LA MAQUINA VIRTUAL-----------------------------------------------------------------------------





void lectura_arch(MaquinaVirtual *mv, short int *tamSeg, char nombre_arch[]){
    FILE *arch;
    char num,version;
    int i;

    arch = fopen(nombre_arch, "rb");
    if(arch != NULL){

        //lee los primero 5 bytes de la cabecera

        i = 0;
        while(!feof(arch) && i<5){
            fread(&num, sizeof(char), 1, arch);
            printf("%c", num);
            i++;
        }
        //leo un byte de la version
        fread(&version, sizeof(char), 1, arch);
        printf("\nVersion del archivo: %d \n", version);
        //lee el tamanio del segmento de codigo
        printf("\n");
        if(!feof(arch)){
            fread(tamSeg, sizeof(short int), 1, arch);
            (*tamSeg) = ((*tamSeg >> 8) & 0x00FF) | ((*tamSeg << 8) & 0xFF00);
        }
        printf("Tamanio del segmento de codigo: %d \n", *tamSeg);
        //comienza la lectura del codigo y lo almacena en la ram
        i = 0;
        if(!feof(arch)){
            fread(&num, sizeof(char), 1, arch);
            while(!feof(arch) && i < *tamSeg){
                (mv->ram)[i] = num;
                i++;
                fread(&num, sizeof(char), 1, arch);
            }
        }
        printf("Se leyeron %d bytes de codigo\n", i);
        fclose(arch);
    }

}

void iniciaMV(MaquinaVirtual *mv, int codSize){ // codsize leido de la cabecera

    //inicio la tabla de segmentos
    mv->seg[0][0] = 0;
    mv->seg[0][1] = codSize-1;
    mv->seg[1][0] = codSize;
    mv->seg[1][1] = MEM-1;

    //inicializo los registros de segmento
    mv->registros[CS] = 0; // ya se que no esta bien inicializarlo asi, a preguntar
    mv->registros[DS] = (1<<16); // el segmento de datos comienza despues del segmento de codigo

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

    instruction_handler(mv->registros[OP1],mv->registros[OP2],mv->registros[OPC],mv);

}

void procesaOperacion(char instruccion, int *topA, int *topB, int *op){

    *op = (int)(instruccion & 0x0000001F);
    if((instruccion & 0x30) != 0){
        *topB = (instruccion >> 6) &0x03;
        *topA = (instruccion >> 4) &0x03;
    }
    else{
        *topB = 0;
        if((instruccion & 0xF0) == 0){
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
    for(i = ((mv->registros[IP])+1); i < ((mv->registros[IP]) + topB+1); i++){
        mv->registros[OP2] = mv->registros[OP2] << 8;
        mv->registros[OP2] += mv->ram[i];
    }
    mv->registros[IP] += topB;

    for(i = ((mv->registros[IP])+1); i < ((mv->registros[IP]) + topA+1); i++){
        mv->registros[OP1] = mv->registros[OP1] << 8;
        mv->registros[OP1] += mv->ram[i];
    }
    mv->registros[IP] += topA;
    mv->registros[IP]++; // avanzo el ip al proximo byte de instruccion porque sino queda en el ultimo operando
    //agrego el tipo de operando en el byte mas significativo
    mv->registros[OP1] += (topA << 24);
    mv->registros[OP2] += (topB << 24);
}

//------------------------------------------------------FIN DE LAS FUNCIONES PRINCIPALES DE LA MAQUINA VIRTUAL-----------------------------------------------------------------------------













//----------------------------------------------------------UTILIDADES--------------------------------------------------------------

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


void leerBinario(int *entrada) {
    char buffer[64];  // guardamos el n�mero como string
    scanf("%63s", buffer);
    // strtol convierte string a entero, indicando base 2
    *entrada = (int) strtol(buffer, NULL, 2);
}

void evaluarCC(int res, MaquinaVirtual *mv){
    int valor = 0;
    if (res==0)
        valor = (0x01);
        else {
            if (res<0)
                valor = (0x02);
            else{
            valor = 0x00;
        }
    }
    mv->registros[CC]=valor;

}

int get_logical_dir(MaquinaVirtual mv, int operandoM){ //funcion creada para obtener la direccion logica de un operando de memoria

    int segmento = (mv.registros[(operandoM >> 16) & 0x000000FF]);
    int offset = (operandoM & 0x0000FFFF);
    int direccion = segmento + offset ;

    return direccion;
}

int logical_to_physical(int logical_dir ,short int seg_table[MAX][2], int cant_bytes){ // funcion para pasar una direccion logica a una fisica
    int physical_dir;
    int segment =((logical_dir >> 16) & 0x0000FFFF);
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

//--------------------------------------------------FIN DE UTILIDADES--------------------------------------------------------------














//------------------------------------GETTERS Y SETTERS DE OPERANDOS--------------------------------------------------------------------------------

void set_valor_operando(int operando, int valor, MaquinaVirtual *mv){

    if((operando & 0xFF000000)>>24 == 1 ){
        mv->registros[(operando & 0x00FFFFFF)] = valor;
    }
    else{ 
        if((operando & 0xFF000000)>>24 == 3){
            set_valor_mem((operando & 0x00FFFFFF), valor, mv);
        }
        else{
            // Error: intento asignar a un inmediato
            error_handler(INVINS);
        }
    }

}

int get_valor_operando(int operando, MaquinaVirtual *mv){

    if((operando & 0xFF000000)>>24 == 2 ){ // operando inmediato
        return (operando & 0x00FFFFFF);
    }
    else{
        if((operando & 0xFF000000)>>24 == 1 ){ // operando registro
            return mv->registros[(operando & 0x00FFFFFF)];
        }
        else{ // operando memoria
            if((operando & 0xFF000000)>>24 == 3)
                return get_valor_mem((operando & 0x00FFFFFF), mv);
        }
    }
}

int get_valor_mem(int operandoM, MaquinaVirtual *mv){


    mv->registros[LAR] = get_logical_dir(*mv, operandoM); // busco la direccion logica

    // paso la direccion logica a fisica

    int direccion = logical_to_physical(mv->registros[LAR], mv->seg , 4);
    mv->registros[MAR] = direccion; // guardo la direccion fisica en los 2 bytes menos significativos
    mv->registros[MAR] +=(3<<32); //quedan los 2 bits mas significativos diciendo que vna a guardar 3 bytes

    if(direccion == -1){
        error_handler(SEGFAULT);
        return -1;
    }
    else{
        mv->registros[MBR] = ((mv->ram[direccion] << 24) | (mv->ram[direccion + 1] << 16) |  (mv->ram[direccion + 2] << 8) | (mv->ram[direccion + 3]));
        return mv->registros[MBR];
    }
}

void set_valor_mem(int operandoM, int valor, MaquinaVirtual *mv){
    // busco la direccion logica
    int aux=0;
    mv->registros[LAR] = get_logical_dir(*mv, operandoM);
    mv->registros[MBR] = valor;
    // paso la direccion logica a fisica

    int direccion = logical_to_physical(mv->registros[LAR] , mv->seg , 4);
    mv->registros[MAR] = direccion; // guardo la direccion fisica en los 2 bytes menos significativos
    mv->registros[MAR] +=(3<<32); //quedan los 2 bits mas significativos diciendo que vna a guardar 3 bytes

    if(direccion == -1){
        error_handler(SEGFAULT);
    }
    else{
        // 1er byte:
        mv->ram[direccion]     = (valor >> 24) & 0xFF;
        // 2do byte:
        mv->ram[direccion + 1] = (valor >> 16) & 0xFF;
        // 3er byte:
        mv->ram[direccion + 2] = (valor >> 8)  & 0xFF;
        // 4to byte:
        mv->ram[direccion + 3] = valor & 0xFF;
    }
}

//------------------------------------FIN GETTERS Y SETTERS DE OPERANDOS--------------------------------------------------------------------------------