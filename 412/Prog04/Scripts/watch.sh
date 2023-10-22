#!/bin/bash

# Exit on error
set -e

# Compile the executables
gcc ./Version1/prog04_v1.c -o version1
gcc ./Version2/prog04_v2.c -o version2
gcc ./Version3/prog04_v3.c -o version3

# Get the command line arguments
WATCH_FOLDER="$1"
VERSION="$2"
OUTPUT_FOLDER="$3"
PROCESSED_FILES="processed_files.tmp"

# Create the watch folder if it doesn't exist
mkdir -p "$WATCH_FOLDER"
touch "$PROCESSED_FILES"

while true; do
    # Process newly added .dat files or folders
    find "$WATCH_FOLDER" -type f -name "*.dat" -o -type d | while read -r entry; do
        # If not already processed
        if ! grep -q "^$entry$" "$PROCESSED_FILES"; then
            if [[ "$entry" == *.dat ]]; then
                ./version"$VERSION" "$OUTPUT_FOLDER" "$entry"
                echo "$entry" >> "$PROCESSED_FILES"
            else  # It's a folder
                # Find all .dat files in the folder
                dat_files=($(find "$entry" -type f -name "*.dat"))
                if (( ${#dat_files[@]} > 0 )); then
                    ./version"$VERSION" "$OUTPUT_FOLDER" "${dat_files[@]}"
                    # Mark all processed .dat files in the folder
                    for file in "${dat_files[@]}"; do
                        echo "$file" >> "$PROCESSED_FILES"
                    done
                fi
                # Mark folder as processed
                echo "$entry" >> "$PROCESSED_FILES"
            fi
        fi
    done
    # Wait for 5 seconds before checking again
    sleep 5
done

