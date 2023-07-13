#!/bin/bash

cd ..

make

mkdir -p results/literature/sos1

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 > "results/literature/sos1/$(basename "$file")"
done
