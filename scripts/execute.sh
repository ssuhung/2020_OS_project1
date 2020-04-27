mkdir output

for method in FIFO RR SJF PSJF
do 
	echo ${method}
	for i in $(seq 1 5)
	do
		echo processing through ${method}_${i}
		sudo dmesg --clear 
		sudo ../OS_Proj1 <../test_data/${method}_${i}.txt >../output/${method}_${i}_stdout.txt 
		dmesg | grep Project1 > ../output/${method}_${i}_dmesg.txt
	done
done

echo time measurement
sudo dmesg --clear
sudo ../OS_Proj1 <./test_data/TIME_MEASUREMENT.txt  
dmesg | grep Projext1 > ../output/TIME_MEASURE_dmesg.txt
echo All done




















