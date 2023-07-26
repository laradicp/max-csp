#!/bin/bash

cd ..

make

mkdir -p results/real/branching

mkdir -p results/literature/branching

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -branch > "results/real/branching/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -branch > "results/literature/branching/$(basename "$file")"
done
