#!/bin/bash

cd ..

make

mkdir -p results/heuristic

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -heuristic -noexact > "results/heuristic/$(basename "$file")"
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -heuristic -noexact > "results/heuristic/$(basename "$file")"
done
