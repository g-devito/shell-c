#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  const int MAX_INPUT_SIZE = 100;
  // Flush after every printf
  setbuf(stdout, NULL);

  printf("$ ");

  // Wait for user input
  char input[MAX_INPUT_SIZE];
  fgets(input, MAX_INPUT_SIZE, stdin);

  return 0;
}
