#!/bin/bash

# Check if the correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <path to data folder> <path to output folder>"
    exit 1
fi

g++ Programs/Version1/prog05v1.cpp --std=c++20 -o Compiled/prog05v1
g++ Programs/Version2/prog05v2.cpp --std=c++20 -o Compiled/prog05v2
g++ Programs/Version3/prog05v3.cpp --std=c++20 -o Compiled/prog05v3

chmod +x Compiled/*

DATA_FOLDER=$1
OUTPUT_FOLDER=$2
TEMP_FOLDER="$OUTPUT_FOLDER/Temp"

# Check if data folder exists
if [ ! -d "$DATA_FOLDER" ]; then
    echo "Data folder not found: $DATA_FOLDER"
    exit 1
fi

# Create or empty the output folder
if [ ! -d "$OUTPUT_FOLDER" ]; then
    mkdir -p "$OUTPUT_FOLDER"
else
    rm -rf "$OUTPUT_FOLDER"/*
fi

# Create the Temp directory inside the output folder
mkdir -p "$TEMP_FOLDER"

# Process each task file in the data folder
for task_file in "$DATA_FOLDER"/*.task; do
    TRACE_FLAG=""
    PROGRAM_MAP_LINE=""
    SKIER_COUNT=0
    SKIER_POINTS=()
    OUTPUT_FILE="$OUTPUT_FOLDER/"

    # Read the task file
    while IFS= read -r line || [[ -n "$line" ]]; do
        if [[ -z "$PROGRAM_MAP_LINE" ]]; then
            if [[ "$line" == "TRACE" ]]; then
                TRACE_FLAG="-t"
            elif [[ "$line" == "NO TRACE" ]]; then
                TRACE_FLAG=""
            else
                PROGRAM_MAP_LINE="$line"
            fi
        elif [[ "$SKIER_COUNT" -eq 0 ]]; then
            SKIER_COUNT="$line"
        else
            SKIER_POINTS+=("$line")
        fi
    done < "$task_file"

    # Extract program name and map file from the combined line
    PROGRAM_NAME=$(echo $PROGRAM_MAP_LINE | awk '{print $1}')
    MAP_FILE=$(echo $PROGRAM_MAP_LINE | awk '{print $2}')

    # Construct the command to run the skier dispatcher
    CMD="$PROGRAM_NAME $TRACE_FLAG $MAP_FILE"
    for point in "${SKIER_POINTS[@]}"; do
        CMD="$CMD $point"
    done
    CMD="$CMD $OUTPUT_FILE"

    # Run the skier dispatcher command in the background
    eval $CMD &
done