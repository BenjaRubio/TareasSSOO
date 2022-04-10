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

Process* dequeue(Queue* queue) {
    Process* exiting = queue->first;
    if (exiting->queue_next) { // if more left in queue:
        queue->first = exiting->queue_next;
        queue->first->queue_prev = 0;
        exiting->queue_next = 0;
        exiting->queue_prev = 0;
    }
    else { // queue left empty
        queue->first = 0;
        queue->last = 0;
        exiting->queue_next = 0;
        exiting->queue_prev = 0;
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
// Queue init_queue(int max_len, int priority, int q)
// {
//     Queue queue = (Queue){.process = malloc(max_len * sizeof(Process)), .priority = priority,
// 	 .quantum = priority*q, .len = 0};
//     return queue;
// }


// void enqueue(Queue* queue, Process process)
// {
//     // agrega el proceso al final de la cola
//     int len = queue->len;
//     queue->process[len] = process;
//     queue->len += 1;
// }

// Process fifo_dequeue(Queue* queue)
// {
//     // antes de usarse esta función se debe comprbar que hay elementos en la cola
//     // entrega el primer proceso de la cola y mueve los restantes un lugar hacia adelante
//     int len = queue->len;
//     Process process = queue->process[0];
//     for (int i = 1; i < len; i++)
//     {
//         queue->process[i-1] = queue->process[i];
//     }
//     queue->len -= 1;
//     return process;
    
// }

// Process sjf_dequeue(Queue* queue)
// {
//     Process p = queue->process[0];
//     if (queue->len == 1)
//     {
//         queue->len -= 1;
//         return p;
//     }
//     int index;
//     // busca el proceso que le queda menos CPU-burst
//     for (int i = 0; i < queue -> len; i++)
//     {
//         if (queue->process[i].t - queue->process[i].cycles < p.t - p.cycles)
//         {
//             p = queue->process[i];
//             index = i;
//         }
//     }
//     // mueve los procesos más atrás un espacio mas adelante en la cola
//     for (int i = index + 1; i < queue->len; i++)
//     {
//         queue->process[i-1] = queue->process[i];
//     }
//     queue->len -= 1;
//     return p;
// }
