#!/bin/bash

# Function to check if string ends with a /
function ensure_no_trailing_slash() {
    local str="$1"
    if [[ "$str" == */ ]]; then
        echo "${str:0:${#str}-1}"
    else
        echo "$str"
    fi
}

# Check for the correct number of arguments
if [[ $# -ne 2 ]]; then
    echo "usage: ./script02.sh <path> <extension>"
    exit 1
fi

# Store arguments in variables
path=$(ensure_no_trailing_slash "$1")
extension="$2"

# Print the initial message
echo "Looking for files with extension .$extension in folder"
echo "$path:"

# Initialize a counter for the number of files found
count=0

# Use a loop to iterate through the files in the folder
for file in "$path"/*; do
    if [[ -f "$file" && "$file" == *".$extension" ]]; then
        # If this is the first file found, print the message
        if [[ $count -eq 0 ]]; then
            files_list=()
        fi
        # Increment the counter and store the filename
        ((count++))
        files_list+=("$(basename "$file")")
    fi
done

# Print the result based on the count
if [[ $count -eq 0 ]]; then
    echo "No file found."
elif [[ $count -eq 1 ]]; then
    echo "1 file found:"
    echo "${files_list[0]}"
else
    echo "$count files found:"
    for file in "${files_list[@]}"; do
        echo "$file"
    done
fi
