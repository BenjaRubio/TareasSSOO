#include "process.h"
#include <stdio.h>
#include <string.h>
#include "../queue/queue.h"


Process init_process(char* name, int pid, int initial_time, int cycles, 
    int wait, int waiting_delay, int s)
{
    Process process = (Process){
        .queue_next = NULL,
        .queue_prev = NULL,
        .name = name,
        .pid = pid, 
        .initial_time = initial_time,
        .cycles = cycles, 
        .wait = wait, 
        .waiting_delay = waiting_delay, 
        .s = s,
        .t = 0, 
        .state = "READY",
        .waiting_time = 0,
        .priority = 2,
        .s_accomplished = 0,
        .t_cpu = 0,
        .interrupts = 0,
        .finish_time = 0,
        .first_cpu_time = 0,
        .total_time = 0};
    return process;
}



void check_s(Process* process, int current_time)
{
    if ((current_time - process->initial_time) % process->s == 0)
    {
        process->s_accomplished = 1;
    }
}



void add_to_cpu(Process* process, int current_time)
{
    process->state = "RUNNING";
    process->t_cpu++;
    if (!process->first_cpu_time)
    {
        process->first_cpu_time = current_time;
    }
}

int turnaorund_time(Process* process)
{
    return process->finish_time - process->initial_time;
}

int response_time(Process* process)
{
    return process->first_cpu_time - process->initial_time;
}

int waiting_time(Process* process)
{
    return process->total_time + process->t; // tiempo total en waiting + tiempo de ejecucion en cpu
}
