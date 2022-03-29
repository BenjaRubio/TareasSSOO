#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("Testing...\n ");
  printf("argc: %i\n", argc);
  sleep(20);
  for (int i = 0; i < argc; i++)
  {
    printf("argumento %i: %s\n", i, argv[i]);
  }
  
  return 0;
}