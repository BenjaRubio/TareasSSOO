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
	*queue1 = (Queue){.process = malloc(input_file->len * sizeof(Process)), .priority = 2,
	 .quantum = 2*q, .len = 0};
	Queue* queue2 = malloc(sizeof(Queue));
	*queue2 = (Queue){.process = malloc(input_file->len * sizeof(Process)), .priority = 1,
	 .quantum = q, .len = 0};
	Queue* queue3 = malloc(sizeof(Queue));
	*queue3 = (Queue){.process = malloc(input_file->len * sizeof(Process)), .priority = 0,
	 .quantum = 0, .len = 0};



	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("%s ", input_file->lines[i][j]);
		}
		printf("\n");
	}

	input_file_destroy(input_file);

	free(queue1 -> process);
	free(queue1);
	free(queue2 -> process);
	free(queue2);
	free(queue3 -> process);
	free(queue3);
}