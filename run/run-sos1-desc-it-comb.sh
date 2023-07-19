#!/bin/bash

cd ..

make

mkdir -p results/real/sos1/desc-iterative/combinatorial

mkdir -p results/literature/sos1/desc-iterative/combinatorial

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -descitsearch > "results/real/sos1/desc-iterative/combinatorial/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -descitsearch > "results/literature/sos1/desc-iterative/combinatorial/$(basename "$file")"
done
