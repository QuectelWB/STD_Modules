ppp
======

sudo apt-get install ppp


follow the readme.

the easist way: run quectel-pppd.sh

or

copy the quectel-chat-connect quectel-chat-disconnect quectel-ppp to /etc/ppp/peers

modify the quectel-ppp and quectel-chat-connect

execute the cmd

	
	pppd call quectel-ppp &

cross compile the pppd
---

follow the [http://www.linuxfromscratch.org/blfs/view/6.3/basicnet/ppp.html](http://www.linuxfromscratch.org/blfs/view/6.3/basicnet/ppp.html)

wget http://samba.org/ftp/ppp/ppp-2.4.4.tar.gz

then build it.

**pppd depends on the libcap **

libpcap-0.9.6 (needed to do PPP filtering), Linux-PAM-0.99.10.0 (to authenticate incoming calls using PAM), and Linux ATM (to build the pppoatm.so plugin) .

PPP support (CONFIG_PPP), the asynchronous line discipline (CONFIG_PPP_ASYNC), the driver for your serial port device and/or the PPP over Ethernet (PPPoE) protocol driver (CONGIG_PPPOE) must be compiled into the kernel or loaded as kernel modules. Udev doesn't load the ppp_generic and pppoe modules automatically, they must be mentioned in the /etc/sysconfig/modules file. 



:)

