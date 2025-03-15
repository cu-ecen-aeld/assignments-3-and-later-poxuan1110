#!/bin/bash

# Check if both arguments are provided
if [ $# -ne 2 ]; then
    echo "Error: Two arguments required - <file path> and <text string>"
    exit 1
fi

writefile="$1"
writestr="$2"

# Create directory structure if it doesn't exist
mkdir -p "$(dirname "$writefile")"

# Write to the file (overwrite if exists)
echo "$writestr" > "$writefile"

# Check if file was successfully created
if [ $? -ne 0 ]; then
    echo "Error: Failed to create/write to '$writefile'"
    exit 1
fi

echo "Successfully wrote to '$writefile'"
