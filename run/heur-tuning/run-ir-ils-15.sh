#!/bin/bash

cd ../..

make

mkdir -p results/heuristic-tuning/pDiv-15/ir-5/j-2

mkdir -p results/heuristic-tuning/pDiv-15/ir-5/j

mkdir -p results/heuristic-tuning/pDiv-15/ir-5/2j

mkdir -p results/heuristic-tuning/pDiv-15/ir-10/j-2

mkdir -p results/heuristic-tuning/pDiv-15/ir-10/j

mkdir -p results/heuristic-tuning/pDiv-15/ir-10/2j

mkdir -p results/heuristic-tuning/pDiv-15/ir-15/j-2

mkdir -p results/heuristic-tuning/pDiv-15/ir-15/j

mkdir -p results/heuristic-tuning/pDiv-15/ir-15/2j

mkdir -p results/heuristic-tuning/pDiv-15/ir-25/j-2

mkdir -p results/heuristic-tuning/pDiv-15/ir-25/j

mkdir -p results/heuristic-tuning/pDiv-15/ir-25/2j

mkdir -p results/heuristic-tuning/pDiv-15/ir-50/j-2

mkdir -p results/heuristic-tuning/pDiv-15/ir-50/j

mkdir -p results/heuristic-tuning/pDiv-15/ir-50/2j

for file in $(ls instances/literature/hard/*); do
    echo "Running $file"
    for ((i=1; i<=10; i++)); do
        echo $i
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-5/j-2/$(basename "$file")" 5 0.5 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-5/j/$(basename "$file")" 5 1 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-5/2j/$(basename "$file")" 5 2 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-10/j-2/$(basename "$file")" 10 0.5 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-10/j/$(basename "$file")" 10 1 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-10/2j/$(basename "$file")" 10 2 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-15/j-2/$(basename "$file")" 15 0.5 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-15/j/$(basename "$file")" 15 1 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-15/2j/$(basename "$file")" 15 2 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-25/j-2/$(basename "$file")" 25 0.5 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-25/j/$(basename "$file")" 25 1 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-25/2j/$(basename "$file")" 25 2 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-50/j-2/$(basename "$file")" 50 0.5 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-50/j/$(basename "$file")" 50 1 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/pDiv-15/ir-50/2j/$(basename "$file")" 50 2 2 15
        sleep 1
    done
done
