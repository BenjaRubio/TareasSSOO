#pragma once

typedef struct process {
    int pid;
    char* name;
    int priority;
    int initial_time;
    int cycles;
    int wait;
    int waiting_delay;
    int t; //tiempo de ejecución que lleva el proceso
    char* state;
    int S;
} Process;

void add_to_cpu(Process* process);
Process init_process(char* name, int pid, int initial_time, int cycles, 
    int wait, int waiting_delay, int S);