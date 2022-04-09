#include "queue.h"

void enqueue(Queue* queue, Process process)
{
    int len = queue->len;
    queue->process[len] = process;
    queue->len += 1;
}

Process dequeue(Queue* queue)
{
    int len = queue->len;
    if (len > 0)
    {
        Process process = queue->process[len - 1];
        queue->len -= 1;
    }
}

Process less_burst_process(Queue* queue, int cycle)
{
    // debe encontrar el proceso, moverlos todos, y restar uno al len
    Process p = queue->process[0];
    for (int i = 0; i < queue -> len; i++)
    {
        if (queue->process[i].t - queue->process[i].cycles < p.t - p.cycles)
        {
            p = queue->process[i];
        }
    }
    return p;
}