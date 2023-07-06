#!/bin/bash

cd ../../results/literature

# Define the folder name
folder="regular"

# Define the output file name
output_file="unsolved-regular.txt"

# Clear the output file before starting
> "$output_file"

# Loop through each file in the folder
for file in "$folder"/*; do
    # Check if the file contains the word "Feasible"
    if grep -q "Feasible" "$file"; then
        # If found, append the file name to the output file
        echo $(basename "$file") >> "$output_file"
    fi
done

folder="sos1"
output_file="unsolved-sos1.txt"

> "$output_file"

# Loop through each file in the folder
for file in "$folder"/*; do
    # Check if the file contains the word "Feasible"
    if grep -q "Feasible" "$file"; then
        # If found, append the file name to the output file
        echo $(basename "$file") >> "$output_file"
    fi
done

# Define the file names
regular_file="unsolved-regular.txt"
sos1_file="unsolved-sos1.txt"
output_file="unsolved-regular-sos1.txt"

# Clear the output file before starting
> "$output_file"

# Read each line from regular_file
while IFS= read -r regular_line; do
    # Read each line from sos1_file
    while IFS= read -r sos1_line; do
        # Compare the lines and write to output_file if they match
        if [[ "$regular_line" == "$sos1_line" ]]; then
            echo "$regular_line" >> "$output_file"
            break
        fi
    done < "$sos1_file"
done < "$regular_file"
