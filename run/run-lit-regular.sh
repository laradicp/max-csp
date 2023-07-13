#!/bin/bash

cd ..

make

mkdir -p results/literature/regular

for file in $(find instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" > "results/literature/regular/$(basename "$file")"
done
