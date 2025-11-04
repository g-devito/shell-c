#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

bool is_builtin(char *input, const char *const BUILT_IN_CMDS[])
{

    for (int i = 0; BUILT_IN_CMDS[i] != NULL; i++)
    {
        if (strcmp(BUILT_IN_CMDS[i], input) == 0)
            return true;
    }
    return false;
}

bool is_exec(char *input, char *path, char *full_path, size_t sizeof_full_path)
{
    char *path_copy = strdup(path);
    char *save_ptr;
    char *ptr = path_copy;
    char *tok = strtok_r(path_copy, ":", &save_ptr);

    while (tok != NULL)
    {
        snprintf(full_path, sizeof_full_path, "%s/%s", tok, input);
        if (access(full_path, X_OK) == 0)
        {
            free(ptr);
            return true;
        }
        tok = strtok_r(NULL, ":", &save_ptr);
    }

    free(ptr);
    return false;
}

int main(int argc, char *argv[])
{
    char *path = getenv("PATH");
    const int MAX_INPUT_SIZE = 100;
    const char *const BUILT_IN_CMDS[] =
        {
            "echo",
            "type",
            "exit",
            NULL};

    while (true)
    {
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
        if (strncmp(input, "type", 4) == 0)
        {
            char full_path[1024];
            char *input_token = strtok(input + 4, " \t");

            while (input_token != NULL)
            {
                if (is_builtin(input_token, BUILT_IN_CMDS))
                    printf("%s is a shell builtin\n", input_token);
                else if (is_exec(input_token, path, full_path, sizeof(full_path)))
                    printf("%s is %s\n", input_token, full_path);
                else
                    printf("%s: not found\n", input_token);

                input_token = strtok(NULL, " \t");
            }
        }

        // echo cmd
        else if (strncmp(input, "echo", 4) == 0)
        {
            bool n_flag = false;
            char *arg_start = input + 4;

            while (*arg_start == ' ' || *arg_start == '\t')
                arg_start++;

            if (strncmp(arg_start, "-n", 2) == 0)
            {
                n_flag = true;
                arg_start += 2;
                while (*arg_start == ' ' || *arg_start == '\t')
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
            char *end_pointer;
            char *arg = input + 4;

            while (*arg == ' ' || *arg == '\t')
                arg++;
            int exit_number = (int)strtol(arg, &end_pointer, 10);

            if (end_pointer == arg || errno == ERANGE || *end_pointer != '\0' || exit_number < 0)
                printf("exit: illegal number: %s\n", arg);
            else
                return exit_number;
        }
        else
            printf("%s: command not found\n", input);
    }

    return 0;
}
