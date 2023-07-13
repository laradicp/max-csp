#!/bin/bash

cd ..

make

mkdir -p results/heuristic

for file in $(ls instances/real/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -heuristic -noexact > "results/heuristic/$(basename "$file")"
done

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -heuristic -noexact > "results/heuristic/$(basename "$file")"
done
