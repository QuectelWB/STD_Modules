网络不通的处理情况
======

客户的Host接上模组,拨号后依然无法正常访问网络,譬如ping不通地址和域名,无法进行http访问的情况, 需要根据具体情况具体分析


> 先行检查

模组的SIM卡本身正常,非专网SIM或其他特殊SIM卡.

> Host 侧可能造成网络不通的原因

- 拨号本身就报错了;IP地址是静态配置的IP地址
- DNS不对
- 路由route不对
- iptables
- 模块的原因

排查顺序也按照这个顺序. 通常用一些常用的AT指令和Linux指令操作检查后依然无法找到原因, 需要抓tcpdump\usbmon\QLog.


##拨号的注意事项


- 无论是quectel-CM拨号还是其他ECM/RNDIS的拨号,Host都需要发送DHCP请求到模组后才能拿到IP地址,静态分配的IP地址当然是不行的
- Host用网卡拨号拿到的IP地址是基站分配的IP的情况,该IP地址和AT+CGPADDR查询到的结果应该一样. 不包括ppp拨号. 包括EC2X/EC2XX/RGXXX等4G/5G模组
- Host拿到的IP地址是模块分配的IP地址,包括高通模组的ECM和RNDIS网卡拨号, 以及ASR/海思/...的网卡拨号又不启用NAT的情况(AT+QCFG="nat",0),这种情况Host拿到的IP地址一般是192.168.X.X. 这种情况Host上模组对应的网卡的网关**必须**配置成192.168.X.1且必须配置,DNS也可以用192.168.X.1. 高通模组的标准模组和Open模组有区别,标准模组的ECM/RNDIS是自动拨号,Open模组需要用API拨号并且配置iptables将rmnet_data0配置成out口. 非高通的标准模组, 需要用AT+QNETDEVCTL拨号.

##DNS

能ping通地址,但是ping不通域名

	ping 8.8.8.8 -I wwan0 
	ping www.baidu.com -I wwan0
	curl www.baidu.com
	nslookup www.baidu.com 114.114.114.114

许多情况, 目标机器的/etc/resolv.conf 无法写入或者根本没有写入有效的DNS, 网络自然不通. 甚至遇到过, 目标平台的/etc/resolv.conf, /etc/hosts根本没有.

	when used with glibc, the busybox 'networking' applets will similarly reuire that you install at least some of the glibc NSS stuff(in particular, /etc/sswitch.conf, /lib/libnss_dns*,/lib/libnss_files*,and /lib/libresolv*)

这是文件系统中没有将交叉编译工具链中对应的文件拷贝进去导致的.

有一种情况, 不通运营商不同的SIM卡在同一个机器上使用, 基站分配不同的DNS导致无法ping域名, 停掉该系统上的systemd-resolvd服务后,固定成一个公用的DNS后一切正常.

**关于DNS的问题,优先让用户自己解决,这和用户自己的目标平台有关**

##route

route -n/netstat -rn/ip route show 看下当前系统的route配置, 模组网卡必须加到默认路由中, 可以

	route add default dev wwwan0
	// 对应删除的指令一般是 route del default gw 0.0.0.0 dev wwan0

遇到过一种情况,AG15的RNDIS网卡usb0以及5G模组的rmnet_usb0会抢占默认路由并且metric更小, 导致rmnet_usb0.1无效. 后来禁掉connonman 服务后无该问题. 后续用户自己解决.

##iptables

排序iptables导致的,清除所有iptables

	iptables -F
	iptables -t nat -F

机器上禁用ICMP, 抓发到一个无效的网卡上,导致无法联网的情况也是可能的.

##驱动和模块

该模组如果放在其他机器上可以正常使用, 基本上就和模组本身无法而是需要排查模组使用方法. 模组固件一直在更新, 建议使用最新固件.

排查模组问题, 最有效的方法是抓模组Log

每ping一次,ifconfig 应该能看到网卡上有对应的状态变化, tcpdump/usbmon 也会抓到对应的数据. 

常用的几条和模块可用的AT

	AT+CGPADDR 和 AT+CGCONTRDP :看到拨号后的模块IP地址;是不是拨在第一路了?
	AT+CGDCONT?   :多路拨号的时候,该路PDP是否已经配置成ims或者sos了?
	AT+QPING  :模块能不能ping通,分别ping下地址和域名


## Debug

