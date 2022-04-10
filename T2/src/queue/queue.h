#pragma once
#include "../process/process.h"

typedef struct queue {
    Process* first;
    Process* last;
    int quantum;
} Queue;

// add process to queue
void enqueue(Process* process, Queue* queue);
// remove process from queue and return pointer
Process* dequeue(Queue* queue);
// pendiente: scan for old processes
void scan(Queue* queue1, Queue* queue2, Queue* queue3);

void destroy_queue(Queue* queue);
void print_queue(Queue* queue, char* name);

// typedef struct queue {
//     Process* process;
//     int priority;
//     int quantum;
//     int len;
// } Queue;


// Queue init_queue(int max_len, int priority, int q);
// void enqueue(Queue* queue, Process process);
// Process fifo_dequeue(Queue* queue);
// Process sjf_dequeue(Queue* queue);