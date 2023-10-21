#!/bin/bash

# Compile the executables
gcc version1.c -o version1
gcc version2.c -o version2
gcc version3.c -o version3

WATCH_FOLDER="$1"
VERSION="$2"
OUTPUT_FOLDER="$3"
PROCESSED_FILES="processed_files.tmp"

mkdir -p "$WATCH_FOLDER"
touch "$PROCESSED_FILES"

while true; do
    # Process newly added .dat files or folders
    find "$WATCH_FOLDER" -type f -name "*.dat" -o -type d | while read entry; do
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
                # Mark folder as processed to avoid re-checking in the future
                echo "$entry" >> "$PROCESSED_FILES"
            fi
        fi
    done
    sleep 5
done
