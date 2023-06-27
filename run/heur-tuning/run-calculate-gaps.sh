#!/bin/bash

cd ../../results/heuristic-tuning

# g++ best-bounds.cpp -o best-bounds.exe

g++ calculate-gaps.cpp -o calculate-gaps.exe

# for file in $(ls ../../instances/literature/hard/*); do
#     echo "Calculating best bounds of file $file"
#     ./best-bounds.exe "$(basename "$file")"
# done

for file in $(ls ../../instances/literature/hard/*); do
    echo "Calculating gaps of $file"
    ./calculate-gaps.exe "$(basename "$file")"
done
