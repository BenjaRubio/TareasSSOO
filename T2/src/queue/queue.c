#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void enqueue(Process* process, Queue* queue) {
    if (queue->last) {
        process->queue_prev = queue->last;
        queue->last->queue_next = process;
        queue->last = process;
    }
    else {
        queue->first = process;
        queue->last = process;
    }
}

Process* fifo_dequeue(Queue* queue) { // ver que pasas si no retorna nada
    Process* exiting = queue->first;
    while (exiting)
    {
        if (exiting->state == "READY")
        {
            if (exiting->queue_next)
            {
                queue->first = exiting->queue_next;
                queue->first->queue_prev = NULL;
                exiting->queue_next = NULL;
            }
            else 
            {
                queue->first = NULL;
                queue->last = NULL;
            }
            return exiting;
            
        }
        if (exiting->queue_next)
        {
            exiting = exiting->queue_next;
        }
    }
}

Process* sjf_dequeue(Queue* queue)
{
    // retornar el proceso que le quede menos rafaga de CPU restante
    // verificar que el proceso se encuentre en READY
    Process* current = queue->first;
    Process* exiting = queue->first;
    while (current)
    {
        if (current->cycles - current->t < exiting->cycles - exiting->t && current->state == "READY")
        {
            exiting = current;
        }
        current = current->queue_next;
    }
    return exiting;

}

void destroy_queue(Queue* queue) {
    Process* current = queue->first;
    Process* following;
    while (current) {
        following = current->queue_next;
        free(current);
        current = following;
    }
    free(queue);
}

void print_queue(Queue* queue, char* name) {
    printf("\n%s elements in order:\n", name);
    Process* current = queue->first;
    Process* following;
    while (current) {
        following = current->queue_next;

        printf("Name: %s, id: %i, i_time: %i, cycles: %i, wait: %i, w_delay: %i, s: %i\n",
        current->name, current->pid, current->initial_time, current->cycles, current->wait, 
        current->waiting_delay, current->s);
        
        current = following;
    }
}

void actualize_state(Queue* queue)
{
    Process* p = queue->first;
    while (p)
    {
        if (p->waiting_time == p->waiting_delay)
        {
            p->state = "READY";
            p->waiting_time = 0;
        }
        p = p->queue_next;
    }
}

void enqueue_all(Queue* process_list, Queue* queue, int time)
{
    Process* current = process_list->first;
    Process* next;
    Process* prev;
    while (current)
    {
        next = current->queue_next;
        prev = current->queue_prev;
        if (time == current->initial_time)
        {
            enqueue(current, queue);
            next->queue_prev = prev;
            if (prev)
            {
                prev->queue_next = next;
            }

        }
        current = next;
    }
}