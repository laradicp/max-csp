#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/combinatorial-trivial

for file in $(find instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -binsearch -trivialub > "results/literature/binary/combinatorial-trivial/$(basename "$file")"
done
