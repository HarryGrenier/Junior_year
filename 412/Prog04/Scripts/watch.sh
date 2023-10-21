#!/bin/bash

WATCH_FOLDER="$1"
VERSION="$2"
OUTPUT_FOLDER="$3"
PROCESSED_FILES="processed_files.tmp"

[ ! -d "$WATCH_FOLDER" ] && mkdir -p "$WATCH_FOLDER"
touch "$PROCESSED_FILES"

while true; do
    for entry in "$WATCH_FOLDER"/*; do
        # Check if file was already processed
        if ! grep -q "$entry" "$PROCESSED_FILES"; then
            if [[ "$entry" == *.dat ]]; then
                ./version"$VERSION" "$OUTPUT_FOLDER" "$entry"
                echo "$entry" >> "$PROCESSED_FILES"
            elif [ -d "$entry" ]; then
                files=($(find "$entry" -type f -name "*.dat"))
                if (( ${#files[@]} > 0 )); then
                    ./version"$VERSION" "$OUTPUT_FOLDER" "${files[@]}"
                    echo "$entry" >> "$PROCESSED_FILES"
                fi
            else
                # Ignore other types of files
                echo "$entry" >> "$PROCESSED_FILES"
            fi
        fi
    done
    sleep 2
done
