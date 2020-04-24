#!/usr/bin/env bash

# This script takes the timing details from dmesg and
# calculates each tasks converted finish time

input=$1
qt=0.001727558  # length of time unit measured with get_time.sh
begin=0.0
cnt=0

while IFS= read -r line
do
    if [ ! "$line" ]; then
        break
    fi

    pid=$(echo $line | cut -f3 -d" ")
    time1=$(echo $line | cut -f4 -d" ")
    time2=$(echo $line | cut -f5 -d" ")

    if [ $cnt -eq 0 ]; then
        begin=$time1
        cnt=1
    fi

    diff=$(echo "scale=9; $time2-$begin" | bc -q)
    rec=$(echo "scale=9; $diff/$qt" | bc -q)
    echo "$pid finish at $rec unit" 

done < $input

