#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/combinatorial

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -binsearch > "results/literature/binary/combinatorial/$(basename "$file")"
done
