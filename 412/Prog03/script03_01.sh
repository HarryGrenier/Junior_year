#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <image_files_directory> <scrap_folder> <destination_directory>"
    exit 1
fi

# Assign arguments to variables
image_dir="$1"
scrap_dir="$2"
dest_dir="$3"

# Create scrap folder if it doesn't exist
mkdir -p "$scrap_dir"

# Process each file in the image directory
for file in "$image_dir"/*.img; do
    # Get the base filename
    base_name=$(basename "$file")
    
    # Use a loop to process each line
    while IFS= read -r line; do
        # Remove Windows line ending (if exists) and add Unix line ending
        printf '%s\n' "${line%$'\r'}"
    done < "$file" > "$scrap_dir/$base_name"
    
    # Move the processed file to the destination folder
    mv "$scrap_dir/$base_name" "$dest_dir/$base_name"
done

# Remove the scrap folder
rmdir "$scrap_dir"

echo "Processing complete!"

