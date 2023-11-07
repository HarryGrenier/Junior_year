#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

// Read in the map to a 2D vector
bool readMap(const std::string& filePath, std::vector<std::vector<float>>& map, int& rows, int& cols);

// Finds the neighboring points to a point
std::vector<std::pair<int, int>> getNeighbors(int row, int col, int rows, int cols);

// Calcuate the lowest point path
std::vector<std::pair<int, int>> steepestDescent(const std::vector<std::vector<float>>& map, int startRow, int startCol);

// Get the map file name
std::string extractMapName(const std::string& mapFilePath);

// Display the errors to the program output
void show_error(std::string outputFolderPath,std::string fileName, std::string Output_String);



int main(int argc, char *argv[]) {
       // Store the variables inputed into the program
    bool traceMode = false; // Stores true or false for if trace mode is needed
    std::string mapFilePath; // Like to the map path
    std::vector<std::pair<int,int>> List_of_Points;
    std::string outputFolderPath; // Stores the path to the output folder 

    int number_of_starting_points = argc - 3;

    // Stores the info when trace is true
    
    if (std::string(argv[1]) == "-t") {
        traceMode = true;
        mapFilePath = argv[2];
        for (int i = 3; i <= number_of_starting_points; i+=2){
            List_of_Points.push_back({atoi(argv[i])-1, atoi(argv[i+1])-1});
        }
        outputFolderPath = argv[argc-1];
    }
    // Stores the info when the trace is false
    else{
        mapFilePath = argv[1];
        for (int i = 2; i <= number_of_starting_points; i+=2){
            List_of_Points.push_back({atoi(argv[i])-1, atoi(argv[i+1])-1});
        }
        outputFolderPath = argv[argc-1];
    }

    if (!outputFolderPath.empty() && outputFolderPath.back() == '/') {
        // Remove the last character
        outputFolderPath.pop_back();
    }
    

    // Gets the name of the files
    std::string fileName = extractMapName(mapFilePath);

    // Stores the map vector
    std::vector<std::vector<float>> map;

    // Stores the rows and cols 
    int rows, cols;

    // reads the map and exits if the file isnt able to be opened
    if(!readMap(mapFilePath, map, rows, cols)){
        show_error((outputFolderPath + "/" + fileName + ".txt"),fileName, "File not found");
        exit(12);
    }

    std::vector<pid_t> childPIDs;
    std::vector<int> pipefds;

    for (int i = 0; i < number_of_starting_points / 2; i++) {
        int fds[2];
        if (pipe(fds) == -1) {
            std::cerr << "Failed to create a pipe: " << strerror(errno) << std::endl;
            exit(3);
        }

        pid_t pid = fork();
        if (pid == 0) { // Child process
            close(fds[0]); // Close the read end of the pipe in the child
            if (List_of_Points[i].first >= rows || List_of_Points[i].second >= cols) {
                show_error((outputFolderPath + "/Temp/" + fileName + std::to_string(i)+".txt"), fileName, ("Start point at row=" + std::to_string(List_of_Points[i].first + 1) + ", column=" + std::to_string(List_of_Points[i].second + 1) + " is invalid."));
                exit(1);
            }

            std::vector<std::pair<int, int>> path = steepestDescent(map, List_of_Points[i].first, List_of_Points[i].second);
            // Instead of show_results, write to the pipe
            std::ostringstream oss;
            if (traceMode) {
                for (const auto& point : path) {
                    oss << (point.first + 1) << " " << (point.second + 1) << " ";
                }
                oss << std::endl;
            }
            // Write the string stream contents to the pipe
            write(fds[1], oss.str().c_str(), oss.str().size());


            // Write the string stream contents to the pipe
            std::string output = oss.str();
            ssize_t bytes_written = write(fds[1], output.c_str(), output.size());
            if (bytes_written < 0) {
                std::cerr << "Failed to write to pipe: " << strerror(errno) << std::endl;
            }

            close(fds[1]); // Close the write end of the pipe
            exit(0); // Child process exits after completing its task
        } else if (pid < 0) {
            std::cerr << "Failed to fork: " << strerror(errno) << std::endl;
            exit(2);
        } else {
            close(fds[1]); // Close the write end of the pipe in the parent
            pipefds.push_back(fds[0]); // Save the read end to read from later
            childPIDs.push_back(pid);
        }
    }

    for (pid_t pid : childPIDs) {
        int status;
        waitpid(pid, &status, 0); // Wait for the specific child process to terminate
    }

    // Open the final output file once
    std::ofstream outFile(outputFolderPath + "/" + fileName + ".txt", std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open final output file: " << strerror(errno) << std::endl;
        exit(4);
    }

    for (size_t i = 0; i < childPIDs.size(); ++i) {
        // Read from the pipe
        char buffer[4096]; // Adjust size as necessary
        ssize_t bytes_read = read(pipefds[i], buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            std::cerr << "Failed to read from pipe: " << strerror(errno) << std::endl;
        } else if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            outFile << buffer; // Write to the final output file
        }
        close(pipefds[i]); // Close the read end of the pipe
    }

    outFile.close(); // Close the final output file

    return 0;
}

