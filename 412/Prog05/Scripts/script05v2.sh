#!/bin/bash

# Check if the correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <path to watch folder> <path to output folder>"
    exit 1
fi

# Compile programs
g++ Programs/Version1/prog05v1.cpp --std=c++20 -o Compiled/prog05v1
g++ Programs/Version2/prog05v2.cpp --std=c++20 -o Compiled/prog05v2
g++ Programs/Version3/prog05v3.cpp --std=c++20 -o Compiled/prog05v3
chmod +x Compiled/*

# Monitor the specified folder for task files
WATCH_FOLDER=$1
OUTPUT_FOLDER=$2
TEMP_FOLDER="$OUTPUT_FOLDER/Temp"
# Create the watch folder if it doesn't exist
if [ ! -d "$WATCH__FOLDER" ]; then
    mkdir -p "$WATCH_FOLDER"
fi

# Create or empty the output folder
if [ ! -d "$OUTPUT_FOLDER" ]; then
    mkdir -p "$OUTPUT_FOLDER"
else
    rm -rf "$OUTPUT_FOLDER"/*
fi

# Create the Temp directory inside the output folder
mkdir -p "$TEMP_FOLDER"


process_task_file() {
    NEWFILE=$1
    echo "Processing task file: $NEWFILE"

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
    done < "$NEWFILE"

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
}



# Start the polling loop
while true; do
    # Check for new .task files
    for FILE in "$WATCH_FOLDER"/*.task; do
        # Skip if no files found
        [ -e "$FILE" ] || continue

        # Process the file if it's new
        if [ ! -f "$TEMP_FOLDER/$(basename "$FILE")" ]; then
            if grep -q "^END$" "$FILE"; then
                echo "Terminating all skier processes."
                # Implement process termination logic here
                exit 0
            else
                process_task_file "$FILE"
                # Mark the file as processed
                touch "$TEMP_FOLDER/$(basename "$FILE")"
            fi
        fi
    done

    # Sleep for a while before checking again
    sleep 5
done