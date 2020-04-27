sudo dmesg --clear
echo processing Time Measurement.txt
sudo ../OS_Proj1 < ../test_data/TIME_MEASUREMENT.txt > ../output/TIME_MEASUREMENT_stdout.txt
dmesg | grep Project1 > ../output/TIME_MEASUREMENT_dmesg.txt

sudo dmesg --clear
sudo ../OS_Proj1 < ../test_data/FIFO_1.txt > ../output/FIFO_1_stdout.txt
echo processing FIFO_1.txt
dmesg | grep Project1 > ../output/FIFO_1_dmesg.txt

sudo dmesg --clear
echo processing PSJF_2.txt
sudo ../OS_Proj1 < ../test_data/PSJF_2.txt > ../output/PSJF_2_stdout.txt
dmesg | grep Project1 > ../output/PSJF_2_dmesg.txt

sudo dmesg --clear
echo processing RR_3.txt
sudo ../OS_Proj1 < ../test_data/RR_3.txt > ../output/RR_3_stdout.txt
dmesg | grep Project1 > ../output/RR_3_dmesg.txt

sudo dmesg --clear
echo processing SJF_4.txt
sudo ../OS_Proj1 < ../test_data/SJF_4.txt > ../output/SJF_4_stdout.txt
dmesg | grep Project1 > ../output/SJF_4_dmesg.txt
