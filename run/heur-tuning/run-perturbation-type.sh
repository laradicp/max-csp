#!/bin/bash

cd ../..

make

mkdir -p results/heuristic-tuning/removal/50

mkdir -p results/heuristic-tuning/removal/25

mkdir -p results/heuristic-tuning/removal/20

mkdir -p results/heuristic-tuning/removal/15

mkdir -p results/heuristic-tuning/removal/10

mkdir -p results/heuristic-tuning/removal/5

mkdir -p results/heuristic-tuning/swap/50

mkdir -p results/heuristic-tuning/swap/25

mkdir -p results/heuristic-tuning/swap/20

mkdir -p results/heuristic-tuning/swap/15

mkdir -p results/heuristic-tuning/swap/10

mkdir -p results/heuristic-tuning/swap/5

mkdir -p results/heuristic-tuning/both/50

mkdir -p results/heuristic-tuning/both/25

mkdir -p results/heuristic-tuning/both/20

mkdir -p results/heuristic-tuning/both/15

mkdir -p results/heuristic-tuning/both/10

mkdir -p results/heuristic-tuning/both/5

for file in $(ls instances/literature/hard/*); do
    echo "Running $file"
    for ((i=1; i<=10; i++)); do
        echo $i
        ./max-csp.exe "$file" "results/heuristic-tuning/removal/50/$(basename "$file")" 1 0.5 0 50
        ./max-csp.exe "$file" "results/heuristic-tuning/swap/50/$(basename "$file")" 1 0.5 1 50
        ./max-csp.exe "$file" "results/heuristic-tuning/both/50/$(basename "$file")" 1 0.5 2 50
        ./max-csp.exe "$file" "results/heuristic-tuning/removal/25/$(basename "$file")" 1 0.5 0 25
        ./max-csp.exe "$file" "results/heuristic-tuning/swap/25/$(basename "$file")" 1 0.5 1 25
        ./max-csp.exe "$file" "results/heuristic-tuning/both/25/$(basename "$file")" 1 0.5 2 25
        ./max-csp.exe "$file" "results/heuristic-tuning/removal/20/$(basename "$file")" 1 0.5 0 20
        ./max-csp.exe "$file" "results/heuristic-tuning/swap/20/$(basename "$file")" 1 0.5 1 20
        ./max-csp.exe "$file" "results/heuristic-tuning/both/20/$(basename "$file")" 1 0.5 2 20
        ./max-csp.exe "$file" "results/heuristic-tuning/removal/15/$(basename "$file")" 1 0.5 0 15
        ./max-csp.exe "$file" "results/heuristic-tuning/swap/15/$(basename "$file")" 1 0.5 1 15
        ./max-csp.exe "$file" "results/heuristic-tuning/both/15/$(basename "$file")" 1 0.5 2 15
        ./max-csp.exe "$file" "results/heuristic-tuning/removal/10/$(basename "$file")" 1 0.5 0 10
        ./max-csp.exe "$file" "results/heuristic-tuning/swap/10/$(basename "$file")" 1 0.5 1 10
        ./max-csp.exe "$file" "results/heuristic-tuning/both/10/$(basename "$file")" 1 0.5 2 10
        ./max-csp.exe "$file" "results/heuristic-tuning/removal/5/$(basename "$file")" 1 0.5 0 5
        ./max-csp.exe "$file" "results/heuristic-tuning/swap/5/$(basename "$file")" 1 0.5 1 5
        ./max-csp.exe "$file" "results/heuristic-tuning/both/5/$(basename "$file")" 1 0.5 2 5
        sleep 1
    done
done
