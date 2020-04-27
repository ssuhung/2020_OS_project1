#！/bin/bash

mkdir output

for method in "FIFO" "RR" "SJF" "PSJF"
do
    echo ${method}
    for i in $(seq 1 5)
    do
        sudo dmesg --clear
        ./OS_Proj1 < ./test_data/${method}_${i}.txt > ./output/${method}_${i}_stdout.txt
        dmesg | grep Project1 > ./output/${method}_${i}.txt
    done
done