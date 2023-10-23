#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 100

/// A function to extract only the command name (before any spaces)
void extract_command_name(const char* command, char* name, size_t len);
void execute_command(const char* command, const char* output_dir, int* idx);
void Breakup_command(const char* command, char** args, int* arg_count);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <output_dir> <command_file1.dat> ... <command_fileN.dat>\n", argv[0]);
        return 1;
    }

    const char* output_dir = argv[1];
    int unique_idx = 1;

    for (int i = 2; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");
        if (!fp) {
            perror("Failed to open command file");
            continue;
        }

        char command[256];
        while (fgets(command, sizeof(command), fp)) {
            size_t len = strlen(command);
            if (len > 0 && command[len-1] == '\n') {
                command[len-1] = '\0';
            }

            execute_command(command, output_dir, &unique_idx);
        }
        fclose(fp);
    }

    // Wait for all child processes to complete
    while (wait(NULL) > 0);

    return 0;
}

void execute_command(const char* command, const char* output_dir, int* idx) {
    char command_name[256];
    char filename[512];
    char* args[MAX_ARGS];
    int arg_count = 0;

    // Extract only the command name
    extract_command_name(command, command_name, sizeof(command_name));

    // Construct the filename
    snprintf(filename, sizeof(filename), "%s/%s_%d.txt", output_dir, command_name, (*idx)++);

    // Tokenize the command to prepare for execvp()
    Breakup_command(command, args, &arg_count);

    // Redirect stdout to the file
    freopen(filename, "w", stdout);

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) { // child process
        execvp(args[0], args);
        perror("Exec failed");  // This line runs only if execvp fails
        _exit(1);  // If exec fails, exit child
    }
}

void Breakup_command(const char* command, char** args, int* arg_count) {
    char* Part;
    char* cmd_copy = strdup(command);  // Duplicate the command for tokenization

    Part = strtok(cmd_copy, " ");
    while (Part != NULL) {
        args[(*arg_count)++] = Part;
        Part = strtok(NULL, " ");
    }

    args[*arg_count] = NULL;  // NULL-terminate the args list
}

void extract_command_name(const char* command, char* name, size_t len) {
    size_t i;
    for (i = 0; i < len && command[i] && command[i] != ' '; i++) {
        name[i] = command[i];
    }
    name[i] = '\0'; // Null-terminate the command name
}
