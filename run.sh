#!/bin/bash

# Find all files that match the pattern "<month>_<id>_<cardinality>.in",
# sort them by id in ascending order, and loop through them

for file in $(ls instances/jan_*_* | sort -t_ -n -k2); do
    ./max-csp.exe "$file"
done

for file in $(ls instances/feb_*_* | sort -t_ -n -k2); do
    ./max-csp.exe "$file"
done

for file in $(ls instances/mar_*_* | sort -t_ -n -k2); do
    ./max-csp.exe "$file"
done

for file in $(ls instances/may_*_* | sort -t_ -n -k2); do
    ./max-csp.exe "$file"
done

