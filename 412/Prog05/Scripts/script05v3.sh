#!/bin/bash

# Check if the correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <path to watch folder> <path to output folder>"
    exit 1
fi

# Paths to folders
WATCH_FOLDER=$1
OUTPUT_FOLDER=$2
GENERAL_DISPATCHER="general_dispatcher" # Name your general dispatcher executable

# Compile the general dispatcher and other programs
g++ Programs/Version4/general_dispatcher.cpp --std=c++20 -o Compiled/$GENERAL_DISPATCHER
# Assuming you have other versions to compile, do it here
# g++ Programs/Version1/prog05v1.cpp --std=c++20 -o Compiled/prog05v1
# g++ Programs/Version2/prog05v2.cpp --std=c++20 -o Compiled/prog05v2
# g++ Programs/Version3/prog05v3.cpp --std=c++20 -o Compiled/prog05v3
chmod +x Compiled/*

# Create the output and temporary folders
mkdir -p "$OUTPUT_FOLDER"
mkdir -p "$WATCH_FOLDER"

# Launch the general dispatcher in the background and get its PID
./Compiled/$GENERAL_DISPATCHER &
GENERAL_DISPATCHER_PID=$!

# Function to clean up background processes on exit
cleanup() {
    echo "Cleaning up..."
    kill $GENERAL_DISPATCHER_PID
    exit 0
}

# Trap script termination to clean up background processes
trap cleanup SIGINT SIGTERM

# Main loop to watch for new files
while true; do
    # Check for new .map and .task files
    for FILE in "$WATCH_FOLDER"/*.{map,task}; do
        # Skip if no files found
        [ -e "$FILE" ] || continue

        # Process the file
        echo "Detected new file: $FILE"
        # Send the file path to the general dispatcher
        # This assumes that the general dispatcher can read from stdin
        echo "$FILE" >&$GENERAL_DISPATCHER_PID

        # Move the file to the output folder to avoid reprocessing
        mv "$FILE" "$OUTPUT_FOLDER/"
    done

    # Sleep for a while before checking again
    sleep 1
done

# Call cleanup function when the script exits
cleanup
