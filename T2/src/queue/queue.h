#pragma once
#include "../process/process.h"

typedef struct queue {
    Process* first;
    Process* last;
    int quantum;
    int priority;
} Queue;

Queue init_queue(int q, int priority);
// add process to queue
void enqueue(Process* process, Queue* queue);
// remove process from queue and return pointer
Process* fifo_dequeue(Queue* queue);
Process* sjf_dequeue(Queue* queue);
// pendiente: scan for old processes
void scan(Queue* queue1, Queue* queue2, Queue* queue3, int current_t);

void destroy_queue(Queue* queue);
void print_queue(Queue* queue, char* name);

void actualize_state(Queue* queue);
void enqueue_all(Queue* process_list, Queue* queue, int time);
void check_waiting(Queue* queue);

void write_to_file(const char* file, Queue* queue);


int check_finish(Queue* queue1, Queue* queue2, Queue* queue3, Queue* process_list);

