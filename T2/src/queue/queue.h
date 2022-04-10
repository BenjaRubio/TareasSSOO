#pragma once
#include "../process/process.h"

typedef struct quehue {
    Process* first;
    Process* last;
} Quehue;

void destroy_quehue(Quehue* quehue);
void print_quehue(Quehue* quehue);