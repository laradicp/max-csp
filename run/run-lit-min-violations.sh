#!/bin/bash

cd ..

make

mkdir -p results/literature/min-violations/penalize

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -minviolations > "results/literature/min-violations/$(basename "$file")"
    ./max-csp.exe "$file" -minviolations -penalize > "results/literature/min-violations/penalize/$(basename "$file")"
done
