#!/bin/bash

cd ../results

g++ instance-plot-data.cpp -o instance-plot-data.exe

# Clear output files before starting
> "instance-plot-data.txt"

for file in $(find ../instances/literature/* -type f); do
    echo "Running $file"
    ./instance-plot-data.exe "$file" -maxvals
done

for file in $(find literature/regular/heuristic-primal/* -type f); do
    echo "Running $file"
    ./instance-plot-data.exe "$file" >> instance-plot-data.txt
done
