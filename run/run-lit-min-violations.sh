#!/bin/bash

cd ..

make

mkdir -p results/literature/min-violations/penalize

for file in $(find instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    timeout 11m ./max-csp.exe "$file" -minviolations > "results/literature/min-violations/$(basename "$file")"
    timeout 11m ./max-csp.exe "$file" -minviolations -penalize > "results/literature/min-violations/penalize/$(basename "$file")"
done
