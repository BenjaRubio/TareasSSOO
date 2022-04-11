#include "process.h"
#include <stdio.h>

void add_to_cpu(Process* process, Process* cpu)
{
    process->state = "RUNNING";
    *cpu = *process;
}

Process init_process(char* name, int pid, int initial_time, int cycles, 
    int wait, int waiting_delay, int s)
{
    Process process = (Process){
        .pid = pid, 
        .name = name, 
        // .priority = 2, // todos los procesos entran a la primera cola
        .initial_time = initial_time,
        .cycles = cycles, 
        .wait = wait, 
        .waiting_delay = waiting_delay, 
        .t = 0, 
        .state = "READY",
        .s = s};
    return process;
}
