Tools
======

strace
-----

>  specific system calls

	$strace -e trace=write ls

> network related system calls

	$strace -e trace=network nc -v -n 127.0.0.1 801

> signal related 

	$strace -e trace=signal nc -v -n 127.0.0.1 801

>  print timestamp of each call

	$strace -r ls



from:
[https://www.geeksforgeeks.org/strace-command-in-linux-with-examples/](https://www.geeksforgeeks.org/strace-command-in-linux-with-examples/)

fuser lsof
-----

打开设备失败，在Ubuntu上有时候和modemmanager 有关

	q@q-OptiPlex-7070:~$ dpkg -s modemmanager
	
	#Ubuntu/Debian
	apt-get remove --purge Modemmanager
	#Centos/Redhat
	yum remove Modemmanager


lsof 和 fuser 是两个非常好用的指令查哪些


 /dev/ttyUSB0

	lsof | grep ttyUSB
	The associated process service is returned

	fuser /dev/ttyUSB0
	PID is returned and the process is queried with the "ps" command

dns
--------

host dig nslookup



