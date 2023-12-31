#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A function to extract only the command name
void extract_command_name(const char* command, char* name, size_t len);
// A function to run the command 
void execute_command(const char* command, const char* output_dir, int* idx);


int main(int argc, char* argv[]) {
    // checks the input is correct
    if (argc < 3) {
        printf("Usage: %s <output_dir> <command_file1.dat> ... <command_fileN.dat>\n", argv[0]);
        return 1;
    }

    // Crates Output variable and unique id
    const char* output_dir = argv[1];
    int unique_idx = 1;

    // Loop to read .dat files
    for (int i = 2; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");
        if (!fp) {
            perror("Failed to open command file");
            continue;  // Move on to the next file
        }

        char command[256];
        // Gets the commands
        while (fgets(command, sizeof(command), fp)) {
            // Remove the newline character if present
            size_t len = strlen(command);
            if (len > 0 && command[len-1] == '\n') {
                command[len-1] = '\0';
            }
            // Runs the commands
            execute_command(command, output_dir, &unique_idx);
        }
        // Closes the file
        fclose(fp);
    }

    return 0;
}

// Runs the commmand
void execute_command(const char* command, const char* output_dir, int* idx) {
    char command_name[256];
    char filename[512];
    char system_cmd[1024];

    // Extract only the command name
    extract_command_name(command, command_name, sizeof(command_name));

    // Construct the filename
    snprintf(filename, sizeof(filename), "%s/%s_%d.txt", output_dir, command_name, (*idx)++);

    // Constructing the system command with redirection
    snprintf(system_cmd, sizeof(system_cmd), "%s > %s", command, filename);
    // runs the command
    system(system_cmd);
}

// Gets just the name of the command
void extract_command_name(const char* command, char* name, size_t len) {
    size_t i;
    for (i = 0; i < len && command[i] && command[i] != ' '; i++) {
        name[i] = command[i];
    }
    name[i] = '\0'; // Null-terminate the command name
}