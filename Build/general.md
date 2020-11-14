To the superior this section customer ignores, suggests the customer to check the compilation error by itself

Ubuntu 

All the tools and driver source code Quectel releases have been verified on mainstream Liunx systems, from kernel 2.6 to the latest kernel versions, but there are cases where the customer has encountered problems during compilation.


### SecureBoot

	modprobe: unsigned module loading is restricted; see man kernel_lockdown.7

rootcause:
	
[SecureBoot](https://unix.stackexchange.com/questions/543576/modprobe-fails-with-operation-not-permitted)


![](VM_SecureBoot.png)


### 


### Stdarg.h

You might meet this: 

![](stdarg.h)


Add this into Makefile:
	ccflags-y = -Wno-declaration-after-statement -mpopcnt -I/usr/lib/gcc/x86_64-linux-gnu/5.4.0/include


### quectel-CM sa_family_t
编译遇到报错

	v@v-VirtualBox:~/Smart210/Quectel/quectel-CM$ make
	rm -rf quectel-CM *~
	rm -rf quectel-qmi-proxy
	arm-linux-gcc -Wall -s quectel-qmi-proxy.c  -o quectel-qmi-proxy -lpthread -ldl
	arm-linux-gcc -Wall -s quectel-mbim-proxy.c  -o quectel-mbim-proxy -lpthread -ldl
	In file included from quectel-mbim-proxy.c:1:0:
	/home/v/Smart210/opt/FriendlyARM/toolschain/4.5.1/bin/../lib/gcc/arm-none-linux-gnueabi/4.5.1/../../../../arm-none-linux-gnueabi/include/linux/un.h:7:2: error: expected specifier-qualifier-list before 'sa_family_t'
	quectel-mbim-proxy.c: In function 'proxy_make_server':
	quectel-mbim-proxy.c:130:13: error: 'struct sockaddr_un' has no member named 'sun_family'
	quectel-mbim-proxy.c:131:13: error: 'struct sockaddr_un' has no member named 'sun_path'
	quectel-mbim-proxy.c:132:22: error: 'struct sockaddr_un' has no member named 'sun_path'
	quectel-mbim-proxy.c:139:32: error: 'struct sockaddr_un' has no member named 'sun_path'
	make: *** [mbim-proxy] Error 1

gcc （包括cross_compile）版本低(小于4.6)有这个问题。

在没办法升级cross_compile tool 的情况下，可以考虑降quectel-CM版本，或者修改quectel-CM源码。对大多数情况，mbim-proxy用不到。


###




###



