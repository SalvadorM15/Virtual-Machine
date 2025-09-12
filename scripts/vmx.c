#include <stdio.h>
#include <stdlib.h>
#include "VM.h"



void main(){

    MaquinaVirtual mv;
    lectura_arch(&mv);

    do{
        step(&mv);
    }while(mv.registros[IP] > -1 && mv.registros[IP]<logical_to_physical(get_logical_dir(mv,mv.registros[DS]),mv.seg,MEM));
    
    
}



