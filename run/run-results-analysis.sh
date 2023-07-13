#!/bin/bash

cd ../results

g++ calculate-gaps.cpp -o calculate-gaps.exe

for file in $(ls ../instances/real/*); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file" .in)".out -real
done

for file in $(ls ../instances/literature/*); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file")" -literature
done

for file in $(ls ../instances/literature/hard/*); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file")" -literature -hard
done
