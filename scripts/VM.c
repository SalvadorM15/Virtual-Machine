#include "VM.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//-------------------------------------------------SECCION DE MNEMONICOS---------------------------------------------------------------------------------

//instrucciones de 2 operandos

//la idea del mov es que le pase el operandoA y el valor del operando b, el valor del operando b se consigue con los getters de valor antes de llamar a la funcion

void mov(int opa , int opb , MaquinaVirtual *mv){

    int valorOPB = get_valor_operando(opb,mv);
    set_valor_operando(opa,valorOPB,mv);
}

void add(int opa, int opb, MaquinaVirtual *mv){

     int valorOPB = get_valor_operando(opb,mv);
    set_valor_operando(opa,get_valor_operando(opa,mv)+valorOPB,mv);
    evaluarCC(get_valor_operando(opa,mv),mv);
}


void sub(int opa , int opb, MaquinaVirtual *mv){

   int  valorOPB = get_valor_operando(opb,mv);
    set_valor_operando(opa,get_valor_operando(opa,mv)-valorOPB,mv);
    evaluarCC(get_valor_operando(opa,mv),mv);


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
    else
        error_handler(DIV0);
}

void cmp(int opa, int opb, MaquinaVirtual *mv){
    int valorOPB = get_valor_operando(opb,mv);
    int res;
    res = get_valor_operando(opa,mv)-valorOPB;
    evaluarCC(res, mv);
}

