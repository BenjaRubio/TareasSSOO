#pragma once

typedef struct process {
    struct process* queue_next;
    struct process* queue_prev;
    int pid;
    char* name;
    int initial_time;
    int cycles;
    int wait;
    int waiting_delay;
    int t; // tiempo de ejecución que lleva el proceso en total
    int t_cpu; // tiempo que lleva en la cpu 
    char* state;
    int s;
    int waiting_time;
    int priority;
} Process;

void add_to_cpu(Process* process, Process* cpu);
Process init_process(char* name, int pid, int initial_time, int cycles, 
    int wait, int waiting_delay, int S);


void imprimir();

