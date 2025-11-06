#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_LEN 1024
static const char *const BUILT_IN_CMDS[] = {"echo", "type", "exit", NULL};

int tokenizer(char *input, char (*tokens)[MAX_LEN])
{
    int insideQuote = 0;
    int tokenCounter = 0;
    int charCounter = 0;

    for (; *input; input++)
    {
        char c = *input;
        if (insideQuote)
        {
            if (c == '\'' || c == '"')
            {
                insideQuote = 0;
                tokens[tokenCounter++][charCounter] = '\0';
                charCounter = 0;
            }
            else
                tokens[tokenCounter][charCounter++] = c;
        }
        else
        {
            if (c == '\'' || c == '"')
                insideQuote = 1;
            else if (c == ' ')
            {
                if (charCounter)
                {
                    tokens[tokenCounter++][charCounter] = '\0';
                    charCounter = 0;
                }
            }
            else
                tokens[tokenCounter][charCounter++] = c;
        }
    }

    return tokenCounter + 1;
}

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

void run_echo(int count, char (*tokens)[MAX_LEN])
{
    int start = (count > 1 && strcmp(tokens[1], "-n") == 0) ? 2 : 1;

    for (int i = start; i < count; i++)
    {
        printf("%s", tokens[i]);
        if (i < count - 1)
            putchar(' ');
    }

    if (start == 1)
        putchar('\n');
}

void run_type(int count, char (*tokens)[MAX_LEN])
{
    char full_path[1024];
    char *path = getenv("PATH");

    for (int i = 1; i < count; i++)
    {
        char *arg = tokens[i];

        if (is_builtin(arg, BUILT_IN_CMDS))
            printf("%s is a shell builtin\n", arg);
        else if (is_exec(arg, path, full_path, MAX_LEN))
            printf("%s is %s\n", arg, full_path);
        else
            printf("%s: not found\n", arg);
    }
}

int main(int argc, char *argv[])
{
    while (true)
    {
        // Flush after every printf
        setbuf(stdout, NULL);

    printf("$ ");

        // Wait for user input
        char in[MAX_LEN];
        fgets(in, sizeof(in), stdin);
        in[strcspn(in, "\n")] = '\0';
        char *input = in;

        char tokens[MAX_LEN][MAX_LEN] = {0};

        int count = tokenizer(input, tokens);

        if (count == 0)
            continue;

        char *cmd = tokens[0];

        if (strcmp(cmd, "type") == 0)
            run_type(count, tokens);
        else if (strcmp(cmd, "echo") == 0)
            run_echo(count, tokens);
        else if (strcmp(cmd, "exit") == 0)
            return count == 1 ? 0 : atoi(tokens[1]);
        else
            printf("%s: command not found\n", cmd);
    }

  return 0;
}
