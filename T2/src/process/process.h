#pragma once

typedef struct process {
    struct process* queue_next;
    struct process* queue_prev;
    char* name;
    int pid;
    int initial_time;
    int cycles;
    int wait;
    int waiting_delay;
    int s;
    int t; // tiempo de ejecución que lleva el proceso en total
    char* state;
    int waiting_time;
    int priority;
    int s_accomplished;
    int t_cpu; // veces que el proceso fue elegido para usar la cpu
    int interrupts;
    int finish_time;
    int first_cpu_time;
    int total_time;
} Process;

void check_s(Process* process, int current_time);
Process init_process(char* name, int pid, int initial_time, int cycles, int wait, int waiting_delay, int s);
void add_to_cpu(Process* process, int current_time);
int turnaorund_time(Process* process);
int response_time(Process* process);




