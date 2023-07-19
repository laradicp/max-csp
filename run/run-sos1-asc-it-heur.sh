#!/bin/bash

cd ..

make

mkdir -p results/real/sos1/asc-iterative/heuristic

mkdir -p results/literature/sos1/asc-iterative/heuristic

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -ascitsearch -heuristic > "results/real/sos1/asc-iterative/heuristic/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -ascitsearch -heuristic > "results/literature/sos1/asc-iterative/heuristic/$(basename "$file")"
done
