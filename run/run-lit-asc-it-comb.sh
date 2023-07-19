#!/bin/bash

cd ..

make

mkdir -p results/literature/asc-iterative/combinatorial

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -ascitsearch > "results/literature/asc-iterative/combinatorial/$(basename "$file")"
done
