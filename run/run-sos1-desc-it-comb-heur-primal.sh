#!/bin/bash

cd ..

make

mkdir -p results/real/sos1/desc-iterative/combinatorial/heuristic-primal

mkdir -p results/literature/sos1/desc-iterative/combinatorial/heuristic-primal

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -descitsearch -heuristic > "results/real/sos1/desc-iterative/combinatorial/heuristic-primal/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -descitsearch -heuristic > "results/literature/sos1/desc-iterative/combinatorial/heuristic-primal/$(basename "$file")"
done
