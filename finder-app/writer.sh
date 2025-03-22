#!/bin/bash

# Check if the correct number of arguments are provided
if [ $# -ne 2 ]; then
    echo "Error: Missing arguments."
    echo "Usage: $0 <file_path> <text_string>"
    exit 1
fi

# Assign arguments to variables
writefile="$1"
writestr="$2"

# Extract directory path from the file path
dirpath=$(dirname "$writefile")

# Create the directory path if it doesn't exist
mkdir -p "$dirpath"

# Attempt to create or overwrite the file with writestr
if echo "$writestr" > "$writefile"; then
    echo "File '$writefile' created successfully with content: $writestr"
else
    echo "Error: Could not create or write to file '$writefile'."
    exit 1
fi

exit 0
	

