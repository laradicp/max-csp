#!/bin/bash

cd ..

make

mkdir -p results/real/sos1/desc-iterative/trivial

mkdir -p results/literature/sos1/desc-iterative/trivial

for file in $(find instances/real/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -descitsearch -triviallb -trivialub > "results/real/sos1/desc-iterative/trivial/$(basename "$file" .in)".out
done

for file in $(find instances/literature/* -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1 -descitsearch -triviallb -trivialub > "results/literature/sos1/desc-iterative/trivial/$(basename "$file")"
done
