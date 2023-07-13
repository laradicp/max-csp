#!/bin/bash

cd ..

make

mkdir -p results/literature/desc-iterative/combinatorial/heuristic-primal

for file in $(find instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -descitsearch -heuristic > "results/literature/desc-iterative/combinatorial/heuristic-primal/$(basename "$file")"
done