### tcpdump 

	root@LipRouter:/sys/kernel/debug/usb# tcpdump -i usb5 port 68 and port 67 &
	root@LipRouter:/sys/kernel/debug/usb# tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
	listening on usb5, link-type EN10MB (Ethernet), capture size 262144 bytes

	root@LipRouter:/sys/kernel/debug/usb#
	root@LipRouter:/sys/kernel/debug/usb#
	root@LipRouter:/sys/kernel/debug/usb# udhcpc -i usb5
	udhcpc: started, v1.31.1
	udhcpc: sending discover
	14:22:40.870590 IP 0.0.0.0.68 > 255.255.255.255.67: BOOTP/DHCP, Request from e0:c8:58:10:00:3a (oui Unknown), length 300
	14:22:40.873000 IP 10.0.0.1.67 > 255.255.255.255.68: BOOTP/DHCP, Reply, length 548
	udhcpc: sending select for 10.36.230.164
	14:22:40.950726 IP 0.0.0.0.68 > 255.255.255.255.67: BOOTP/DHCP, Request from e0:c8:58:10:00:3a (oui Unknown), length 300
	14:22:40.953225 IP 10.0.0.1.67 > 255.255.255.255.68: BOOTP/DHCP, Reply, length 548
	udhcpc: lease of 10.36.230.164 obtained, lease time 86400
	udhcpc: ifconfig usb5 10.36.230.164 netmask 255.0.0.0 broadcast +
	udhcpc: setting default routers: 10.0.0.1

### DNS 问题


	https://www.dazhuanlan.com/jiayeye/topics/1323551 
	

LIBC NSS GNU Libc (glibc) uses the Name Service Switch (NSS) to configure the behavior
of the C library for the local environment, and to configure how it reads
system data, such as passwords and group information. This is implemented
using an /etc/nsswitch.conf configuration file, and using one or more of the
/lib/libnss_* libraries. BusyBox tries to avoid using any libc calls that make
use of NSS. Some applets however, such as login and su, will use libc functions
that require NSS. If you enable CONFIG_USE_BB_PWD_GRP, BusyBox will use internal functions to
directly access the /etc/passwd, /etc/group, and /etc/shadow files without
using NSS. This may allow you to run your system without the need for
installing any of the NSS configuration files and libraries. When used with glibc, the BusyBox ‘networking’ applets will similarly require
that you install at least some of the glibc NSS stuff (in particular,
/etc/nsswitch.conf, /lib/libnss_dns, /lib/libnss_files, and /lib/libresolv*). Shameless Plug: As an alternative, one could use a C library such as uClibc. In
addition to making your system significantly smaller, uClibc does not require the
use of any NSS support files or libraries.

libnss设计就是动态库，不建议静态编译。要静态编译，请先重新编译glibc，把–enable-libnss-static放上
glibc需要libnss库支持，编译工具链中直接包含了相关的文件，是动态库
✔ ~/gcc-linaro-4.9.4-2017.01-i686_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib > ll libnss_dns*
 libnss_dns-2.19-2014.08-1-git.so
 libnss_dns.so.2 -> libnss_dns-2.19-2014.08-1-git.so
✔ ~/gcc-linaro-4.9.4-2017.01-i686_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib > ll libnss_files*
 libnss_files-2.19-2014.08-1-git.so
 libnss_files.so.2 -> libnss_files-2.19-2014.08-1-git.so
✔ ~/gcc-linaro-4.9.4-2017.01-i686_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib > ll libresolv*
 libresolv-2.19-2014.08-1-git.so
 libresolv.so.2 -> libresolv-2.19-2014.08-1-git.so
busybox需要动态编译来完成对libnss的使用
NSS(Name Service Switch 与 nsswitch.conf
Name Service Switch

NSS(Name Service Switch)是类unix操作系统中的一种工具，它为通用配置数据库和名称解析机制提供了各种来源。这些源文件包括本地操作系统文件 (例如/etc/passwd、/etc/group和/etc/hosts)、域名系统(DNS)、网络信息服务(NIS)和LDAP

nsswitch.conf(name service switch configuration，名称服务切换配置)文件位于/etc目录下，由它规定通过哪些途径以及按照什么顺序以及通过这些途径来查找特定类型的信息，还可以指定某个方法奏效或失效时系统将采取什么动作

> 解决方法

busybox使用动态编译，然后将libnss相关文件拷贝到rootfs/lib下，并配置文件/etc/hosts, /etc/resolv.conf, /etc/nsswitch.conf





