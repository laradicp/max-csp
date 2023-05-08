#!/bin/bash

make

for file in $(ls instances/sun*); do
    echo "Running $file"
    ./max-csp.exe "$file" -sos1
done