void shl(int opa, int opb, MaquinaVirtual *mv){
    unsigned int aux;
    int valorOPB = get_valor_operando(opb,mv);
    if (valorOPB>=0  && valorOPB<32 ) {
        aux = get_valor_operando(opa,mv);
        aux = aux << valorOPB;
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
    aux = get_valor_operando(opa,mv);
    aux = aux | valorOPB;
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
    direccionInicial = logical_to_physical(direccionInicial, mv, tamanioCelda*cantCeldas,"CUALQUIERA"); // obtengo la direccion fisica inicial
    if ( direccionInicial == -1){
        error_handler(SEGFAULT);
        return;
    }
    else {
        for (int i=direccionInicial; i<direccionInicial+cantCeldas*tamanioCelda; i+=tamanioCelda){ //recorro todas las celdas a utilizar
                printf("[%x]:", i);
                if (get_valor_operando(op,mv)==1){
                    int entrada=0;
                    switch (mv->registros[EAX]){ // evaluo el tipo de dato de entrada
                    case 16: leerBinario(&entrada);
                            break;
                    case 8: scanf("%x" , &entrada);
                            break;
                    case 4: scanf("%o", &entrada);
                            break;
                    case 2: {
                            char ch=0;
                            scanf("%c", &ch);
                            entrada=(unsigned char) ch;
                            break;
                    }
                    case 1: scanf("%d" , &entrada);
                            break;
                    default: //error_handler(INVINS);
                             break;
                    }
                    // 1er byte:
                    mv->ram[i]     = (entrada >> 24) & 0x000000FF;
                    // 2do byte:
                    mv->ram[i + 1] = (entrada >> 16) & 0x000000FF;
                    // 3er byte:
                    mv->ram[i + 2] = (entrada>> 8)  & 0x000000FF;
                    // 4to byte:
                    mv->ram[i + 3] = entrada & 0x000000FF;
                    
                }
                else {
                    if(get_valor_operando(op,mv) == 2){ 
                       int salida = 0;
                        // evaluo si me voy a quedar sin memoriarmo el dato de salida, que estaba previamente guardado byte por byte   
                       salida = (((mv->ram[i] << 24)&0xFF000000) | ((mv->ram[i + 1] << 16)&0x00FF0000) |  ((mv->ram[i + 2] << 8)&0x00000FF00) | ((mv->ram[i + 3]&0x000000FF)));
                       if (mv->registros[EAX] & 0x10) // si el bit 4 del eax es 1, imprimo en formato compacto
                           imprimirBinarioCompacto(salida);
                       if (mv->registros[EAX] & 0x08) 
                            printf("salida: %x \n", salida);
                       if (mv->registros[EAX] & 0x04)
                            printf("salida: %o \n", salida);    
                       if (mv->registros[EAX] & 0x02){
                            for(int i = 0; i < 4; i++){
                                unsigned char ch = (salida >> (8 * (3 - i))) & 0x000000FF;
                                if(ch != 0)
                                    if (ch >= 32 && ch <= 126) // evaluo si es caracter imprimible
                                        printf("salida: %c \n", ch);
                                    else
                                        printf(". \n");
                            }
                        }
                          if (mv->registros[EAX] & 0x01)
                             printf("salida: %d \n", salida);
                    }
                    else  if(get_valor_operando(op,mv) == 3){
                        char cadena[100]; //no se q tamaño darle
                        scanf("%s", &cadena);
                        short int stringlen = (mv->registros[ECX] & 0x0000FFFF);

                        if(stringlen & 0x00008000) // si el bit 15 del inmediato es 1, es negativo
                            stringlen = stringlen | 0xFFFF0000;

                        if(stringlen == -1)
                            stringlen = strlen(cadena);

                        for(int i = 0; i< stringlen; i++){
                            mv->ram[direccionInicial + i] = cadena[i];
                        }

                        mv->ram[direccionInicial + stringlen] = '\0'; // agrego el caracter nulo al final
                    }
                    else if(get_valor_operando(op,mv) == 4){
                        int i = mv->registros[EDX];
                        while(mv->ram[i] != '\0'){
                            printf("%c", mv->ram[i]);
                            i++;
                        }

                    }
                    else if(get_valor_operando(op,mv) == 7){
                        system("clear");
                    }
                    else if(get_valor_operando(op,mv) == 0xf){
                        breakPoint(mv,mv->vmiFileName);
                    }
                        
                }
        }
    }
}
void jmp(int op, MaquinaVirtual *mv){
    int proxIP = (get_valor_operando(op,mv))&0x0000ffff;
    if(proxIP < 0 || proxIP >= mv->seg[mv->registros[CS]][1] + 1)
        error_handler(SEGFAULT);
    else
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


void push(int operando,MaquinaVirtual *mv){
    mv->registros[SP] -= 4;
    if(mv->registros[SP] < mv->registros[SS])
        error_handler(STACKOVER);
    //SI HABIA LUGAR:
    else{
        //GUARDO EN valor EL VALOR DEL OPERANDO (VALOR A GUARDAR EN LA PILA) CONVERTIDO A 4 BYTES (CHEQUEAR)
        int valor = get_valor_operando(operando,mv);
        int direccion = logical_to_physical(mv->registros[SP], mv, 0, "STACK");

        set_valor_pila(mv,direccion,valor);

        //GUARDAR valor EN MEMORIA

    }
}

void pop(int operando, MaquinaVirtual *mv){
    int valor; int cantBytes;
    //CHEQUEO QUE NO HAYA STACK UNDERFLOW
    int numSeg = (mv->registros[SS] >> 16)&0x0000FFFF;
    int techo = mv->registros[SS] | mv->seg[numSeg][1];
    if(mv->registros[SP] > techo)
        error_handler(STACKUNDER);
    else{
        //LEO DE MEMORIA EL VALOR GUARDADO EN LA PILA
        int direccion = logical_to_physical(mv->registros[SP], mv, 0, "STACK");
        valor = get_valor_pila(mv,direccion);
        //ACTUALIZO EL REGISTRO SP
        mv->registros[SP] +=4;
        //GUARDO EN EL OPERANDO EL VALOR LEIDO DE LA PILA
        set_valor_operando(operando,valor,mv);
    }
}

void call(int operando, MaquinaVirtual *mv){
    //CALL ES COMO HACER PUSH IP Y HACER JMP A LA SUBRUTINA DEL OPERANDO
    push((mv->registros[IP])|0x02000000, mv);
    jmp(operando, mv);
}


 // Sin operandos

 void stop(MaquinaVirtual *mv){
    mv->registros[IP] = -1;
 }

 void ret(MaquinaVirtual *mv){
    //obtengo el valor de la pila al que apunta sp
    int i = logical_to_physical(mv->registros[SP], mv, 4, "STACK");
    int direccion = 0;
    direccion = get_valor_pila(mv,i);
    mv->registros[SP] += 4;
    int numSeg = (mv->registros[SS] >> 16)&0x0000FFFF;
    int techo = mv->registros[SS] | mv->seg[numSeg][1];
    if(mv->registros[SP] > techo)
        error_handler(STACKUNDER);
    if(direccion < -1|| direccion >= mv->seg[mv->registros[CS]][1] + mv->seg[mv->registros[CS]][0])
        error_handler(SEGFAULT);
    else
        mv->registros[IP] = direccion;
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
    case PUSH:
            push(opA,mv);
            break;
    case POP: 
            pop(opA,mv);
            break;
    case CALL:
            call(opA,mv);
            break;
    case RET:
            ret(mv);
            break;
    default:
            error_handler(INVINS);
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
        printf("ERROR: INSTRUCCION INVALIDA:\n");
        break;
    case MEMINS:
        printf("ERROR: MEMORIA INSUFICIENTE\n");
        break;
    case STACKOVER:
        printf("ERROR: STACK OVERFLOW\n");
        break;
    case STACKUNDER:
        printf("ERROR: STACK UNDERFLOW\n");
        break;
    case NOFILE:
        printf("ERROR: NO SE ENCONTRO EL ARCHIVO\n");
        break;
    default: break;
    }

    exit(1);
}

//--------------------------------------------------FIN DE LOS HANDLERS-----------------------------------------------------------------------------





//--------------------------------------------------FUNCIONES PRINCIPALES DE LA MAQUINA VIRTUAL-----------------------------------------------------------------------------





void lectura_arch(MaquinaVirtual *mv, char nombre_arch[], unsigned short int *codeSeg, unsigned short int *dataSeg, unsigned short int *extraSeg, unsigned short int *stackSeg, unsigned short int *constSeg, unsigned short int *offsetEP, char *version){
    FILE *arch;
    unsigned char num;
    int i;
    *dataSeg=0,*extraSeg=0,*stackSeg=0,*constSeg=0,*offsetEP=0;
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
        fread(version, sizeof(char), 1, arch);
        printf("\nVersion del archivo: %d \n", *version);
        fread(codeSeg,sizeof(unsigned short int),1,arch);
        //cambio el endian del dato
        *codeSeg = ((*codeSeg >> 8) & 0x00FF) | ((*codeSeg << 8) & 0xFF00);
         // por alguna razon lee 1 byte corrido a la izquierda
      
        //leo el tamanio de los segmentos de codigo
        if(*version == 2){
            fread(dataSeg,sizeof(unsigned short int),1,arch);
            fread(extraSeg,sizeof(unsigned short int),1,arch);
            fread(stackSeg,sizeof(unsigned short int),1,arch);
            fread(constSeg,sizeof(unsigned short int),1,arch);
            fread(offsetEP,sizeof(unsigned short int),1,arch);
            *dataSeg = ((*dataSeg >> 8) & 0x00FF) | ((*dataSeg << 8) & 0xFF00);
            *extraSeg = ((*extraSeg >> 8) & 0x00FF) | ((*extraSeg << 8) & 0xFF00);
            *stackSeg = ((*stackSeg >> 8) & 0x00FF) | ((*stackSeg << 8) & 0xFF00);
            *constSeg = ((*constSeg >> 8) & 0x00FF) | ((*constSeg << 8) & 0xFF00);
            *offsetEP = ((*offsetEP >> 8) & 0x00FF) | ((*offsetEP << 8) & 0xFF00);
        }
        else
            *dataSeg = mv->MemSize - (*codeSeg);
        //comienza la lectura del codigo y lo almacena en la ram
        i = 0;
        if(!feof(arch)){
            //leo el codigo maquina del archivo
            fread(&num, sizeof(char), 1, arch);
            while(!feof(arch) && i < *codeSeg){
                mv->ram[i] = num;
                i++;
                fread(&num, sizeof(char), 1, arch);
            }
        }
            if(*version == 2){
            
                if(!feof(arch)){
                    i = *constSeg;
                    while(!feof(arch) && i < *constSeg + *dataSeg){
                        fread(&num, sizeof(char), 1, arch);
                        (mv->ram)[i] = num;
                        i++;
                    }
                }
            }
        fclose(arch);
    }

}

