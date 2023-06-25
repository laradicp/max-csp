#!/bin/bash

cd ..

make

mkdir -p results/literature/asc-iterative/heuristic

for file in $(find instances/literature/ -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -ascitsearch -heuristic > "results/literature/asc-iterative/heuristic/$(basename "$file")"
done
