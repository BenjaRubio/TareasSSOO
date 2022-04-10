#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	// Cola de procesos:
	Quehue* process_list = calloc(1, sizeof(Quehue));

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

		// save new process node to heap
		Process* initialized = calloc(1, sizeof(Process));
		// assign node attributes
		*initialized = (Process) {.name = lines[i][0], .pid = atoi(lines[i][1]), .ti = atoi(lines[i][2]),
		.cycles = atoi(lines[i][3]), .wait = atoi(lines[i][4]), .waiting_delay = atoi(lines[i][5]), 
		.s = atoi(lines[i][6])};
		
		//  if list is empty, assign as first and last
		if (!process_list->first) {
			process_list->first = initialized;
			process_list->last = initialized;
		}
		else {
			process_list->last->quehue_next = initialized;
			initialized->quehue_prev = process_list->last;
			process_list->last = initialized;
		}
		
		printf("\n");
	}

	print_quehue(process_list);
	destroy_quehue(process_list);

	input_file_destroy(input_file);
}