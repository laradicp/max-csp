#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/heuristic-combinatorial

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic > "results/literature/binary/heuristic-combinatorial/$(basename "$file")"
done
