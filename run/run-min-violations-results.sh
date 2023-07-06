#!/bin/bash

cd ..

jan_sum=0
feb_sum=0
mar_sum=0
may_sum=0

for file in $(ls results/real/min-violations/jan_*_*); do
    # Extract the number after "Primal number of violations:" using grep and awk
    unscheduled=$(grep "Primal number of violations:" "$file" | awk '{print $5}')
    jan_sum=$((jan_sum + unscheduled));
done

for file in $(ls results/real/min-violations/feb_*_*); do
    # Extract the number after "Primal number of violations:" using grep and awk
    unscheduled=$(grep "Primal number of violations:" "$file" | awk '{print $5}')
    feb_sum=$((feb_sum + unscheduled));
done

for file in $(ls results/real/min-violations/mar_*_*); do
    # Extract the number after "Primal number of violations:" using grep and awk
    unscheduled=$(grep "Primal number of violations:" "$file" | awk '{print $5}')
    mar_sum=$((mar_sum + unscheduled));
done

for file in $(ls results/real/min-violations/may_*_*); do
    # Extract the number after "Primal number of violations:" using grep and awk
    unscheduled=$(grep "Primal number of violations:" "$file" | awk '{print $5}')
    may_sum=$((may_sum + unscheduled));
done

# Print the sum for each group
echo "Sum for jan: $jan_sum"
echo "Sum for feb: $feb_sum"
echo "Sum for mar: $mar_sum"
echo "Sum for may: $may_sum"
