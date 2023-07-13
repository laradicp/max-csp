#!/bin/bash

cd ..

make

mkdir -p results/literature/asc-iterative/heuristic

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -ascitsearch -heuristic > "results/literature/asc-iterative/heuristic/$(basename "$file")"
done
