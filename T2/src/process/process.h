#pragma once

typedef struct process {
    struct process* queue_next;
    struct process* queue_prev;
    int pid;
    char* name;
    // int priority;
    int initial_time;
    int cycles;
    int wait;
    int waiting_delay;
    int t; //tiempo de ejecución que lleva el proceso
    char* state;
    int s;
} Process;

void add_to_cpu(Process* process);
Process init_process(char* name, int pid, int initial_time, int cycles, 
    int wait, int waiting_delay, int S);

void imprimir();

