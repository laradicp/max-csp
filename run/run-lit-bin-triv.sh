#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/trivial

for file in $(find instances/literature/ -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > "results/literature/binary/trivial/$(basename "$file")"
done
