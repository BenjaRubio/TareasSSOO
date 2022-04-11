#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void enqueue(Process* process, Queue* queue) {
    if (queue->last) {
        process->queue_prev = queue->last;
        queue->last->queue_next = process;
        queue->last = process;
        process->queue_next = 0;
    }
    else {
        queue->first = process;
        queue->last = process;
        process->queue_prev = 0;
        process->queue_next = 0;
    }
}

Process* fifo_dequeue(Queue* queue) { // ver que pasas si no retorna nada
    Process* exiting = queue->first;
    while (exiting)
    {
        if (exiting->state == "READY")
        {
            if (exiting->queue_next) // if more left in queue
            {
                queue->first = exiting->queue_next;
                queue->first->queue_prev = NULL;
                exiting->queue_next = NULL;
                exiting->queue_prev = NULL;
            }
            else // queue left empty
            {
                queue->first = NULL;
                queue->last = NULL;
                exiting->queue_next = NULL;
                exiting->queue_prev = NULL;
            }
            return exiting;
            
        }
        else if (exiting->queue_next)
        {
            exiting = exiting->queue_next;
        }
        else
        {
            return NULL;
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

static void scan_one(Queue* queue1, Queue* queuex, int current_t) {
    // recorremos queuex
    Process* scanned = queuex->first;
    Process* following;

    while (scanned) {
        following = scanned->queue_next;

        if ( (current_t - scanned->initial_time) % scanned->s == 0 ) { // scanned se va
            if (scanned->queue_prev && scanned->queue_next) { // si esta en medio
                Process* prev = scanned->queue_prev;
                Process* next = scanned->queue_next;
                prev->queue_next = next;
                next->queue_prev = prev;
                scanned->queue_next = 0;
                scanned->queue_prev = 0;
            }
            else if (scanned->queue_next) {// si es el primero
                scanned = dequeue(queuex);
            }
            else if (scanned->queue_prev) { // si es el ultimo
                queuex->last = scanned->queue_prev;
                queuex->last->queue_next = 0;
                scanned->queue_next = 0;
                scanned->queue_prev = 0;
            }
            else { // si es el unico
                queuex->first = 0;
                queuex->last = 0;
                scanned->queue_next = 0;
                scanned->queue_prev = 0;
            }
            // ya lo sacamos, toca meterlo en queue1
            enqueue(scanned, queue1);
        }
        scanned = following;
    }

}

void scan(Queue* queue1, Queue* queue2, Queue* queue3, int current_t) {
    // scan queue3:
    scan_one(queue1, queue3, current_t);
    // scan queue2:
    scan_one(queue1, queue2, current_t);
}

void destroy_queue(Queue* queue) {
    if (queue->first) {
        Process* current = queue->first;
        Process* aux;
        while (current) {
            aux = current;
            current = current->queue_next;
            free(aux);
        }
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