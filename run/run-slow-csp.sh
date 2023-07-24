#!/bin/bash

cd ..

make

mkdir -p results/real/slow-csp

mkdir -p results/literature/slow-csp

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -minpacedelay > "results/real/slow-csp/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -minpacedelay > "results/literature/slow-csp/$(basename "$file")"
done