void iniciaMV(MaquinaVirtual *mv, unsigned short int codeSeg,unsigned short int dataSeg,unsigned short int extraSeg,unsigned short int stackSeg,unsigned short int constSeg,unsigned short int paramSeg, int offsetEP) { // codsize leido de la cabecera
    
    //inicio la tabla de segmentos y los registros punteros a los segmentos
    
    creaTablaSegmentos(mv,paramSeg,codeSeg,dataSeg,extraSeg,stackSeg,constSeg);
    

    //inicializo el ip

    mv->registros[IP] = logical_to_physical(mv->registros[CS] | offsetEP, mv,4, "CODE"); // el entry point es un offset dentro del segmento de codigo

}

void step(MaquinaVirtual *mv){

    //primer paso: leer la instruccion del registro IP
    int ToperandoA,ToperandoB,operacion;
    char instruccion = mv->ram[mv->registros[IP]];

    //leo los valores del cs y muevo el IP
    
    procesaOperacion(instruccion,&ToperandoA,&ToperandoB,&operacion); // desarma la instruccion codificada
    mv->registros[OPC] = operacion;
    lee_operandos(ToperandoA,ToperandoB,mv); // lee los siguientes bytes de los operandos A y B y mueve el ip
    
    //lo valores operando 1 y 2 quedan guardados en los registros OP1 Y OP2 respectivamente

    instruction_handler(mv->registros[OP1],mv->registros[OP2],mv->registros[OPC],mv);

}

