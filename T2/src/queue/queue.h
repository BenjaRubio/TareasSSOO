#pragma once
#include "../process/process.h"

typedef struct queue {
    Process* first;
    Process* last;
} Queue;

void destroy_queue(Queue* queue);
void print_queue(Queue* queue);

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