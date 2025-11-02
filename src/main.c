#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  const int MAX_INPUT_SIZE = 100;
  while (1)
  {
    // Flush after every printf
    setbuf(stdout, NULL);

    printf("$ ");

    // Wait for user input
    char input[MAX_INPUT_SIZE];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    // check user input
    if (strcmp(input, "exit 0"))
	    return 0;
    if (strcmp(input, "exit 1"))
	    return 1;
    printf("%s: command not found\n", input);
  }
  return 0;
}
