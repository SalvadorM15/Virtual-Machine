#include<stdio.h>

//CONSTANTES AUXILIARES QUE CREE PARA TESTEAR LA FUNCION


const MAX = 2;
const seg_table[2][2] = {0, 0x0033, 0x0033, 0xFFCD};

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



