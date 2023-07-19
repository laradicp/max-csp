#!/bin/bash

cd ..

make

mkdir -p results/real/sos1/asc-iterative/combinatorial

mkdir -p results/literature/sos1/asc-iterative/combinatorial

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -ascitsearch > "results/real/sos1/asc-iterative/combinatorial/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -ascitsearch > "results/literature/sos1/asc-iterative/combinatorial/$(basename "$file")"
done
