#!/bin/bash

# Check if the correct number of arguments are provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <watch_folder> <version> <output_folder>"
    exit 1
fi

# Compile the executables
gcc version1.c -o version1
gcc version2.c -o version2
gcc version3.c -o version3

# Arguments
WATCH_FOLDER="$1"
VERSION="$2"
OUTPUT_FOLDER="$3"
PROCESSED_FILES=".processed_files.tmp"

# Create the watch folder if it doesn't exist
mkdir -p "$WATCH_FOLDER"

# Create or clear the processed files list
> "$PROCESSED_FILES"

# Monitor the watch folder indefinitely
inotifywait -m -e create -e moved_to --format "%f" "$WATCH_FOLDER" | while read ENTRY; do
    ENTRY_PATH="$WATCH_FOLDER/$ENTRY"
    
    if [[ "$ENTRY" == *.dat ]]; then
        # Execute the command on the new .dat file
        ./version"$VERSION" "$OUTPUT_FOLDER" "$ENTRY_PATH"
        echo "$ENTRY_PATH" >> "$PROCESSED_FILES"
    elif [ -d "$ENTRY_PATH" ]; then
        # Find all .dat files in the new directory
        dat_files=($(find "$ENTRY_PATH" -type f -name "*.dat"))
        if (( ${#dat_files[@]} > 0 )); then
            ./version"$VERSION" "$OUTPUT_FOLDER" "${dat_files[@]}"
            for file in "${dat_files[@]}"; do
                echo "$file" >> "$PROCESSED_FILES"
            done
        fi
    fi
done
