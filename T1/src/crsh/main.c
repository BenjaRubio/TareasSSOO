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
  int status;
  pid_t pid_child;

  pid_t process_array[255];
  time_t time_array[255];
  // int status_array[255];
  char* names_array[255];
  for (int i = 0; i < 255; i++) 
  {
    process_array[i] = -1;
    time_array[i] = 0;
    // status_array[i] = -1;
    names_array[i] = "unrecognized";
  }
  
  while (1)
  {
    // Revisamos todos los procesos y liberamos los que terminaron:
    for (int i=0; i < 255; i++)
    {
      pid_t stopped_running = waitpid(process_array[i], &status, WNOHANG);
      // pid_t stopped_running = 1;
      if (stopped_running)
      {
        // con p_a = -1, permite sobreescribirlo
        process_array[i] = -1;
      }
    }
    printf("> ");
    char **input = read_user_input();
    // printf("> The first argument you wrote was: %s\n", input[0]);
    
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
      // waitpid(pid_child, status, WNOHANG);
      // process_array[process_counter] = pid_child;
      // time_array[process_counter] = time(NULL);
      // status_array[process_counter] = status;
      // process_counter++;
      // buscamos un lugar donde guardarlo
      for (int i = 0; i < 255; i++)
      {
        if (process_array[i] == -1)
        {
          process_array[i] = pid_child;
          time_array[i] = time(NULL);
          names_array[i] = "hello";
          break;
        }
      }
    }

    else if (strcmp(input[0], "sum") == 0)
    {

      if (!input[1] || !input[2] || input[3])
      {
        printf("Error: sum must receive 2 arguments\n");
      }
      else
      {
        double x = atof(input[1]);
        double y = atof(input[2]);
        if ((strcmp(input[1], "0") && x == 0) || (strcmp(input[2], "0") && y == 0))
        {
          printf("Error: sum must receive floating point numbers\n");
        }
        else
        {
          pid_t pid_child = fork();
          if (pid_child == 0)
          {
            // funcion del proceso hijo
            // se deben tomar el input[1] e input [2] y pasarse a double
            
            printf("> %f\n", x+y);
            // recordar terminarlo para que no siga en el while
            int my_id = getpid();
            exit(my_id);
          }
          for (int i = 0; i < 255; i++)
          {
            if (process_array[i] == -1)
            {
              process_array[i] = pid_child;
              time_array[i] = time(NULL);
              names_array[i] = "sum";
              break;
            }
          }
          
        }
      }
    
    }

    else if (strcmp(input[0], "is_prime") == 0)
    {
      // implementar funcion is_prime
      // hacer algo con el caso en que input[1] sea 0 o negativo
      if (input[1])
      {
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
          for (int i = 0; i < 255; i++)
          {
            if (process_array[i] == -1)
            {
              process_array[i] = pid_child;
              time_array[i] = time(NULL);
              names_array[i] = "is_prime";
              break;
            }
          }
  
        }
      }
      else
      {
        printf("Error: is_prime must receive an argument\n");
      }
      
   
    }

    else if (strcmp(input[0], "crexec") == 0)
    {
      // implementar funcion crexec
      pid_t pid_child = fork();
      if (pid_child == 0)
      {
        execv(input[1], &input[1]);
        perror("> crexec");
        // si hay error hay que terminar el proceso
        exit(0);
      }
      for (int i = 0; i < 255; i++)
      {
        if (process_array[i] == -1)
        {
          process_array[i] = pid_child;
          time_array[i] = time(NULL);
          names_array[i] = input[1];
          break;
        }
      }
    }

    else if (strcmp(input[0], "crlist") == 0)
    {
      // implementar funcion crlist
      for (int i = 0; i < 255; i++)
      {
        if (process_array[i] != -1)
        {
          time_t time_passed = time(NULL) - time_array[i];
          pid_t stopped_running = waitpid(process_array[i], &status, WNOHANG);
          if (stopped_running == 0)
          {
            printf("name: %s | process ID: %i | running time: %ld s | status: %d", 
            names_array[i], process_array[i], time_passed, status);
            printf("%i\n", WIFEXITED(status));
          }      
      }
        // printf("%d\n", WIFEXITED(status_array[i]));
        // free(status);
        

      }
      
    }

    else if (strcmp(input[0], "crexit") == 0)
    {
      free_user_input(input);
      break;
    }

    else
    {
      // implementar que lance una excepcion por comando invalido
      printf("> comando '%s' desconocido\n", input[0]);
    }

  }
  
}
