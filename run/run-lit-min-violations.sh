#!/bin/bash

cd ..

make

mkdir -p results/literature/min-violations/penalize

for file in $(find instances/literature/ -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -minviolations > "results/literature/min-violations/$(basename "$file")"
    ./max-csp.exe "$file" -minviolations -penalize > "results/literature/min-violations/penalize/$(basename "$file")"
done
