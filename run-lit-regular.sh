#!/bin/bash

make

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" > "results/literature/regular/$(basename "$file")"
done
