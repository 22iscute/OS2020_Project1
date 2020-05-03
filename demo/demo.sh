#!/bin/bash

dmesg -c
for name in TIME_MEASUREMENT FIFO_1 PSJF_2 RR_3 SJF_4; do
        echo "Processing ${name}..."

        ./PJ1 < "OS_PJ1_Test/${name}.txt"
        dmesg -c | grep Project1
        sleep 2
done
