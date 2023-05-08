#!/bin/bash

make

# run instance-generator.exe to each file in folder testset
# include -lit flag
for file in $(ls testset/*); do
    ./instance-generator.exe "$file" -lit
done
