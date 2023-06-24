#!/bin/bash

cd ..

make

mkdir -p results/literature/desc-iterative/trivial

for file in $(find instances/literature/ -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > "results/literature/desc-iterative/trivial/$(basename "$file")"
done