void procesaOperacion(char instruccion, int *topA, int *topB, int *op){

    *op = (int)(instruccion & 0x0000001F); // obtengo los 5 bits menos significativos
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
        mv->registros[OP2] = ((mv->registros[OP2])<< 8);
        mv->registros[OP2] |= (mv->ram[i])&0x000000FF;
    }
    if(topB == 2){
        if(mv->registros[OP2] & 0x00008000) // si el bit 15 del inmediato es 1, es negativo
            mv->registros[OP2] = mv->registros[OP2] | 0x00FF0000; // lo extiendo a 32 bits
    }
    mv->registros[IP] += topB;

    for(i = ((mv->registros[IP])+1); i < ((mv->registros[IP]) + topA+1); i++){
        mv->registros[OP1] = ((mv->registros[OP1]) << 8);
        mv->registros[OP1] |= mv->ram[i]&0x000000FF;
    }
    if(topA == 2){
        if(mv->registros[OP1] & 0x00008000) // si el bit 15 del inmediato es 1, es negativo
            mv->registros[OP1] = mv->registros[OP1] | 0x00FF0000; // lo extiendo a 32 bits
    }
    mv->registros[IP] += topA;
    mv->registros[IP]++; // avanzo el ip al proximo byte de instruccion porque sino queda en el ultimo operando
    //agrego el tipo de operando en el byte mas significativo
    mv->registros[OP1] |= (topA << 24)&0x03000000;
    mv->registros[OP2] |= (topB << 24)&0x03000000;
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
        else 
            if (res<0)
                valor = (0x02);
            else
            valor = 0x00;
    
    mv->registros[CC]=valor;

}

int get_logical_dir(MaquinaVirtual mv, int operandoM){ //funcion creada para obtener la direccion logica de un operando de memoria

    int segmento = (mv.registros[(operandoM >> 16) & 0x0000001F]);
    int offset =(operandoM & 0x0000FFFF);

    if(offset & 0x00008000) // si el bit 15 del offset es 1, es negativo
        offset = offset | 0xFFFF0000; // lo extiendo a 32 bits

    int direccion = segmento + offset; 

    return direccion;
}

int logical_to_physical(int logical_dir ,MaquinaVirtual *mv, int cant_bytes, char segmento[]){ // funcion para pasar una direccion logica a una fisica
    int physical_dir;
    int segment =((logical_dir >> 16) & 0x0000FFFF);
    int segment_limit = mv->seg[segment][0] + mv->seg[segment][1];
    if(segment < 8){
        physical_dir = mv->seg[segment][0];
        physical_dir += (logical_dir & 0x0000FFFF);
        if(strcmp(segmento, "STACK") != 0){
            if(physical_dir > segment_limit|| physical_dir < mv->seg[segment][0]){
            physical_dir = -1;
            printf("SEGFAULT 1 EN LOGICAL TO PHYSICAL\n");
          }
        }
    }
    else{
        physical_dir = -1;
        printf("SEGFAULT 2 EN LOGICAL TO PHYSICAL\n");
    }
    return physical_dir;
}

void printSegTable(short int seg_table[8][2]){
    printf("Tabla de segmentos:\n");
    for(int i = 0; i < 8; i++){
        printf("Segmento %d: Base: %x , Limite: %x\n", i, seg_table[i][0], seg_table[i][1]);
    }
}

//--------------------------------------------------FIN DE UTILIDADES--------------------------------------------------------------














//------------------------------------GETTERS Y SETTERS DE OPERANDOS--------------------------------------------------------------------------------

