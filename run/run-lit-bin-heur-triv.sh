#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/heuristic-trivial

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic -trivialub > "results/literature/binary/heuristic-trivial/$(basename "$file")"
done
