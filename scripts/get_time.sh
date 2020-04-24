#!/usr/bin/env bash

# This scripts help measure the estimated length of each time unit

input="../kernel_log/base_Sean"
accum=0.0

while IFS= read -r line
do
    pid=$(echo $line | cut -f4 -d" ")
    time1=$(echo $line | cut -f5 -d" ")
    time2=$(echo $line | cut -f6 -d" ")
    diff=$(echo "scale=9; $time2-$time1" | bc -q)
    accum=$(echo "scale=9; $accum+$diff" | bc -q) 
done < $input

echo "accum=$accum"
bench=$(echo "scale=9; $accum/5000.0" | bc -q)
echo "time unit= $bench sec"
