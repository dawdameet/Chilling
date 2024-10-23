#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_NUM_ARGS 100

void read_input(char *buffer) {
    printf("stash> ");
    fgets(buffer, MAX_INPUT_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove trailing newline
}
int parse_input(char *input, char **args) {
    int count = 0;
    char *token = strtok(input, " ");
    while (token != NULL && count < MAX_NUM_ARGS) {
        args[count++] = token;
        token = strtok(NULL, " ");
    }
    args[count] = NULL;  // Null-terminate the argument list
    return count;
}
void execute_command(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL) {
            chdir(args[1]);
        } else {
            fprintf(stderr, "cd: missing argument\n");
        }
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("stash");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("stash");
    } else {
        // Parent process
        wait(NULL);  // Wait for the child process to finish
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_NUM_ARGS];

    while (1) {
        read_input(input);
        int arg_count = parse_input(input, args);
        if (arg_count > 0) {
            execute_command(args);
        }
    }

    return 0;
}

