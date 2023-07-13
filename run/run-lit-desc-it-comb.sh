#!/bin/bash

cd ..

make

mkdir -p results/literature/desc-iterative/combinatorial

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -descitsearch > "results/literature/desc-iterative/combinatorial/$(basename "$file")"
done
