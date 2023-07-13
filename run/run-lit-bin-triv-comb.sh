#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/trivial-combinatorial

for file in $(find instances/literature/* -maxdepth 1 -type f); do
    echo "Running $file"
    ./max-csp.exe "$file" -binsearch -triviallb > "results/literature/binary/trivial-combinatorial/$(basename "$file")"
done
