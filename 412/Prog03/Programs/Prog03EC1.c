#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    //error checking
    if (argc < 4) {
        printf("Usage: %s <image_file> <output_directory> <pattern1> <pattern2> ...", argv[0]);
        return 1;
    }

    //open File
    FILE *Image_File = fopen(argv[1], "r");
    if (!Image_File) {
        perror("Failed to open image file");
        return 1;
    }

    //Getting image dimensions
    int imageWidth, imageHeight;
    fscanf(Image_File, "%d %d", &imageWidth, &imageHeight);

    //makes image array
    char image[imageHeight][imageWidth];

    //Gets data out of image file
    for (int i = 0; i < imageHeight; i++) {
        fscanf(Image_File, "%s", image[i]);
    }

    // closes file
    fclose(Image_File);

    // Handels slash for output directory
    char *outputDir = argv[2];
    if (outputDir[strlen(outputDir) - 1] == '/') {
        outputDir[strlen(outputDir) - 1] = '\0';
    }

    // gets root name of the image file for output file naming
    char *imageName = strrchr(argv[1], '/') ? strrchr(argv[1], '/') + 1 : argv[1];
    char *dotPtr = strrchr(imageName, '.');
    if (dotPtr) *dotPtr = '\0'; 

    //gets output file name
    char outputFileName[1000];
    sprintf(outputFileName, "%s/%s_Matches.txt", outputDir, imageName);

    // Opens the output file for writing
    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Failed to open output file");
        return 1;
    }

    //start loop for checking patterns
    for (int patIndex = 3; patIndex < argc; patIndex++) {
        int PatternWidth, PatternHeight;

        //opens pattern file
        FILE *Pattern_File = fopen(argv[patIndex], "r");
        if (!Pattern_File) {
            perror("Failed to open pattern file");
            continue;
        }

        //reads pattern dimensions
        fscanf(Pattern_File, "%d %d", &PatternWidth, &PatternHeight);

        //get pattern array dynamically based on its dimensions
        char **pattern = malloc(PatternHeight * sizeof(char *));
        for (int i = 0; i < PatternHeight; i++) {
            pattern[i] = malloc((PatternWidth + 1) * sizeof(char)); // +1 for null terminator
            fscanf(Pattern_File, "%s", pattern[i]);
        }

        //close the file to prevent memory leaks
        fclose(Pattern_File);

        //output pattern file name
        fprintf(outputFile, "%s\n\t", strrchr(argv[patIndex], '/') ? strrchr(argv[patIndex], '/') + 1 : argv[patIndex]);

        // tracks matches
        int match_count = 0;
        int matches_found[1000][2];

        //iterate the pattern and compare it to the image array
        for (int i = 0; i <= imageHeight - PatternHeight; i++) {
            for (int j = 0; j <= imageWidth - PatternWidth; j++) {
                // keeps track of when there is a valid match
                int Valid_match = 1;

                for (int Pattern_i = 0; Pattern_i < PatternHeight; Pattern_i++) {
                    for (int Pattern_j = 0; Pattern_j < PatternWidth; Pattern_j++) {
                        if (image[i + Pattern_i][j + Pattern_j] != pattern[Pattern_i][Pattern_j]) {
                            Valid_match = 0;
                            break;
                        }
                    }
                    if (!Valid_match) {
                        break;
                    }
                }
                if (Valid_match) {
                    matches_found[match_count][0] = i;
                    matches_found[match_count][1] = j;
                    match_count++;
                }
            }
        }

        //outputs match results
        if (match_count == 0) {
            fprintf(outputFile, "0");
        } else {
            fprintf(outputFile, "%d ", match_count);
            for (int Current = 0; Current < match_count; Current++) {
                fprintf(outputFile, "%d %d ", matches_found[Current][0], matches_found[Current][1]);
            }
        }
        fprintf(outputFile, "\n");

        // free allocated memory for pattern
        for (int i = 0; i < PatternHeight; i++) {
            free(pattern[i]);
        }
        free(pattern);
    }

    fclose(outputFile);

    return 0;
}

