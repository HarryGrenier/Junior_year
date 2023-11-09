#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <map>

// Function to check if the file is a map file
bool isMapFile(const std::string& filePath) {
    // Implement logic to check if the file is a map file
    // For example, check the file extension or file content
    return filePath.find(".map") != std::string::npos;
}

// Function to launch a skier dispatcher process
void launchSkierDispatcher(const std::string& mapFilePath) {
    // Fork a new process
    pid_t pid = fork();
    if (pid == 0) { // Child process
        // Replace the child process with the skier dispatcher program
        execlp("./skier_dispatcher", "skier_dispatcher", mapFilePath.c_str(), (char*)NULL);
        // If execlp returns, it must have failed
        std::cerr << "Failed to launch skier dispatcher for " << mapFilePath << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Handle error in forking
        std::cerr << "Failed to fork for skier dispatcher." << std::endl;
    }
    // Parent process continues
}

int main() {
    std::string filePath;
    std::map<std::string, pid_t> mapToDispatcher; // Map file paths to dispatcher PIDs

    // Read file paths from standard input
    while (std::getline(std::cin, filePath)) {
        if (isMapFile(filePath)) {
            // If it's a map file, launch a new skier dispatcher process
            launchSkierDispatcher(filePath);
        } else {
            // If it's a task file, find the corresponding skier dispatcher and send it the task
            // This part of the code is left as an exercise, as it requires a communication mechanism
            // between the general dispatcher and skier dispatchers, which could be pipes, sockets, etc.
        }
    }

    // Wait for all child processes to finish
    int status;
    while (wait(&status) > 0);

    return 0;
}
 
