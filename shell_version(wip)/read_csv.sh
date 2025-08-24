#!/bin/bash



duration_col=($(tail -n +2 logged_times.csv | cut -d ',' -f4))

sum=0

for i in "${duration_col[@]}"; do
	sum=$((sum + i))
done

echo "Total: $sum"

echo "Duration: ${duration_col[@]}"