void set_valor_operando(int operando, int valor, MaquinaVirtual *mv){
    if(valor & 0x00800000) // si el bit 23 del inmediato es 1, es negativo
            valor = valor | 0xFF000000; // lo extiendo a 32 bits  

    if(((operando>>24) & 0x00000003) == 1 ){
        int tipoReg = (operando & 0x00000000C0) >> 6;
        switch (tipoReg){
            case 0: // registro de 4 bytes
                mv->registros[(operando & 0x00FFFFFF)]= valor;
                break;
            case 1: // 4to byte
                mv->registros[(operando & 0x00FFFFFF)] = (mv->registros[(operando & 0x00FFFFFF)] & 0XFFFFFF00) | ((unsigned int)(valor & 0x000000FF));
                break;
            case 2: // 3er byte
                mv->registros[(operando & 0x00FFFFFF)] = (mv->registros[(operando & 0x00FFFFFF)] & 0XFFFF00FF) | ((unsigned int)(valor<<8 & 0x0000FF00));
                break;
            case 3: // 2 bytes menos significativos
                mv->registros[(operando & 0x00FFFFFF)] = (mv->registros[(operando & 0x00FFFFFF)] & 0xFFFF0000) | ((unsigned int)(valor & 0x0000FFFF));
                break;
            }
    }
    else{ 
        if((operando>>24)&0X00000003 == 3){
            switch((operando>>22)&0X00000003){
                case 0: // long -> 4 bytes
                    set_valor_mem((operando & 0x00FFFFFF),valor, mv,4);
                    break;
                case 2: // word -> 2 bytes 
                    set_valor_mem((operando & 0x00FFFFFF),valor, mv,2);
                    break;
                case 3: // byte -> 1 byte
                    set_valor_mem((operando & 0x00FFFFFF),valor, mv,1);
                    break;
                default:
                    error_handler(INVINS);
                    break;
            }
        }
        else{
            // Error: intento asignar a un inmediato
            error_handler(INVINS);
        }
    }

}

int get_valor_operando(int operando, MaquinaVirtual *mv){
    int resultado;
    if(((operando & 0x03000000)>>24)== 2 ){ // operando inmediato
        resultado = (operando & 0x0000FFFF);
        if(resultado & 0x00008000) // si el bit 15 del inmediato es 1, es negativo
            resultado = resultado | 0xFFFF0000; // lo extiendo a 32 bits
    }
    else{
        if(((operando & 0x03000000)>>24)== 1 ){ // operando registro
             int tipoReg = (operando & 0x00000000C0) >> 6;
            switch (tipoReg){
                case 0: // registro de 4 bytes
                    resultado = mv->registros[(operando & 0x00FFFFFF)];
                    break;
                case 1: // 4to byte
                    resultado = (mv->registros[(operando & 0x00FFFFFF)] &  0xFF000000) >> 24;
                    break;
                case 2: // 3er byte
                    resultado = (mv->registros[(operando & 0x00FFFFFF)] &  0x00FF0000) >> 16;
                case 3: // 2 bytes menos significativos
                    resultado = mv->registros[(operando & 0x00FFFFFF)] &  0x0000FFFF;
                    break;
                }
        }
        else{ // operando memoria
            if((operando & 0xFF000000)>>24 == 3){
                switch((operando>>22)&0X00000003){
                    case 0: // long -> 4 bytes
                        resultado = get_valor_mem((operando & 0x00FFFFFF), mv,4);
                        break;
                    case 2: // word -> 2 bytes 
                        resultado = get_valor_mem((operando & 0x00FFFFFF), mv,2);
                        break;
                    case 3: // byte -> 1 byte
                        resultado = get_valor_mem((operando & 0x00FFFFFF), mv,1);
                        break;
                    default:
                        error_handler(INVINS);
                        break;
                }
            }
                
        }
         
    
    }
    return resultado;
   
}

int get_valor_mem(int operandoM, MaquinaVirtual *mv, int cant_bytes){

    char segmento[10];
    mv->registros[LAR] = get_logical_dir(*mv, operandoM); // busco la direccion logica
    if(operandoM & 0x1F000000 == BP || operandoM & 0x1E000000 == SP){
        strcpy(segmento, "STACK");
    }
    else
        strcpy(segmento, "CUAQUIERA");
    // paso la direccion logica a fisica
    int direccion = logical_to_physical(mv->registros[LAR], mv, 4,segmento);
    mv->registros[MAR] = direccion; // guardo la direccion fisica en los 2 bytes menos significativos
    mv->registros[MAR] |=(3<<30); //quedan los 2 bits mas significativos diciendo que vna a guardar 3 bytes

    if(direccion == -1){
        error_handler(SEGFAULT);
        return -1;
    }
    else{
        if(strcmp(segmento,"STACK") == 0){
            mv->registros[MBR] = get_valor_pila(mv, direccion);
        }
        else{
            for(int i =0; i<cant_bytes; i++){
                mv->registros[MBR] = (mv->registros[MBR] << 8) | (mv->ram[direccion + i]&0x000000FF);
            }
        }
        if(mv->registros[MBR] & (1 << ((cant_bytes * 8) - 1))) // si el bit mas significativo del valor leido es 1, es negativo
            mv->registros[MBR] |= 0xFFFFFFFF << (cant_bytes * 8); // lo extiendo a 32 bits
        return mv->registros[MBR];
    }
}

