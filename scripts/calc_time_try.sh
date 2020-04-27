#!/usr/bin/env bash

# This script takes the timing details from dmesg and
# calculates each tasks converted finish time

qt=0.001554701  # length of time unit measured with get_time.sh

for method in FIFO RR SJF PSJF
do
	echo ${method} 
	for i in $(seq 1 5)
	do
		input="../output/${method}_${i}_dmesg.txt"
		begin=0.0
		cnt=0

		while IFS= read -r line;
		do
		    if [ ! "$line" ]; then
			break
		    fi

		    pid=$(echo $line | cut -d' ' -f4 )
		    time1=$(echo $line | cut -d' ' -f5)
		    time2=$(echo $line | cut -d' ' -f6)

		    if [ $cnt -eq 0 ]; then
			begin=$time1
			cnt=1
		    fi

		    diff=$(echo "scale=9; $time2-$begin" | bc -q)
		    rec=$(echo "scale=9; $diff/$qt" | bc -q)
		    echo  "$pid finish at $rec unit" 
		done < $input
		echo --------------------
	done
done
