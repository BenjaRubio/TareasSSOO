#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


Queue init_queue(int q, int priority)
{
    Queue queue = (Queue){.first = NULL, .last = NULL, .quantum = q * priority, .priority = priority};
    return queue;
}

void enqueue(Process* process, Queue* queue) {
    if (queue->last) {
        process->queue_prev = queue->last;
        queue->last->queue_next = process;
        queue->last = process;
        process->queue_next = NULL;
    }
    else {
        queue->first = process;
        queue->last = process;
        process->queue_prev = NULL;
        process->queue_next = NULL;
    }
    
    process->priority = queue->priority;
}

Process* brute_dequeue(Queue* queue) {
    Process* exiting = queue->first;
    if (exiting->queue_next) { // if more left in queue:
        queue->first = exiting->queue_next;
        queue->first->queue_prev = NULL;
    }
    else { // queue left empty
        queue->first = NULL;
        queue->last = NULL;
    }
    exiting->queue_next = NULL;
    exiting->queue_prev = NULL;
    return exiting;
}

Process* fifo_dequeue(Queue* queue) { // ver que pasas si no retorna nada
    Process* exiting = queue->first;
    while (exiting)
    {
        if (strcmp(exiting->state, "READY") == 0) // strings iguales
        {
            Process* prev = exiting->queue_prev;
            Process* next = exiting->queue_next;
            if (prev && next) // esta al medio
            {
                prev->queue_next = next;
                next->queue_prev = prev;
            }
            else if (prev && !next) // esta al final
            {
                prev->queue_next = NULL;
                queue->last = prev;
            }
            else if (!prev && next) // esta al inicio
            {
                next->queue_prev = NULL;
                queue->first = next;
            }
            else // esta solo
            {
                queue->first = NULL;
                queue->last = NULL;
            }
            exiting->queue_next = NULL;
            exiting->queue_prev = NULL;
            return exiting;
            
        }
        
        exiting = exiting->queue_next;
        
    }
    return NULL;
}

Process* sjf_dequeue(Queue* queue)
{
    // retornar el proceso que le quede menos rafaga de CPU restante
    // verificar que el proceso se encuentre en READY
    Process* current = queue->first;
    Process* exiting = queue->first;
    while (current)
    {
        if (current->cycles - current->t < exiting->cycles - exiting->t && strcmp(current->state, "READY") == 0)
        {
            exiting = current;
        }
        current = current->queue_next;
    }
    if (strcmp(exiting->state, "READY") == 0)
    {
        Process* prev = exiting->queue_prev;
        Process* next = exiting->queue_next;
        if (prev && next) // está el medio
        {
            prev->queue_next = next;
            next->queue_prev = prev;
        }
        else if (prev && !next) // esta al final
        {
            prev->queue_next = NULL;
            queue->last = prev;
        }
        else if (!prev && next) // etá al inicio
        {
            next->queue_prev = NULL;
            queue->first = next;
        }
        else // esta solo
        {
            queue->first = NULL;
            queue->last = NULL;
        }
        exiting->queue_next = NULL;
        exiting->queue_prev = NULL;
        return exiting;
    }
    else
    {
        return NULL;
    }
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
            }
            else if (scanned->queue_next) {// si es el primero
                scanned = brute_dequeue(queuex);
            }
            else if (scanned->queue_prev) { // si es el ultimo
                queuex->last = scanned->queue_prev;
                queuex->last->queue_next = NULL;
            }
            else { // si es el unico
                queuex->first = NULL;
                queuex->last = NULL;
            }
            scanned->queue_next = NULL;
            scanned->queue_prev = NULL;
            // ya lo sacamos, toca meterlo en queue1
            // printf("Enqueue process id: %i\n", scanned->pid);
            enqueue(scanned, queue1);
        }
        scanned = following;
    }

}

void scan(Queue* queue1, Queue* queue2, Queue* queue3, int current_t) {
    // scan queue3:
    // printf("scan queue2\n");
    scan_one(queue1, queue2, current_t);
    // scan queue2:
    // printf("scan queue3\n");
    scan_one(queue1, queue3, current_t);
}

void destroy_queue(Queue* queue) {
    // if (queue->first) {
    //     Process* current = queue->first;
    //     Process* aux;
    //     while (current) {
    //         aux = current;
    //         current = current->queue_next;
    //         free(aux);
    //     }
    // }
    // free(queue);
    Process* current = queue->first;
    
    while (current)
    {
        Process* next = current->queue_next;
        free(current);
        current = next;
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
            // p->total_time += p->waiting_time;
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
            if (next) 
            {
                next->queue_prev = prev;
            }
            else // si no hay next hay que actualizar last
            {
                process_list->last = prev;   
            }
            if (prev)
            {
                prev->queue_next = next;
            }

            else // si no hay prev hay que actualizar first
            {
                process_list->first = next;
            }
            enqueue(current, queue);
        }
        current = next;
    }
}


void check_waiting(Queue* queue)
{
    Process* p = queue->first;
    while (p)
    {
        if (strcmp(p->state, "WAITING") == 0)
        {
            p->waiting_time++;
        }
        p->total_time++; // se le suma al tiempo total ya ue en las colas los procesos olo estna WAITING o READY
        p = p->queue_next;
    }
}

void write_to_file(const char* file, Queue* queue)
{
    FILE* file_pointer = fopen(file, "w");

    if (queue->first) {
        Process* current = queue->first;
        Process* following;
        while (current) {
            following = current->queue_next;
            char* string;
            int n_chars;
            // formatear la linea aqui:
            int turnaround = turnaorund_time(current);
            int response = response_time(current);
            int waiting = waiting_time(current);
            n_chars = asprintf(&string, "%s,%d,%d,%d,%d,%d\n", current->name, current->t_cpu, current->interrupts,
                turnaround, response, waiting);
            fwrite(string, 1, n_chars, file_pointer);
            free(string);
            current = following;
        }
    }
    fclose(file_pointer);
}

int check_finish(Queue* queue1, Queue* queue2, Queue* queue3, Queue* process_list)
{
    if (!queue1->first && !queue2->first && !queue3->first && !process_list->first)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}