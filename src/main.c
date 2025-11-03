#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

    // echo cmd
    void* ptr = strstr(input, "echo");
    if (ptr)
	    printf("%p", ptr);

    // exit cmd
    if (strncmp(input, "exit ", 5) == 0 || strcmp(input, "exit") == 0)
    {
	char *end_pointer;
	int exit_number = (int) strtol(input+5, &end_pointer, 10); 

	if (end_pointer == (input+4) || errno == ERANGE, *end_pointer != '\0' || exit_number < 0)
		printf("exit: illegal number: %s\n", (input+5));
	else
		return exit_number;
    }
    else
	    printf("%s: command not found\n", input);
  }
  return 0;
}
