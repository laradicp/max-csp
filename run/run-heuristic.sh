#!/bin/bash

cd ..

make

mkdir -p results/heuristic

for file in $(find instances/ -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -heuristic -noexact > "results/heuristic/$(basename "$file")"
done