void set_valor_mem(int operandoM, int valor, MaquinaVirtual *mv, int cant_bytes){
    // busco la direccion logica
    int aux=0;
    char segmento[10];
    mv->registros[LAR] = get_logical_dir(*mv, operandoM);
    mv->registros[MBR] = valor;
    // paso la direccion logica a fisica
    if(operandoM & 0x1F000000 == BP || operandoM & 0x1E000000 == SP){
       strcpy(segmento, "STACK");
    }
    else
        strcpy(segmento, "CUAQUIERA");

    int direccion = logical_to_physical(mv->registros[LAR] , mv , 4,segmento);
    mv->registros[MAR] = direccion; // guardo la direccion fisica en los 2 bytes menos significativos
    mv->registros[MAR] +=(4<<29); //quedan los 2 bits mas significativos diciendo que vna a guardar 3 bytes

    if(direccion == -1 || (direccion + cant_bytes) > mv->MemSize){
        error_handler(SEGFAULT);
    }
    else{
        if(strcmp(segmento,"STACK") == 0){
            set_valor_pila(mv, direccion, valor, cant_bytes);
        }
        else{
            for(int i = 0; i<cant_bytes; i++){
                mv->ram[direccion + i] = (valor >> (8 * (3 - i))) & 0x000000FF;
            }
        }
    }
}

//------------------------------------FIN GETTERS Y SETTERS DE OPERANDOS--------------------------------------------------------------------------------

int creaDireccionLogica(int segmento, int offset){
    int puntero;
    puntero = segmento;
    puntero = puntero << 16;
    puntero |= (offset && 0x0000FFFF);
    return puntero;
}

void creaTablaSegmentos(MaquinaVirtual *mv,int param, int code, int data, int extra, int stack, int constant){
    int i = 0;
    for(i = 0; i < 8; i++){
        mv->seg[i][0] = -1;
        mv->seg[i][1] = -1;
    }
    i = 0;
    int offset = 0;
    mv->registros[PS] = -1;
    mv->registros[CS] = -1;
    mv->registros[DS] = -1;
    mv->registros[ES] = -1;
    mv->registros[SS] = -1;
    mv->registros[KS] = -1;
    if(param > 0){
        mv->seg[i][0] = offset;
        mv->seg[i][1] = param;
        i++;
        mv->registros[PS] = creaDireccionLogica(i, 0); // inicializo el puntero de ParamSegment al comienzo del segmento de parametros
        offset += param;
    }
    if(constant > 0){
        mv->seg[i][0] = offset;
        mv->seg[i][1] = offset + constant;
        mv->registros[KS] = offset; // inicializo el puntero de ConstantSegment al comienzo del segmento de constantes
        i++;
        offset += constant;
    }    

    mv->seg[i][0] = offset;
    mv->seg[i][1] = offset + code;
    mv->registros[CS] = creaDireccionLogica(i, 0); // inicializo el puntero de CodeSegment al comienzo del segmento de codigo
    i++;
    offset += code;

    if(data > 0){
        mv->seg[i][0] = offset;
        mv->seg[i][1] = offset + data;
        mv->registros[DS] = creaDireccionLogica(i, 0); // inicializo el puntero de DataSegment al comienzo del segmento de datos
        i++;
        offset += data;
    }
    if(extra > 0){
        mv->seg[i][0] = offset;
        mv->seg[i][1] = offset + extra;
        mv->registros[ES] = creaDireccionLogica(i, 0); // inicializo el puntero de ExtraSegment al comienzo del segmento extra
        i++;
        offset += extra;
    }
    if(stack > 0){
        mv->seg[i][0] = offset;
        mv->seg[i][1] = offset + stack;
        mv->registros[SS] = creaDireccionLogica(i, 0); // inicializo el puntero de StackSegment al comienzo del segmento de stack
        mv->registros[SP] = mv->registros[SS] + stack; // inicializo el puntero SP al final del segmento de stack
        i++;
        offset += stack;
    }

}
//saque los if del code y stack segment porque siempre van a estar segun la especificacion de la mv2


