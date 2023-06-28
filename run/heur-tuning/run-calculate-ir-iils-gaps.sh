#!/bin/bash

cd ../../results/heuristic-tuning

# g++ best-bounds.cpp -o best-bounds.exe

g++ calculate-ir-iils-gaps.cpp -o calculate-ir-iils-gaps.exe

# for file in $(ls ../../instances/literature/hard/*); do
#     echo "Calculating best bounds of file $file"
#     ./best-bounds.exe "$(basename "$file")"
# done

for file in $(ls ../../instances/literature/hard/*); do
    echo "Calculating gaps of $file"
    ./calculate-ir-iils-gaps.exe "$(basename "$file")"
done
