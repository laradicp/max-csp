#!/bin/bash

# Find all files that match the pattern "<month>_<id>_<cardinality>.in",
# sort them by id in ascending order, and loop through them

make

for file in $(ls instances/real/jan_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > results/real/cumulative/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" > results/real/regular/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > results/real/sos1/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > results/real/desc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > results/real/desc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > results/real/asc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > results/real/asc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > results/real/binary/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > results/real/binary/trivial/"$(basename "$file" .in)".out
done

for file in $(ls instances/real/feb_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > results/real/cumulative/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" > results/real/regular/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > results/real/sos1/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > results/real/desc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > results/real/desc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > results/real/asc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > results/real/asc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > results/real/binary/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > results/real/binary/trivial/"$(basename "$file" .in)".out
done

for file in $(ls instances/real/mar_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > results/real/cumulative/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" > results/real/regular/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > results/real/sos1/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > results/real/desc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > results/real/desc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > results/real/asc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > results/real/asc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > results/real/binary/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > results/real/binary/trivial/"$(basename "$file" .in)".out
done

for file in $(ls instances/real/may_*_* | sort -t_ -n -k2); do
    echo "Running $file"
    ./max-csp.exe "$file" -cumulative > results/real/cumulative/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" > results/real/regular/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -sos1 > results/real/sos1/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch > results/real/desc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -descitsearch -triviallb -trivialub > results/real/desc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch > results/real/asc-iterative/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -ascitsearch -triviallb -trivialub > results/real/asc-iterative/trivial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch > results/real/binary/combinatorial/"$(basename "$file" .in)".out
    ./max-csp.exe "$file" -binsearch -triviallb -trivialub > results/real/binary/trivial/"$(basename "$file" .in)".out
done