// Read in the map to a 2D vector
bool readMap(const std::string& filePath, std::vector<std::vector<float>>& map, int& rows, int& cols) {
    
    // Start our file stream
    std::ifstream inFile(filePath);

    // if file cant be opened return false
    if (!inFile.is_open()) {
        return 0;
    }

    // create the line variable 
    std::string line;

    // Read the dimensions of the map
    std::getline(inFile, line);
    std::istringstream iss(line);
    iss >> rows >> cols;

    // Resize the map to the dimensions read
    map.resize(rows, std::vector<float>(cols));

    // Read the map data
    for (int i = 0; i < rows; ++i) {
        std::getline(inFile, line);
        std::istringstream iss(line);
        for (int j = 0; j < cols; ++j) {
            iss >> map[i][j];
        }
    }

    // if properly compleated return true
    return 1;
}

// Finds the neighboring points to a point
std::vector<std::pair<int, int>> getNeighbors(int row, int col, int rows, int cols) {

    // create a vector to store the neighbors
    std::vector<std::pair<int, int>> neighbors;

    // search for neighbors
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // Skips the current point

            // bounds check
            int newRow = row + i;
            int newCol = col + j;
            // if requirments are met store neighbor
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                neighbors.push_back({newRow, newCol});
            }
        }
    }

    // Return the nighbors vector
    return neighbors;
}

// Calcuate the lowest point path
std::vector<std::pair<int, int>> steepestDescent(const std::vector<std::vector<float>>& map, int startRow, int startCol) {

    // Initiate the required variables for the function
    int rows = map.size(); // store the number of rows
    int cols = map[0].size(); // Store the number of Colums
    std::vector<std::pair<int, int>> path; // Store the path the program followed
    int currentRow = startRow; // Sets current row position to starting point
    int currentCol = startCol; // setts current column position to starting point
    path.push_back({currentRow, currentCol}); // Added starting point to start of the path followed

    while (true) {

        // Store the Neighbors of the current position to a vector
        std::vector<std::pair<int, int>> neighbors = getNeighbors(currentRow, currentCol, rows, cols);

        // Set the next point to the lowest value (currentrow and col will be differnt after first itteration)
        std::pair<int, int> nextPoint = {currentRow, currentCol};

        // Reset the lowest nearby elevation to the current position
        float lowestElevation = map[currentRow][currentCol];

        // Itterate through all the neighbors that were found
        for (const auto& neighbor : neighbors) {

            // Set Row and col to neighbor to investigate
            int row = neighbor.first;
            int col = neighbor.second;

            // If that neighbor is lower then the current lowest point store that at the next point and lowest point untill a new one arives
            if (map[row][col] < lowestElevation) {
                lowestElevation = map[row][col];
                nextPoint = neighbor;
            }
        }

        // If the next point is still == the point it was before then you have already found the lowest point
        if (nextPoint.first == currentRow && nextPoint.second == currentCol) {
            break;
        }

        // Set the current values to the lowest point found and store that to the path
        currentRow = nextPoint.first;
        currentCol = nextPoint.second;
        path.push_back({currentRow, currentCol});
    }

    // Return the path to the main function
    return path;
}

// Display the errors to the program output
void show_error(std::string outputFolderPath,std::string fileName, std::string Output_String) {
    // Concat Output String
    std::ofstream outFile(outputFolderPath);

    // If you cant open output path exit
    if (!outFile.is_open()) {
        exit(11);
    }

    // Output the error
    outFile << Output_String;
}

// Get the map file name
std::string extractMapName(const std::string& mapFilePath) {
    std::string fileName;
    int start = 0; // Start of the file name
    int end = mapFilePath.size(); // End of the file name

    // Find the start of the file name by backtracking
    for (int i = mapFilePath.size() - 1; i >= 0; i--) {
        if (mapFilePath[i] == '/' || mapFilePath[i] == '\\') {
            start = i + 1;
            break;
        }
    }

    // Find the end of the file name
    for (int i = start; i < mapFilePath.size(); i++) {
        if (mapFilePath[i] == '.') {
            end = i;
            break;
        }
    }

    // Extract the file name
    for (int i = start; i < end; i++) {
        fileName += mapFilePath[i];
    }
    return fileName;
}