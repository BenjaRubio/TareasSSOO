#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

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

void print_queue(Queue* queue) {
    printf("Queue elements in order:\n");
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
