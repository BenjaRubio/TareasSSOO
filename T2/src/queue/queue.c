#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void destroy_quehue(Quehue* quehue) {
    Process* current = quehue->first;
    Process* following;
    while (current) {
        following = current->quehue_next;
        free(current);
        current = following;
    }
    free(quehue);
}

void print_quehue(Quehue* quehue) {
    printf("Quehue elements in order:\n");
    Process* current = quehue->first;
    Process* following;
    while (current) {
        following = current->quehue_next;

        printf("Name: %s, id: %i, i_time: %i, cycles: %i, wait: %i, w_delay: %i, s: %i\n",
        current->name, current->pid, current->ti, current->cycles, current->wait, 
        current->waiting_delay, current->s);
        
        current = following;
    }
}