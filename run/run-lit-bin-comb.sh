#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/combinatorial

for file in $(find instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -binsearch > "results/literature/binary/combinatorial/$(basename "$file")"
done
