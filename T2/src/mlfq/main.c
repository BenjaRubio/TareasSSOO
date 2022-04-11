#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	// Cola de procesos:
	Queue* process_list = calloc(1, sizeof(Queue));
	*process_list = (Queue){.first = NULL, .last = NULL, .quantum = 0};

	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("Procesos:\n");

	for (int i = 0; i < input_file->len; ++i)
	{

		for (int j = 0; j < 7; ++j)
		{
			printf("%s ", input_file->lines[i][j]);

		}
		char*** lines = input_file->lines;

		// save new process to heap
		Process* initialized = calloc(1, sizeof(Process));

		// assign node attributes
		*initialized = (Process) {.queue_next = NULL, .queue_prev = NULL, .name = lines[i][0], .pid = atoi(lines[i][1]),
		 .initial_time = atoi(lines[i][2]), .cycles = atoi(lines[i][3]), .wait = atoi(lines[i][4]),
		 .waiting_delay = atoi(lines[i][5]), .s = atoi(lines[i][6]), .t = 0, .state = "READY", .waiting_time = 0,
		 .priority = 2};
		
		//  if list is empty, assign as first and last
		if (!process_list->first) {
			process_list->first = initialized;
			process_list->last = initialized;
		} //else append
		else {
			process_list->last->queue_next = initialized;
			initialized->queue_prev = process_list->last;
			process_list->last = initialized;
		}
		
		printf("\n");
	}

	int q_factor = atoi(argv[3]);
	Process* cpu = calloc(1, sizeof(Process*));
	Queue* queue1 = calloc(1, sizeof(Queue));
	*queue1 = (Queue){.first = NULL, .last = NULL, .quantum = q_factor * 2};
	Queue* queue2 = calloc(1, sizeof(Queue));
	*queue2 = (Queue){.first = NULL, .last = NULL, .quantum = q_factor};
	Queue* queue3 = calloc(1, sizeof(Queue));
	*queue3 = (Queue){.first = NULL, .last = NULL, .quantum = 0};

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
			int quantum = cpu->priority * q_factor;
			div_t d = div(cpu->t, cpu->wait);
			if (cpu->t == cpu->cycles)
			{
				// proceso termina
			}
			else if (d.rem == 0)
			{
				// proceso pasa a WAITING
				// aumentar su prioridad
			}
			else if (quantum_time == quantum) // se acaba el quantum
			{
				// el if aun no esta bien hecho
				// mover a la cola correspondiente
				quantum_time = 0;
				if (cpu->priority == 2)
				{
					// salio de la queue1 y se debe añadir a queue2
				}
				else if (cpu->priority == 1)
				{
					// salio de la queue2 y se debe añadir a queue3
				}
				else 
				{
					// salio de la queue 3 y debe volver a la queue3
				}
			}
		}

		// 3. Ingresar procesos a sus colas correspondientes
		enqueue_all(process_list, queue1, actual_time);

		// 4. Ingresar proceso a la CPU si corresponde

		// chequear condicion de termino
		actual_time++;
	}




	

	destroy_queue(process_list);
	destroy_queue(queue1);
	destroy_queue(queue2);
	destroy_queue(queue3);
	free(cpu);
	
	input_file_destroy(input_file);

	// free(queue1 -> process);
	// free(queue1);
	// free(queue2 -> process);
	// free(queue2);
	// free(queue3 -> process);
	// free(queue3);
}