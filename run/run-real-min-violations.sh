#!/bin/bash

cd ..

make

mkdir -p results/real/min-violations/penalize

for file in $(ls instances/real/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -minviolations > "results/real/min-violations/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -minviolations -penalize > "results/real/min-violations/penalize/$(basename "$file" .in)".out
done
