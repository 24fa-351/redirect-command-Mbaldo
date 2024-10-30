#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *input_file, char *command, char *output_file) {
    int inp_fd = 0, out_fd = 1;

    // Check if the input file is not "-"
    if (strcmp(input_file, "-") != 0) {
        inp_fd = open(input_file, O_RDONLY);
        if (inp_fd < 0) {
            perror("Failed to open input file");
            exit(EXIT_FAILURE);
        }
    }

    // Check if the output file is not "-"
    if (strcmp(output_file, "-") != 0) {
        out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd < 0) {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        // Redirect input if needed
        if (inp_fd != 0) {
            dup2(inp_fd, STDIN_FILENO);
            close(inp_fd);
        }

        // Redirect output if needed
        if (out_fd != 1) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        // Split the command into program and arguments
        char *args[100];
        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // Null terminate the argument list

        // Execute the command (search in PATH if necessary)
        if (execvp(args[0], args) == -1) {
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
    } else {  // Parent process
        // Wait for the child process to complete
        wait(NULL);
    }

    // Close file descriptors if they were opened
    if (inp_fd != 0) close(inp_fd);
    if (out_fd != 1) close(out_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        return EXIT_FAILURE;
    }

    execute_command(argv[1], argv[2], argv[3]);

    return EXIT_SUCCESS;
}
