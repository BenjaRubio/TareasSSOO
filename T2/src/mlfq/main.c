#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	// Cola de procesos:
	Queue* process_list = calloc(1, sizeof(Queue));
	Process* cpu = 0;

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
		*initialized = (Process) {.name = lines[i][0], .pid = atoi(lines[i][1]), .initial_time = atoi(lines[i][2]),
		.cycles = atoi(lines[i][3]), .wait = atoi(lines[i][4]), .waiting_delay = atoi(lines[i][5]), 
		.s = atoi(lines[i][6])};
		
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

	print_queue(process_list);
	destroy_queue(process_list);

	input_file_destroy(input_file);

	// free(queue1 -> process);
	// free(queue1);
	// free(queue2 -> process);
	// free(queue2);
	// free(queue3 -> process);
	// free(queue3);
}