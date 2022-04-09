#include "queue.h"


Queue init_queue(int max_len, int priority, int q)
{
    Queue queue = (Queue){.process = malloc(max_len * sizeof(Process)), .priority = priority,
	 .quantum = priority*q, .len = 0};
    return queue;
}


void enqueue(Queue* queue, Process process)
{
    // agrega el proceso al final de la cola
    int len = queue->len;
    queue->process[len] = process;
    queue->len += 1;
}

Process fifo_dequeue(Queue* queue)
{
    // antes de usarse esta función se debe comprbar que hay elementos en la cola
    // entrega el primer proceso de la cola y mueve los restantes un lugar hacia adelante
    int len = queue->len;
    Process process = queue->process[0];
    for (int i = 1; i < len; i++)
    {
        queue->process[i-1] = queue->process[i];
    }
    queue->len -= 1;
    return process;
    
}

Process sjf_dequeue(Queue* queue)
{
    Process p = queue->process[0];
    if (queue->len == 1)
    {
        queue->len -= 1;
        return p;
    }
    int index;
    // busca el proceso que le queda menos CPU-burst
    for (int i = 0; i < queue -> len; i++)
    {
        if (queue->process[i].t - queue->process[i].cycles < p.t - p.cycles)
        {
            p = queue->process[i];
            index = i;
        }
    }
    // mueve los procesos más atrás un espacio mas adelante en la cola
    for (int i = index + 1; i < queue->len; i++)
    {
        queue->process[i-1] = queue->process[i];
    }
    queue->len -= 1;
    return p;
}

