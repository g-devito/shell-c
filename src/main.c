#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  const char *path = getenv("PATH");
  const int MAX_INPUT_SIZE = 100;
  const char *const BUILT_IN_CMDS[] = {"echo", "type", "exit", NULL};

  while (true) {
    // Flush after every printf
    setbuf(stdout, NULL);

    printf("$ ");

    // Wait for user input
    char in[MAX_INPUT_SIZE];
    fgets(in, sizeof(in), stdin);
    in[strcspn(in, "\n")] = '\0';
    char *input = in;

    while (*input == ' ' || *input == '\t')
      input++;

    // type cmd
    if (strncmp(input, "type", 4) == 0) {
      char *arg_start = input + 4;

      char *input_token = strtok(input + 4, " \t");
      while (input_token != NULL) {
        bool is_builtin = false;
        for (int i = 0; BUILT_IN_CMDS[i] != NULL && !is_builtin; i++) {
          if (strcmp(BUILT_IN_CMDS[i], input_token) == 0)
            is_builtin = true;
        }
        if (is_builtin)
          printf("%s is a shell builtin\n", input_token);
        else
          printf("%s: not found\n", input_token);
        input_token = strtok(NULL, " \t");
      }
    }

    // echo cmd
    else if (strncmp(input, "echo", 4) == 0) {
      bool n_flag = false;
      char *arg_start = input + 4;

      while (*arg_start == ' ' || *arg_start == '\t')
        arg_start++;

      if (strncmp(arg_start, "-n", 2) == 0) {
        n_flag = true;
        arg_start += 2;
        while (*arg_start == ' ' || *arg_start == '\t')
          arg_start++;
      }
      for (int i = 0; arg_start[i] != '\0'; i++) {
        if (arg_start[i] != '\"' && arg_start[i] != '\'')
          putchar(arg_start[i]);
      }
      if (!n_flag)
        putchar('\n');
    }

    // exit cmd
    else if (strncmp(input, "exit", 4) == 0) {
      char *arg_start = input + 4;
      char *end_pointer;

      while (*arg_start == ' ' || *arg_start == '\t')
        arg_start++;
      int exit_number = (int)strtol(arg_start, &end_pointer, 10);

      if (end_pointer == arg_start || errno == ERANGE,
          *end_pointer != '\0' || exit_number < 0)
        printf("exit: illegal number: %s\n", arg_start);
      else
        return exit_number;
    } else
      printf("%s: command not found\n", input);
  }

  return 0;
}
