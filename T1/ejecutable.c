#include "ejecutable.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    // devolveremos la suma de todos los numeros entregados
    double total = 0;
    for (int i = 1; i < argc; i++)
    {
        total += atof(argv[i]);
    }
    printf("El total de la suma es %f\n", total);
    


    return 0;
}