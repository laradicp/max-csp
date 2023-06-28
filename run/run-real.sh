#!/bin/bash

# Find all files that match the pattern "<month>_<id>_<cardinality>.in",
# sort them by id in ascending order, and loop through them

cd ..

make

mkdir -p results/real/cumulative

mkdir -p results/real/regular/heuristic-primal

mkdir -p results/real/sos1/heuristic-primal

mkdir -p results/real/desc-iterative/combinatorial/heuristic-primal

mkdir -p results/real/desc-iterative/trivial

mkdir -p results/real/asc-iterative/combinatorial

mkdir -p results/real/asc-iterative/trivial

mkdir -p results/real/asc-iterative/heuristic

mkdir -p results/real/binary/combinatorial

mkdir -p results/real/binary/trivial

mkdir -p results/real/binary/trivial-combinatorial

mkdir -p results/real/binary/combinatorial-trivial

mkdir -p results/real/binary/heuristic-combinatorial

mkdir -p results/real/binary/heuristic-trivial

for file in $(ls instances/real/jan_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > "results/real/cumulative/$(basename "$file" .in)".out
    ./max-csp.exe "$file" > "results/real/regular/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -heuristic > "results/real/regular/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > "results/real/sos1/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -heuristic > "results/real/sos1/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > "results/real/desc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -descitsearch -heuristic > "results/real/desc-iterative/combinatorial/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > "results/real/desc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > "results/real/asc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > "results/real/asc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -ascitsearch -heuristic > "results/real/asc-iterative/heuristic/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > "results/real/binary/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > "results/real/binary/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb > "results/real/binary/trivial-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -trivialub > "results/real/binary/combinatorial-trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic > "results/real/binary/heuristic-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic -trivialub > "results/real/binary/heuristic-trivial/$(basename "$file" .in)".out
done

for file in $(ls instances/real/feb_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > "results/real/cumulative/$(basename "$file" .in)".out
    ./max-csp.exe "$file" > "results/real/regular/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -heuristic > "results/real/regular/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > "results/real/sos1/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -heuristic > "results/real/sos1/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > "results/real/desc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -descitsearch -heuristic > "results/real/desc-iterative/combinatorial/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > "results/real/desc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > "results/real/asc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > "results/real/asc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -ascitsearch -heuristic > "results/real/asc-iterative/heuristic/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > "results/real/binary/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > "results/real/binary/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb > "results/real/binary/trivial-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -trivialub > "results/real/binary/combinatorial-trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic > "results/real/binary/heuristic-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic -trivialub > "results/real/binary/heuristic-trivial/$(basename "$file" .in)".out
done

for file in $(ls instances/real/mar_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > "results/real/cumulative/$(basename "$file" .in)".out
    ./max-csp.exe "$file" > "results/real/regular/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -heuristic > "results/real/regular/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > "results/real/sos1/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -heuristic > "results/real/sos1/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > "results/real/desc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -descitsearch -heuristic > "results/real/desc-iterative/combinatorial/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > "results/real/desc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > "results/real/asc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > "results/real/asc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -ascitsearch -heuristic > "results/real/asc-iterative/heuristic/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > "results/real/binary/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > "results/real/binary/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb > "results/real/binary/trivial-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -trivialub > "results/real/binary/combinatorial-trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic > "results/real/binary/heuristic-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic -trivialub > "results/real/binary/heuristic-trivial/$(basename "$file" .in)".out
done

for file in $(ls instances/real/may_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > "results/real/cumulative/$(basename "$file" .in)".out
    ./max-csp.exe "$file" > "results/real/regular/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -heuristic > "results/real/regular/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > "results/real/sos1/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -sos1 -heuristic > "results/real/sos1/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > "results/real/desc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -descitsearch -heuristic > "results/real/desc-iterative/combinatorial/heuristic-primal/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > "results/real/desc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > "results/real/asc-iterative/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > "results/real/asc-iterative/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -ascitsearch -heuristic > "results/real/asc-iterative/heuristic/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > "results/real/binary/combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > "results/real/binary/trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb > "results/real/binary/trivial-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -trivialub > "results/real/binary/combinatorial-trivial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic > "results/real/binary/heuristic-combinatorial/$(basename "$file" .in)".out
    ./max-csp.exe "$file" "results/heuristic/$(basename "$file")" -binsearch -heuristic -trivialub > "results/real/binary/heuristic-trivial/$(basename "$file" .in)".out
done
