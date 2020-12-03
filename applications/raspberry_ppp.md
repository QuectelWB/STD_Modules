ppp
======

Debian系统上如果没有pppd，通过apt安装

	sudo apt-get install ppp
	

参考Quectel的Readme文件。即
	
	quectel-pppd.sh
	
或者

	copy the quectel-chat-connect quectel-chat-disconnect quectel-ppp to /etc/ppp/peers
	modify the quectel-ppp and quectel-chat-connect
	execute "pppd call quectel-ppp &"

cross compile the pppd
---

follow the [http://www.linuxfromscratch.org/blfs/view/6.3/basicnet/ppp.html](http://www.linuxfromscratch.org/blfs/view/6.3/basicnet/ppp.html)

下载pppd源码
	
	https://ppp.samba.org/download.html
	http://samba.org/ftp/ppp/ppp-2.4.4.tar.gz

pppd 依赖于libcap.

	libpcap-0.9.6 ---> do PPP filtering
	Linux-PAM-0.99.10.0 ---> authenticate incoming calls using PAM
	Linux ATM ---> pppoatm.so 

PPP support (CONFIG_PPP), the asynchronous line discipline (CONFIG_PPP_ASYNC), the driver for your serial port device and/or the PPP over Ethernet (PPPoE) protocol driver (CONGIG_PPPOE) must be compiled into the kernel or loaded as kernel modules. Udev doesn't load the ppp_generic and pppoe modules automatically, they must be mentioned in the /etc/sysconfig/modules file. 

移植参考
[ppp移植](http://www.linuxfromscratch.org/blfs/view/6.3/basicnet/ppp.html)

wvdial
-----

wvdial是一个简单好用的Linux 下的拨号工具。它完全不需要chat scripts，也不用你去编辑pap-secrets 和chap-secrets，它完全能够自动发现你modem ，并自动选择合适的初始化串。利用wvdial 和ppp 可以实现linux 下的轻松上网。在整个过程中wvdial 的作用是拨号并等待提示，并根据提示输入相应的用户名和密码等认证信息。




:)

