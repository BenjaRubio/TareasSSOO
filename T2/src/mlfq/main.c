#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h>
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	// Cola de procesos:
	Queue* process_list = calloc(1, sizeof(Queue));
	*process_list = init_queue(0, 0);

	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);
	char* output_file = (char *)argv[2];

	for (int i = 0; i < input_file->len; ++i)
	{

		char*** lines = input_file->lines;

		// save new process to heap
		Process* initialized = calloc(1, sizeof(Process));

		// assign node attributes
		*initialized = init_process(lines[i][0], atoi(lines[i][1]), atoi(lines[i][2]), atoi(lines[i][3]), atoi(lines[i][4]),
			atoi(lines[i][5]), atoi(lines[i][6]));
	
		enqueue(initialized, process_list);
		
	}

	int q_factor = atoi(argv[3]);
	Process* cpu = NULL;
	Queue* queue1 = calloc(1, sizeof(Queue));
	*queue1 = init_queue(q_factor, 2);
	Queue* queue2 = calloc(1, sizeof(Queue));
	*queue2 = init_queue(q_factor, 1);
	Queue* queue3 = calloc(1, sizeof(Queue));
	*queue3 = init_queue(q_factor, 0);

	Queue* final_list = calloc(1, sizeof(Queue));
	*final_list = init_queue(0, -1);

	int actual_time = 0;
	int quantum_time = 0;
	

	while (1)
	{
		// 1. Actualizar procesos que cumplan su I/O burst de WAITING a READY
		actualize_state(queue1);
		actualize_state(queue2);
		actualize_state(queue3);

		// 2. Si hay proceso en RUNNING actualizar su estado
		if (cpu)
		{	
			// printf("it %i: el proceso de id %i esta en la cpu\n", actual_time, cpu->pid);
			int quantum = cpu->priority * q_factor;
			check_s(cpu, actual_time); // ve si se cumple el s mientras el proceso está en running
			
			if (cpu->t == cpu->cycles)
			{
				// proceso termina
				// printf("it %i: proceso de id %i termino\n", actual_time, cpu->pid);
				cpu->state = "FINISHED";
				cpu->finish_time = actual_time;
				enqueue(cpu, final_list); // lo guardamos para el output
				cpu = NULL;
				if (check_finish(queue1, queue2, queue3, process_list))
				{
					break;
				}
			}
			else if (cpu->t > 0 && (cpu->t % cpu->wait) == 0)
			{
				// printf("it %i: proceso de id %i paso a waiting\n", actual_time, cpu->pid);
				// proceso pasa a WAITING
				cpu->state = "WAITING";
				// aumentar su prioridad
				// enqueue_all: asignar priority
				if (cpu->priority >= 1 || cpu->s_accomplished) 
				{
					// printf("it %i: proceso de id %i entra a queue1\n", actual_time, cpu->pid);
					// vuelve a queue1
					enqueue(cpu, queue1);
					cpu->s_accomplished = 0;
				}
				else if (cpu->priority == 0)
				{
					// pasa a queue2
					// printf("it %i: proceso de id %i entra a queue2\n", actual_time, cpu->pid);
					enqueue(cpu, queue2);
				}
				cpu = NULL;

			}
			else if (quantum_time == quantum && cpu->priority != 0) // se acaba el quantum
			{
				// mover a la cola correspondiente
				// printf("it %i: proceso de id %i termino su quantum\n", actual_time, cpu->pid);
				cpu->state = "READY";
				cpu->interrupts++;
				if (cpu->s_accomplished == 1)
				{
					enqueue(cpu, queue1);
					cpu->s_accomplished = 0;
				}
				else if (cpu->priority == 2)
				{
					// salio de la queue1 y se debe añadir a queue2
					enqueue(cpu, queue2);
				}
				else if (cpu->priority == 1)
				{
					// salio de la queue2 y se debe añadir a queue3
					enqueue(cpu, queue3);
				}
				cpu = NULL;
			}
		}

		// 3. Ingresar procesos a sus colas correspondientes
		
		// 3.1 Si un proceso salio de la cpu (arriba)
		// 3.2 Procesos que inician
		
		enqueue_all(process_list, queue1, actual_time);
		
		// 3.3 y 3.4 Envejecimiento 2da cola y 3ra cola:
		scan(queue1, queue2, queue3, actual_time);

		// 4. Ingresar proceso a la CPU si corresponde
		if (!cpu) // si la cpu está vacía ingresamos un proceso
		{
			int cpu_added = 0;
			// printf("it %i: Intento ingresar un nuevo proceso\n", actual_time);
			if (queue1->first)
			{
				// queue1 tiene procesos, revisamos si hay uno en ready que pueda entrar
				Process* p = fifo_dequeue(queue1);
				if (p)
				{
					cpu = p;
					quantum_time = 0;
					cpu_added = 1;
					add_to_cpu(cpu, actual_time);
					// printf("it %i: queue1: proceso de id %i entro a la cpu\n", actual_time, cpu->pid);
				}
	
			}
			if (queue2->first && cpu_added == 0)
			{
				// queue2 tiene procesos, revisamos si hay uno en ready que pueda entrar
				Process* p = fifo_dequeue(queue2);
				if (p)
				{
					cpu = p;
					cpu_added = 1;
					quantum_time = 0;
					add_to_cpu(cpu, actual_time);
					// printf("it %i: queue2: proceso de id %i entro a la cpu\n", actual_time, cpu->pid);
				}
			}
			if (queue3->first && cpu_added == 0)
			{
				// queue3 tiene procesos, revisamos si hay uno en ready que pueda entrar
				Process* p = sjf_dequeue(queue3);
				if (p)
				{
					cpu = p;
					cpu_added = 1;
					quantum_time = 0;
					add_to_cpu(cpu, actual_time);
					// printf("it %i: queue3: proceso de id %i entro a la cpu\n", actual_time, cpu->pid);
				}
			}
		}
		
		if (cpu) // sino se ingresa proceso, ya hay uno y se ejecuta normalmente
		{
			cpu->t++;
			quantum_time++;
		}
		
		// aumentar los waiting_time de los procesos en waiting
		check_waiting(queue1);
		check_waiting(queue2);
		check_waiting(queue3);

		actual_time++;
	}

	

	write_to_file(output_file, final_list);




	

	free(process_list);
	free(queue1);
	free(queue2);
	free(queue3);
	destroy_queue(final_list);
	
	input_file_destroy(input_file);
}