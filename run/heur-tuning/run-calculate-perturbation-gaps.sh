#!/bin/bash

cd ../../results/heuristic-tuning

g++ best-bounds.cpp -o best-bounds.exe

g++ calculate-perturbation-gaps.cpp -o calculate-perturbation-gaps.exe

# Clear output files before starting
> "best-dual.txt"
> "best-primal.txt"

for file in $(ls ../../instances/literature/hard/*); do
    echo "Calculating best bounds of file $file"
    ./best-bounds.exe "$(basename "$file")"
done

for file in $(ls ../../instances/literature/hard/*); do
    echo "Calculating gaps of $file"
    ./calculate-perturbation-gaps.exe "$(basename "$file")"
done
