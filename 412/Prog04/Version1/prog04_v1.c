#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A function to execute the bash command and redirect its output to a file
void execute_command(const char* command, const char* output_dir, int* idx) {
    char filename[512];
    char system_cmd[1024];

    snprintf(filename, sizeof(filename), "%s/%s%d.txt", output_dir, command, (*idx)++);
    
    // Remove potential unsafe characters from filename (simple sanitization)
    for (int i = 0; filename[i]; i++) {
        if (filename[i] == ' ' || filename[i] == '|' || filename[i] == '&')
            filename[i] = '_';
    }

    // Constructing the system command with redirection
    snprintf(system_cmd, sizeof(system_cmd), "%s > %s", command, filename);

    system(system_cmd);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <output_dir> <command_file1.dat> ... <command_fileN.dat>\n", argv[0]);
        return 1;
    }

    const char* output_dir = argv[1];
    int unique_idx = 1;  // Start our unique index from 1

    for (int i = 2; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");
        if (!fp) {
            perror("Failed to open command file");
            continue;  // Move on to the next file
        }

        char command[256];
        while (fgets(command, sizeof(command), fp)) {
            // Remove the newline character if present
            size_t len = strlen(command);
            if (len > 0 && command[len-1] == '\n') {
                command[len-1] = '\0';
            }

            execute_command(command, output_dir, &unique_idx);
        }
        fclose(fp);
    }

    return 0;
}