// CHEQUEAR, NO HAY NINGUN TIPO DE CHANCE DE QUE ESTO ANDE
void manejaArgumentos(int argc, char *argv[], char vmx[], char vmi[], int *d, int *p, int *argCMV, int argvMV, unsigned short int *paramSeg, MaquinaVirtual *mv){
    int i;
    mv->MemSize = 16384; // valor por defecto
    *d = 0;
    *p = 0;
    vmx[0] = '\0';  
    vmi[0] = '\0';
    *paramSeg = 0;
    if(argc < 2){
        error_handler(NOFILE);
    }
    for(i = 1; i < argc; i++){
        if (strstr(argv[i], ".vmx")) {
            strcpy(vmx, argv[i]);
        } 
        else if (strstr(argv[i], ".vmi")) {
            strcpy(vmi, argv[i]);
        } 
        else if (strncmp(argv[i], "m=", 2) == 0) {
            mv->MemSize = atoi(argv[i] + 2) * 1024;  // convertir KiB a bytes
        } 
        else if (strcmp(argv[i], "-d") == 0) {
            *d = 1;
        } 
        else if (strcmp(argv[i], "-p") == 0 && vmx[0] != '\0') {
            *p = 1;          
        }
        if(*p == 1){
            //guardo el string del parametro en el param segment
            for(int j = *paramSeg; i<= *paramSeg + strlen(argv[i]) ; j++){
                mv->ram[j] = (argv[i])[j];
            }
            // incremento la cantidad de argumentos ingresados
            (*argCMV)++;
            //aumento el tamanio del paramSegment en el tamanio del argumento ingresado
            *paramSeg += (strlen(argv[i]) + 1); // agrego el terminator
        }        
    }
     //agrego el puntero al primer argumento del param segment
    if(*p == 1){
            argvMV = (*paramSeg); int j = 0;
            for(i = 0; i<argCMV; i++){
                mv->ram[*paramSeg] = j;
                *paramSeg += 4;
                do
                {
                    j++;
                } while (mv->ram[j] != '\0');
            }
        }
    if(vmx[0] == '\0' && vmi[0] == '\0'){
        error_handler(NOFILE);
    }
       
    
}


// ----------------------------------------- IMAGEN ----------------------------------------------------

// ----------- LECTURA --------------


void leeImg(MaquinaVirtual *mv, char vmi[]){
    FILE *arch;
    int tamMem;
    char version;
    
    arch = fopen(vmi, "rb");
    if(arch){
        leeHeaderImg(&version, &tamMem, arch);
        leeRegistrosImg(mv, arch);
        leeTablaSegmentosImg(mv, arch);
        leeMemoriaImg(mv, arch, tamMem);
        fclose(arch);
    }
    else{
        printf("NO SE PUDO ABRIR EL ARCHIVO .vmi \n");
        error_handler(NOFILE);
    }
    
}



void leeHeaderImg(char *version, short int *tamMem, FILE *arch){
    char car;
    int i;
    //IMPRIME LA VERSION
    for(i = 0; i < 5; i++){
        fread(&car, sizeof(char), 1, arch);
        printf("%c", car);
    }
    printf("\n");
    
    // LEE LA VERSION
    fread(version, sizeof(char), 1, arch);
    printf("VERSION: %d\n", *version); 
    
    // LEE EL TAMAÑO DE MEMORIA EN KiB
    fread(tamMem, sizeof(short int), 1, arch);
    printf("MEMORIA: %d KiB\n", *tamMem);
    
    //PASO A BYTES
    *tamMem = (*tamMem) * 1024; 
}

void leeRegistrosImg(MaquinaVirtual *mv, FILE *arch){
    int i;
    int reg;
    //LEE EL VALOR DEL REIGSTRO i Y LO GUARDA EN LA MAQUINA VIRTUAL
    for(i = 0; i < 32; i++){
        fread(&reg, sizeof(int), 1, arch);
        mv->registros[i] = reg;
    }
}

void leeTablaSegmentosImg(MaquinaVirtual *mv, FILE *arch){
    int i;
    short int segBase, segLimit;
    //LEE CADA SEGMENTO (BASE Y LIMITE) Y LO GUARDA EN LA MAQUINA VIRTUAL
    for(i=0; i<8; i++){
        fread(&segBase, sizeof(short int), 1, arch);
        fread(&segLimit, sizeof(short int), 1, arch);
        mv->seg[i][0] = segBase;
        mv->seg[i][1] = segLimit;
    }
}

void leeMemoriaImg(MaquinaVirtual *mv, FILE *arch, short int tamMem){
    int i;
    char elem;
    //LEE LA MEMORIA DE LA IMAGEN Y LA GUARDA EN LA MAQUINA VIRTUAL BYTE A BYTE
    for(i = 0; i<tamMem; i++){
        fread(&elem, sizeof(char), 1, arch);
        mv->ram[i] = elem;
    }
}




//--------------- ESCRITURA ---------------

