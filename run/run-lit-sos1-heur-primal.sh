#!/bin/bash

cd ..

make

mkdir -p results/literature/sos1/heuristic-primal

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -heuristic > "results/literature/sos1/heuristic-primal/$(basename "$file")"
done
