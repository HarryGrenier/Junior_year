#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/// A function to extract only the command name (before any spaces)
void extract_command_name(const char* command, char* name, size_t len);
void execute_command(const char* command, const char* output_dir, int* idx);
void Breakup_command(const char* command, char** args, int* arg_count);

int main(int argc, char* argv[]) {
    // Checks for the proper # of arguments
    if (argc < 3) {
        printf("Usage: %s <output_dir> <command_file1.dat> ... <command_fileN.dat>\n", argv[0]);
        return 1;
    }

    // Declares Vars for output Directory and index id
    const char* output_dir = argv[1];
    int unique_idx = 1;
    
    // Loop to open dat files
    for (int i = 2; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");
        //checks file can open
        if (!fp) {
            perror("Failed to open command file");
            continue;
        }

        //stores the command
        char command[256];

        // Loop to get commands from the file
        while (fgets(command, sizeof(command), fp)) {
            size_t len = strlen(command);
            if (len > 0 && command[len-1] == '\n') {
                command[len-1] = '\0';
            }

            // Function to run the commands 
            execute_command(command, output_dir, &unique_idx);
        }
        fclose(fp);
    }

    // Wait for all child processes to complete
    while (wait(NULL) > 0);

    return 0;
}
// Function that runs the commands 
void execute_command(const char* command, const char* output_dir, int* idx) {

    //Required variables for storage
    char command_name[256];
    char filename[512];
    char* args[100];
    int arg_count = 0;

    // Extract only the command name
    extract_command_name(command, command_name, sizeof(command_name));

    // Construct the filename
    snprintf(filename, sizeof(filename), "%s/%s_%d.txt", output_dir, command_name, (*idx)++);

    // Breakup the command to prepare for execvp()
    Breakup_command(command, args, &arg_count);

    // Redirect stdout to the file
    freopen(filename, "w", stdout);

    //forks the process 
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    //if its a child process run the command
    if (pid == 0) { 
        execvp(args[0], args);
        perror("Exec failed");  // This line runs only if execvp fails
        _exit(1);  // If exec fails, exit child
    }
}

// Breaks up the command in orer to pass it throug as execvp
void Breakup_command(const char* command, char** args, int* arg_count) {
    char* Part;
    char* cmd_copy = strdup(command);  // Duplicate the command

    Part = strtok(cmd_copy, " ");
    while (Part != NULL) {
        args[(*arg_count)++] = Part;
        Part = strtok(NULL, " ");
    }

    args[*arg_count] = NULL;  // NULL-terminate the args list
}


// Function to get just the name of the command
void extract_command_name(const char* command, char* name, size_t len) {
    size_t i;
    for (i = 0; i < len && command[i] && command[i] != ' '; i++) {
        name[i] = command[i];
    }
    name[i] = '\0'; // Null-terminate the command name
}
