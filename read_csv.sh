#!/bin/bash



#while IFS="," read -r  duration

duration_col=($(tail -n +2 logged_times.csv | cut -d ',' -f4))

#read -a duration_col

#tot=0
sum=$(expr 0)
for i in "${duration_col[@]}"; do
	#let tot+=$i
	sum=$(expr $sum + $i)
done

echo "Total: $tot"

echo "Duration: ${duration_col[@]}"

#done < <(tail -n +2 logged_times.csv;) # Ignores header row