void escribeImg(MaquinaVirtual mv, char vmi[], char version, short int tamMem){
    //tamMem EN KiB
    FILE *arch;
    arch = fopen(vmi, "wb");
    if(arch){
        fclose(arch);
        escribeHeaderImg(version, tamMem, vmi);
        escribeRegistrosImg(mv, vmi);
        escribeTablaSegImg(mv, vmi);
        escribeMemoriaImg(mv, tamMem, vmi);
    }
    else{
        printf("NO SE PUDO ABRIR EL ARCHIVO .vmi");
        error_handler(NOFILE);
    }


}

void escribeHeaderImg(char version, short int tamMem, char vmi[]){
    char identificador[] = "VMI25";
    FILE *arch = fopen(vmi, "ab");
    //ESCRIBOEL IDENTIFICADOR
    fwrite(identificador, sizeof(char), strlen(identificador), arch);
    
    //ESCRIBO LA VERSION
    fwrite(&version, sizeof(char), 1, arch);
    
    //ESCRIBO EL TAMANO EN KiB
    fwrite(&tamMem, sizeof(short int), 1, arch);
    fclose(arch);

}

void escribeRegistrosImg(MaquinaVirtual mv, char vmi[]){
    int i;
    FILE *arch = fopen(vmi, "ab");
    //POR CADA REGISTRO ESCRIBO SUS 4 BYTES EN EL ARCHIVO
    for(i = 0; i < 32; i++){
        fwrite(&(mv.registros[i]), sizeof(int), 1, arch);
    }
    fclose(arch);
}

void escribeTablaSegImg(MaquinaVirtual mv, char vmi[]){
    int i;
    FILE *arch = fopen(vmi, "ab");
    for(i=0; i<8; i++){
        fwrite(&(mv.seg[i][0]), sizeof(short int), 1, arch);
        fwrite(&(mv.seg[i][1]), sizeof(short int), 1, arch);

    }

    fclose(arch);

}

void escribeMemoriaImg(MaquinaVirtual mv, short int tamMem, char vmi[]){
    int i;
    FILE *arch = fopen(vmi, "ab");
    //ESCRIBO CADA BYTE DE LA RAM EN EL ARCHIVO .vmi

    for(i = 0; i < tamMem *1024; i++){
        fwrite(&(mv.ram[i]), sizeof(char), 1, arch);
    }

    fclose(arch);
}

void breakPoint(MaquinaVirtual *mv, char vmiFileName[]){

    char inst;
    escribeImg(*mv, vmiFileName, 2, mv->MemSize / 1024);
    scanf("%c", &inst);

    while(inst == '\n' && mv->registros[IP] != -1){
        step(mv);
        escribeImg(*mv,vmiFileName,2, mv->MemSize / 1024);
        scanf("%c", &inst);
    }
    


    if(inst == 'q' || mv->registros[IP] == -1){ // q-> corta la ejecucion habiendo guardado la imagen
        exit(0);
    }
    else if(inst == 'g'){ // g-> continua la ejecucion hasta que termine el programa o haya otro break point
        return;
    }

}


void iniciaPila(MaquinaVirtual *mv, int argc, int argv){

    if(mv->registros[SP] < mv->registros[SS])
        error_handler(STACKOVER);
    else{
        int direccionargv = mv->registros[SP] - 4;
        int direccionargc = mv->registros[SP] - 8;
        direccionargv = logical_to_physical(direccionargv,mv,-4,"STACK");
        direccionargc = logical_to_physical(direccionargc,mv,-4,"STACK");
        
        for(int i = 0; i<4; i++){
            mv->ram[direccionargv + i] = (argv >> (8 * (3 - i))) & 0x000000FF;
            mv->ram[direccionargc + i] = (argc >> (8 * (3 - i))) & 0x000000FF;
        }
       

        mv->registros[SP] -= 8;
    }
    int direccionRetorno = -1 ;//GUARDO EL TIPO DE OPERANDO (MEMORIA) EN LOS 2 BITS MAS SIGNIFICATIVOS Y -1 EN EL RESTO DE BITS
    direccionRetorno = direccionRetorno & 0x0200FFFF;
    push(direccionRetorno, mv);
}

int get_valor_pila(MaquinaVirtual *mv, int direccion){
    int valor = 0;
    for(int i = 0; i<4; i++){
        valor = (valor << 8) | (mv->ram[direccion + i]&0x000000FF);

    }
    if(valor & 0x00008000) // si el bit 23 del valor es 1, es negativo
            valor = valor | 0xFFFF0000; // lo extiendo a 32 bits
    return valor;
}

void set_valor_pila(MaquinaVirtual *mv, int direccion, int valor){
    for(int i = 0; i<4; i++){
        mv->ram[direccion + i] = valor >> (8 * (3 - i)) & 0x000000FF;
    }
}




//fin vm.c