#!/bin/bash

cd ..

make

mkdir -p results/literature/regular

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" > "results/literature/regular/$(basename "$file")"
done
