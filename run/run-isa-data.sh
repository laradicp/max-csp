#!/bin/bash

cd ../results

g++ isa-data.cpp -o isa-data.exe

echo "Calculating max values"
for file in $(find ../instances/real/* -type f); do
    ./isa-data.exe "$file" -maxvals
done

# Clear output files before starting
> "instance-plot-data.txt"
echo "instances	feature_nb_cars	feature_nb_options	feature_nb_classes	feature_min_util	feature_avg_util	feature_max_util	feature_std_util	feature_avg_options	feature_min_pq_ratio	feature_avg_pq_ratio	feature_max_pq_ratio	feature_std_pq_ratio	feature_lcm_q_vals	feature_class_pop	algo_H	algo_F1u	algo_F1i	algo_IIu	algo_IIi	algo_IDu	algo_IDi	algo_IBu	algo_IBi	algo_F2u	algo_F2i	algo_F2Iu	algo_F2Ii	algo_F2Du	algo_F2Di	algo_F2Bu	algo_F2Bi" >> "instance-plot-data.txt"

for file in $(find ../instances/real/* -type f); do
    echo "Running $file"
    ./isa-data.exe "$file" >> "instance-plot-data.txt"
done
