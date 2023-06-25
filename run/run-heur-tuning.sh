#!/bin/bash

cd ..

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
        ./max-csp.exe "$file" 1 0.5 0 50 >> "results/heuristic-tuning/removal/50/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 1 50 >> "results/heuristic-tuning/swap/50/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 2 50 >> "results/heuristic-tuning/both/50/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 0 25 >> "results/heuristic-tuning/removal/25/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 1 25 >> "results/heuristic-tuning/swap/25/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 2 25 >> "results/heuristic-tuning/both/25/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 0 20 >> "results/heuristic-tuning/removal/20/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 1 20 >> "results/heuristic-tuning/swap/20/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 2 20 >> "results/heuristic-tuning/both/20/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 0 15 >> "results/heuristic-tuning/removal/15/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 1 15 >> "results/heuristic-tuning/swap/15/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 2 15 >> "results/heuristic-tuning/both/15/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 0 10 >> "results/heuristic-tuning/removal/10/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 1 10 >> "results/heuristic-tuning/swap/10/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 2 10 >> "results/heuristic-tuning/both/10/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 0 5 >> "results/heuristic-tuning/removal/5/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 1 5 >> "results/heuristic-tuning/swap/5/$(basename "$file")"
        ./max-csp.exe "$file" 1 0.5 2 5 >> "results/heuristic-tuning/both/5/$(basename "$file")"
        sleep 1
    done
done

# Filter "Primal"
grep "Primal" results/heuristic-tuning/removal/50/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/50/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/removal/25/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/25/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/removal/20/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/20/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/removal/15/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/15/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/removal/10/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/10/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/removal/5/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/5/filtered-benchmark-primal.txt

grep "Primal" results/heuristic-tuning/swap/50/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/50/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/swap/25/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/25/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/swap/20/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/20/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/swap/15/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/15/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/swap/10/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/10/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/swap/5/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/5/filtered-benchmark-primal.txt

grep "Primal" results/heuristic-tuning/both/50/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/50/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/both/25/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/25/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/both/20/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/20/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/both/15/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/15/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/both/10/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/10/filtered-benchmark-primal.txt
grep "Primal" results/heuristic-tuning/both/5/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/5/filtered-benchmark-primal.txt

# Filter "Time"
grep "Time" results/heuristic-tuning/removal/50/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/50/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/removal/25/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/25/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/removal/20/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/20/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/removal/15/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/15/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/removal/10/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/10/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/removal/5/* | sed 's/^[^:]*://' > results/heuristic-tuning/removal/5/filtered-benchmark-time.txt

grep "Time" results/heuristic-tuning/swap/50/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/50/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/swap/25/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/25/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/swap/20/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/20/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/swap/15/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/15/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/swap/10/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/10/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/swap/5/* | sed 's/^[^:]*://' > results/heuristic-tuning/swap/5/filtered-benchmark-time.txt

grep "Time" results/heuristic-tuning/both/50/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/50/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/both/25/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/25/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/both/20/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/20/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/both/15/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/15/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/both/10/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/10/filtered-benchmark-time.txt
grep "Time" results/heuristic-tuning/both/5/* | sed 's/^[^:]*://' > results/heuristic-tuning/both/5/filtered-benchmark-time.txt
