#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

void readMap(const std::string& filePath, std::vector<std::vector<float>>& map, int& rows, int& cols,std::string outputFolderPath,std::string fileName);
std::vector<std::pair<int, int>> getNeighbors(int row, int col, int rows, int cols);
std::vector<std::pair<int, int>> steepestDescent(const std::vector<std::vector<float>>& map, int startRow, int startCol);
void show_results(std::string outputFolderPath, bool traceMode,int startRow, int startCol,std::vector<std::pair<int, int>> path,std::string fileName);
std::string extractMapName(const std::string& mapFilePath);
void show_results(std::string outputFolderPath,std::string fileName, std::string Output_String);

int main(int argc, char *argv[]) {
    // Parse Command Line Arguments
    bool traceMode = false;
    std::string mapFilePath;
    int startRow, startCol;
    std::string outputFolderPath;

    if (std::string(argv[1]) == "-t") {
        traceMode = true;
        mapFilePath = argv[2];
        startRow = atoi(argv[3]);
        startCol = atoi(argv[4]);
        outputFolderPath = argv[5];
    }
    else{
        mapFilePath = argv[1];
        startRow = atoi(argv[2]);
        startCol = atoi(argv[3]);
        outputFolderPath = argv[4];
    }
    startRow -= 1;
    startCol -= 1;

    std::string fileName = extractMapName(mapFilePath);

    // Read the Map File
    std::vector<std::vector<float>> map;
    int rows, cols;
    readMap(mapFilePath, map, rows, cols,outputFolderPath,fileName);

    if (startCol > cols || startRow > rows){
        show_results(outputFolderPath,fileName,("Start point at row=" + std::to_string(startRow+1) + ", column=" + std::to_string(startCol+1) + " is invalid." ));
        return 0;
    }

    // Implement the Steepest Descent Algorithm
    std::vector<std::pair<int, int>> path = steepestDescent(map, startRow, startCol);

    // Output Results
    show_results(outputFolderPath,traceMode,startRow,startCol,path,fileName);

    return 0;
}



void readMap(const std::string& filePath, std::vector<std::vector<float>>& map, int& rows, int& cols,std::string outputFolderPath,std::string fileName) {
    std::ifstream inFile(filePath);
     if (!inFile.is_open()) {
        show_results(outputFolderPath,fileName, "File not found");
        exit(12);
    }
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
}

std::vector<std::pair<int, int>> getNeighbors(int row, int col, int rows, int cols) {
    std::vector<std::pair<int, int>> neighbors;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // Skip the current point
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                neighbors.push_back({newRow, newCol});
            }
        }
    }
    return neighbors;
}

std::vector<std::pair<int, int>> steepestDescent(const std::vector<std::vector<float>>& map, int startRow, int startCol) {
    int rows = map.size();
    int cols = map[0].size();
    std::vector<std::pair<int, int>> path;
    int currentRow = startRow;
    int currentCol = startCol;
    path.push_back({currentRow, currentCol});

    while (true) {
        std::vector<std::pair<int, int>> neighbors = getNeighbors(currentRow, currentCol, rows, cols);
        std::pair<int, int> nextPoint = {currentRow, currentCol};
        float lowestElevation = map[currentRow][currentCol];

        for (const auto& neighbor : neighbors) {
            int row = neighbor.first;
            int col = neighbor.second;
            if (map[row][col] < lowestElevation) {
                lowestElevation = map[row][col];
                nextPoint = neighbor;
            }
        }

        if (nextPoint.first == currentRow && nextPoint.second == currentCol) {
            // Local minimum reached
            break;
        }

        currentRow = nextPoint.first;
        currentCol = nextPoint.second;
        path.push_back({currentRow, currentCol});
    }

    return path;
}

void show_results(std::string outputFolderPath, bool traceMode, int startRow, int startCol, std::vector<std::pair<int, int>> path,std::string fileName) {
    std::ofstream outFile(outputFolderPath +"/" + fileName +".txt");
    if (!outFile.is_open()) {
        exit(11);
    }

    if (traceMode) {
        outFile << "TRACE" << std::endl;
    } else {
        outFile << "NO_TRACE" << std::endl;
    }

    // Adjust the output to be 1-based
    outFile << (startRow + 1) << " " << (startCol + 1) << " " << (path.back().first + 1) << " " << (path.back().second + 1) << " " << path.size() << std::endl;

    if (traceMode) {
        for (const auto& point : path) {
            outFile << (point.first + 1) << " " << (point.second + 1) << " ";
        }
        outFile << std::endl;
    }
}

void show_results(std::string outputFolderPath,std::string fileName, std::string Output_String) {
    std::ofstream outFile(outputFolderPath +"/" + fileName +".txt");
    if (!outFile.is_open()) {
        exit(11);
    }
    // Adjust the output to be 1-based
    outFile << Output_String;
}

std::string extractMapName(const std::string& mapFilePath) {
    std::string fileName;
    int start = 0; // Start of the file name
    int end = mapFilePath.size(); // End of the file name (non-inclusive)

    // Find the start of the file name (after the last slash)
    for (int i = mapFilePath.size() - 1; i >= 0; i--) {
        if (mapFilePath[i] == '/' || mapFilePath[i] == '\\') {
            start = i + 1;
            break;
        }
    }

    // Find the end of the file name (before the last dot)
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
