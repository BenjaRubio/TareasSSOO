#pragma once
#include "../process/process.h"

typedef struct queue {
    Process* process;
    int priority;
    int quantum;
    int len;
} Queue;

Process less_burst_process(Queue* queue, int cycle);
void enqueue(Queue* queue, Process process);
Process dequeue(Queue* queue);