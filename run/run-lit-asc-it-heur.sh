#!/bin/bash

cd ..

make

mkdir -p results/literature/asc-iterative/heuristic

for file in $(find instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -ascitsearch -heuristic > "results/literature/asc-iterative/heuristic/$(basename "$file")"
done
