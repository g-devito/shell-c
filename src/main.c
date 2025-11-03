#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
  const int MAX_INPUT_SIZE = 100;
  while (true)
  {
    // Flush after every printf
    setbuf(stdout, NULL);

    printf("$ ");

    // Wait for user input
    char input[MAX_INPUT_SIZE];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    // echo cmd
    if (strncmp(input, "echo", 4) == 0)
    {
	bool n_flag = false;
	char *arg_start = input+4;

	while (*arg_start == ' ')
		arg_start++;

	if (strncmp(arg_start, "-n", 2) == 0)
	{
		n_flag = true;
		arg_start += 2;
		while (*arg_start == ' ')
			arg_start++;

	}
	for (int i = 0; arg_start[i] != '\0'; i++)
	{
		if (arg_start[i] != '\"' && arg_start[i] != '\'')
			putchar(arg_start[i]); 
	}
	if (!n_flag)
		putchar('\n');
    }

    // exit cmd
    else if (strncmp(input, "exit", 4) == 0)
    {
	char *arg_start = input+4;
	char *end_pointer;

	while (*arg_start == ' ')
		arg_start++;
	int exit_number = (int) strtol(arg_start, &end_pointer, 10); 

	if (end_pointer == arg_start || errno == ERANGE, *end_pointer != '\0' || exit_number < 0)
		printf("exit: illegal number: %s\n", arg_start);
	else
		return exit_number;
    }
    else
	    printf("%s: command not found\n", input);
  }
  return 0;
}
