#!/bin/bash

cd ..

make

mkdir -p results/literature/binary/trivial

for file in $(ls instances/literature/*); do
    echo "Running $file"
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > "results/literature/binary/trivial/$(basename "$file")"
done
