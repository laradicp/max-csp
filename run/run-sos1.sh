#!/bin/bash

cd ..

make

mkdir -p results/real/sos1

mkdir -p results/literature/sos1

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 > "results/real/sos1/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 > "results/literature/sos1/$(basename "$file")"
done
