#!/bin/bash

cd ../results

g++ calculate-gaps.cpp -o calculate-gaps.exe

for file in $(find ../instances/real/ -type f); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file" .in)".out -real
done

for file in $(find ../instances/literature/ -type f); do
    echo "Running $file"
    ./calculate-gaps.exe "$(basename "$file")" -literature
done
