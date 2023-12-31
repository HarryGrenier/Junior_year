#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A function to extract only the command name (before any spaces)
void extract_command_name(const char* command, char* name, size_t len);
// Function to run a command
void execute_command(const char* command, const char* output_dir, int* idx);


int main(int argc, char* argv[]) {

    // Checks for the proper input
    if (argc < 3) {
        printf("Usage: %s <output_dir> <command_file1.dat> ... <command_fileN.dat>\n", argv[0]);
        return 1;
    }

    // stores the output directory and uniue Id
    const char* output_dir = argv[1];
    int unique_idx = 1;

    // Loop that opens .dat files
    for (int i = 2; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");
        if (!fp) {
            perror("Failed to open command file");
            continue;  // Move on to the next file
        }
        // gets the commands from the files
        char command[256];
        while (fgets(command, sizeof(command), fp)) {
            // Remove the newline character if needed
            size_t len = strlen(command);

            if (len > 0 && command[len-1] == '\n') {
                command[len-1] = '\0';
            }
        
            // Check for bash comment lines and skip them
            char* trimmed_command = command;

            while (*trimmed_command && (*trimmed_command == ' ' || *trimmed_command == '\t')) {
                trimmed_command++;
            }

            if (*trimmed_command == '#') {
            continue;
            }

            // runs the command
            execute_command(command, output_dir, &unique_idx);
        }
        //closes the file
        fclose(fp);
    }

    return 0;
}

// function that runs the command
void execute_command(const char* command, const char* output_dir, int* idx) {
    char command_name[256];
    char filename[512];
    char system_cmd[1024];

    // Extract only the command name
    extract_command_name(command, command_name, sizeof(command_name));

    // Construct the filename
    snprintf(filename, sizeof(filename), "%s/%s_%d.txt", output_dir, command_name, (*idx)++);

    // Constructing the system command with file redirection
    snprintf(system_cmd, sizeof(system_cmd), "%s > %s", command, filename);
    // Runs the command
    system(system_cmd);
}

// gets just the name of the command
void extract_command_name(const char* command, char* name, size_t len) {
    size_t i;
    for (i = 0; i < len && command[i] && command[i] != ' '; i++) {
        name[i] = command[i];
    }
    name[i] = '\0'; // Null-terminate the command name
}
