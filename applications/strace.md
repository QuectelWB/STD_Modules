strace
======

examples
-----

>  specific system calls

	$strace -e trace=write ls

> network related system calls

	$strace -e trace=network nc -v -n 127.0.0.1 801

> signal related 

	$strace -e trace=signal nc -v -n 127.0.0.1 801

>  print timestamp of each call

	$strace -r ls




-----


from:
https://www.geeksforgeeks.org/strace-command-in-linux-with-examples/
