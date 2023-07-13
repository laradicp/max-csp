#!/bin/bash

cd ../results

g++ calculate-gaps.cpp -o calculate-gaps.exe

for file in $(find ../instances/real/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file" .in)".out -real
    ./calculate-gaps.exe "$(basename "$file" .in)".out -real -gap
done

for file in $(find ../instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file")" -literature
    ./calculate-gaps.exe "$(basename "$file")" -literature -gap
done

for file in $(find ../instances/literature/hard/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file")" -literature -hard
done
