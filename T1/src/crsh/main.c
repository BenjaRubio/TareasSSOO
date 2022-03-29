#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// definicion de NULL tomada de https://techoverflow.net/2019/06/20/how-to-fix-c-error-null-undeclared/
#define NULL ((void *)0)

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "../input_manager/manager.h"
#include "func.h"
// #include "linked_list.h"

int main(int argc, char const *argv[])
{

  
  // pid_t arreglo_de_procesos[255];
  // time_t arreglo_de_tiempos[255];
  // for (int i = 0; i < 255; i++) 
  // {
  //   arreglo_de_procesos[i] = 0;
  //   arreglo_de_tiempos[i] = 0;
  // }
  pid_t* process_array = calloc(255, sizeof(pid_t));
  time_t* time_array = calloc(255, sizeof(time_t));
  int** status_array = malloc(255 * sizeof(int*));
  int process_counter = 0;
  
  while (1)
  {

    char **input = read_user_input();
    // printf("> The first argument you wrote was: %s\n", input[0]);
    pid_t pid_child;
    int* status = malloc(sizeof(int));
    
    
    if (strcmp(input[0], "hello") == 0)
    {
      // implementar funcion hello world
      pid_t pid_child = fork();
      if (pid_child == 0)
      {
        // funcion del proceso hijo
        printf("> Hello World!\n");
        // recordar terminarlo para que no siga en el while
        int my_id = getpid();
        exit(my_id);
      }
      waitpid(pid_child, status, WNOHANG);
      process_array[process_counter] = pid_child;
      time_array[process_counter] = time(NULL);
      status_array[process_counter] = status;
      process_counter++;
    }

    else if (strcmp(input[0], "sum") == 0)
    {
      // implementar funcion sum
      pid_t pid_child = fork();
      if (pid_child == 0)
      {
        // funcion del proceso hijo
        // se deben tomar el input[1] e input [2] y pasarse a double
        double x = atof(input[1]);
        double y = atof(input[2]);
        printf("> %f\n", x+y);
        // recordar terminarlo para que no siga en el while
        int my_id = getpid();
        exit(my_id);
      }
      waitpid(pid_child, status, WNOHANG);
      process_array[process_counter] = pid_child;
      time_array[process_counter] = time(NULL);
      status_array[process_counter] = status;
      process_counter++;
    
    }

    else if (strcmp(input[0], "is_prime") == 0)
    {
      // implementar funcion is_prime
      // hacer algo con el caso en que input[1] sea 0 o negativo
      int x = atoi(input[1]);
      if (x <= 0)
      {
        printf("> input inválido\n");
      }
      else
      {  
        pid_t pid_child = fork();
        if (pid_child == 0)
        {
          // funcion del proceso hijo
          if (1 == is_prime(x))
          {
            printf("> TRUE, %d es un número primo\n", x);
          }
          else
          {
            printf("> FALSE, %d no es un número primo\n", x);
          }
          // recordar terminarlo para que no siga en el while
          int my_id = getpid();
          exit(my_id);
        }
      }
      waitpid(pid_child, status, WNOHANG);
      process_array[process_counter] = pid_child;
      time_array[process_counter] = time(NULL);
      status_array[process_counter] = status;
      process_counter++;
      
      
    }

    else if (strcmp(input[0], "crexec") == 0)
    {
      // implementar funcion crexec
      pid_t pid_child = fork();
      if (pid_child == 0)
      {
        // funcion del proceso hijo
        // recordar terminarlo para que no siga en el while
        // debe ser terminado dentro de la funcion que ejecuta el exec
        // porque reemplazara lo que venga despues
        execv(input[1], &input[1]);
        perror("> crexec");
        // si hay error hay que terminar el proceso
        exit(0);
      }
      waitpid(pid_child, status, WNOHANG);
      process_array[process_counter] = pid_child;
      time_array[process_counter] = time(NULL);
      status_array[process_counter] = status;
      process_counter++;
    }

    else if (strcmp(input[0], "crlist") == 0)
    {
      // implementar funcion crlist
      for (int i = 0; i < process_counter; i++)
      {
        // int* status = malloc(sizeof(int));
        // waitpid(process_array[i], status, WNOHANG);
        time_t time_passed = time(NULL) - time_array[i];
        // int exited = WIFEXITED(status_array[i]);
        pid_t estado = waitpid(process_array[i], status, WNOHANG);
        if (estado == 0)
        {
          printf("process ID: %i | running time: %ld s | status: %d", 
          process_array[i], time_passed, *status);
          printf("%i\n", WIFEXITED(*status));
        }
        // printf("%d\n", WIFEXITED(status_array[i]));
        // free(status);
        

      }
      
    }

    else if (strcmp(input[0], "crexit") == 0)
    {
      // implementar funcion crexit
      pid_t pid_child = fork();
      if (pid_child == 0)
      {
        // funcion del proceso hijo
        // recordar terminarlo para que no siga en el while
      }

    }

    else
    {
      // implementar que lance una excepcion por comando invalido
      printf("> comando '%s' desconocido\n", input[0]);
    }
    free_user_input(input);
  }
  free(time_array);
  free(process_array);
  for (int i = 0; i < process_counter; i++){
    free(status_array[i]);
  }
  free(status_array);
}

