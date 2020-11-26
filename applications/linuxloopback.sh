#!/bin/bash

echo 20000 > /proc/sys/net/core/netdev_max_backlog

echo 80 > /sys/class/net/rmnet_usb0.1/queues/rx-0/rps_cpus

echo 40 > /sys/class/net/rmnet_usb0/queues/rx-0/rps_cpus

# cpu设置全部online，配置在最高性能

for i in {1..7}
do
	echo 1 > /sys/bus/cpu/devices/cpu$i/online
	echo performance >  /sys/devices/system/cpu/cpufreq/policy$i/scaling_governor
done


