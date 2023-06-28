#!/bin/bash

cd ..

make

mkdir -p results/literature/regular/heuristic-primal

for file in $(find instances/literature/ -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -heuristic > "results/literature/regular/heuristic-primal/$(basename "$file")"
done
