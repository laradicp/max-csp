#!/bin/bash

cd ..

make

mkdir -p results/real/branching/heuristic-primal

mkdir -p results/literature/branching/heuristic-primal

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -branch -heuristic > "results/real/branching/heuristic-primal/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -branch -heuristic > "results/literature/branching/heuristic-primal/$(basename "$file")"
done
