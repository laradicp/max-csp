#!/bin/bash

cd ..

make

mkdir -p results/real/sos1/asc-iterative/trivial

mkdir -p results/literature/sos1/asc-iterative/trivial

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -ascitsearch -triviallb -trivialub > "results/real/sos1/asc-iterative/trivial/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -ascitsearch -triviallb -trivialub > "results/literature/sos1/asc-iterative/trivial/$(basename "$file")"
done
