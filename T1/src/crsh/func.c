#include <stdio.h>
#include <stdlib.h>

#include "func.h"

int is_prime(int n){
    // return 1 si n es primo
    // return 0 si no
    div_t result;
    for (int i = 2; i < n; i++)
    {
        result = div(n, i);
        if (result.rem == 0)
        {
            return 0;
        }
    }
    return 1;
}