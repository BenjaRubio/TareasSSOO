#pragma once

typedef struct process {
    struct process* quehue_next;
    struct process* quehue_prev;
    char* name;
    int pid;
    int ti;
    int cycles;
    int wait;
    int waiting_delay;
    int s;

} Process;

void imprimir();