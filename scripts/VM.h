#include <stdio.h>
#define REG 32
#define MEM 4216
#define MAX 10

/*crear constantes con los nombres de los registros para no tener que 
acordarse los nombres despues*/

typedef struct{
    int registros[REG];
    int ram[MEM];
    int seg[MAX][2];
 } MaquinaVirtual;

