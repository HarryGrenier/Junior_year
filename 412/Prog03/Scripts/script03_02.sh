#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <patterns_directory> <image_files_directory> <output_directory>"
    exit 1
fi

# Assign arguments to variables
pattern_dir="$1"
image_dir="$2"
output_dir="$3"

# Build the executable for Version 2
gcc -o prog03v2 prog03v2.c

# Use find to get all the pattern files
pattern_files=$(find "$pattern_dir" -type f -name "*.pat")

# Use a loop to call the Version 2 program for each image file
for image_file in "$image_dir"/*.img; do
    ./prog03v2 "$image_file" "$output_dir" $pattern_files
done

echo "Processing complete!"

