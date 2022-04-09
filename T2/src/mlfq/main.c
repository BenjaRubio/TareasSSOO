#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);
	int q = (int)argv[3];

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("Procesos:\n");

	Queue* queue1 = malloc(sizeof(Queue));
	*queue1 = init_queue(input_file->len, 2, q);

	Queue* queue2 = malloc(sizeof(Queue));
	*queue2 = init_queue(input_file->len, 1, q);

	Queue* queue3 = malloc(sizeof(Queue));
	*queue3 = init_queue(input_file->len, 0, q);

	Queue* processes = malloc(sizeof(Queue));
	*processes = init_queue(input_file->len, 0, q);

	//se crean procesos y agregan a la cola de todos los procesos
	for (int i = 0; i < input_file->len; ++i)
	{
		
		char* name = (char *) input_file->lines[i][0];
		int pid = (int) input_file->lines[i][1];
		int initial_time = (int) input_file->lines[i][2];
		int cycles = (int) input_file->lines[i][3];
		int wait = (int) input_file->lines[i][4];
		int waiting_delay = (int) input_file->lines[i][5];
		int s = (int) input_file->lines[i][6];
		Process p = init_process(name, pid, initial_time, cycles, wait, waiting_delay, s);
		processes->process[i] = p;
		processes->len += 1;
	}



	input_file_destroy(input_file);

	free(queue1 -> process);
	free(queue1);
	free(queue2 -> process);
	free(queue2);
	free(queue3 -> process);
	free(queue3);
}