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