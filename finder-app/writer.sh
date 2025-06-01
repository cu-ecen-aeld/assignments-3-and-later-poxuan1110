#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Error: Two arguments required - <file path> and <text string>"
    exit 1
fi

writefile="$1"
writestr="$2"


mkdir -p "$(dirname "$writefile")"


echo "$writestr" > "$writefile"


if [ $? -ne 0 ]; then
    echo "Error: Failed to create/write to '$writefile'"
    exit 1
fi

echo "Successfully wrote to '$writefile'"
