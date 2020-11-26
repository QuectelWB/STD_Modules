Android Loopback 网速测试
========

准备工作
-----

- 模组写loopback功能
- Android RIL库打开loopback
- 工具iperf2 mpstat等


### 模组写loopback

如果模组可以取下来连到Windows PC上，直接用QPST写即可。按照 X55_IPQ8074_rmnet网卡_loopback功能使用说明_20200707_V4.pdf

如果模块已经和AP做成PCB，无法让模块接到PC上，可以用网络转发方式，需要在AP上运行QLog，QLog内嵌tty2tcp，通过QLog -s 9000，将DM port转发到AP的9000端口上。和AP处于同一局域网的一台Windows PC上运行QPST，将loopback_config.txt 写入模组。X55_IPQ8074_rmnet网卡_loopback功能使用说明_20200707_V4.pdf 中有详细介绍。

QPST工具及其使用咨询QuectelFAE。

### Android Loopback

ril库在启动过程中会调用ql-ril.conf. ql-ril.conf 的位置一般位于 /system/etc/或者/vendor/etc/, 或者根目录下的etc/（根目录下的etc/是软连接）。或者通过rillog确定ql-ril.conf的位置。

ql-ril.conf 中有一行

	support_X55_loopback=14

默认情况该行被注释掉了，打开该行。

如果以上两步完成，无意外情况，插上SIM卡（RIL拨号需要，loopback测试不会使用SIM卡流量，不需要接模组天线），Android上rmnet_usb0.1就可以获得IP

	
	rmnet_usb0.1 Link encap:Ethernet  HWaddr 16:8d:31:d6:ce:06
	          inet addr:192.168.48.171  Mask:255.255.255.248
	          inet6 addr: 8877:6655:4433:2211:d4c2:8ba3:dd8c:295a/64 Scope: Global
	          inet6 addr: fe80::148d:31ff:fed6:ce06/64 Scope: Link
	          inet6 addr: 8877:6655:4433:2211:148d:31ff:fed6:ce06/64 Scope: Global
	          UP RUNNING NOARP  MTU:1500  Metric:1
	          RX packets:1681 errors:0 dropped:0 overruns:0 frame:0
	          TX packets:121 errors:0 dropped:0 overruns:0 carrier:0
	          collisions:0 txqueuelen:1000
	          RX bytes:69544 TX bytes:5984
	
IP 地址一般是192.168.48.17X，模组的地址是192.168.48.172

Android系统性能优化
----

增大网卡待处理 skb 队列数量大小，防止网卡驱动短时间内收到 skb 过多，系统来不及处理. 

/proc/sys/net/core/netdev_max_backlog 默认1000，调整成2000

对8核cpu，将rmnet_usb0.1 分配在cpu7 上

echo 80 > /sys/class/net/rmnet_usb0.1/queues/rx-0/rps_cpus

将rmnet_usb0.1 分配在cpu6

echo 40 > /sys/class/net/rmnet_usb0/queues/rx-0/rps_cpus

cpu设置全部online，配置在最高性能

echo 1 > /sys/bus/cpu/devices/cpu7/online
echo 1 > /sys/bus/cpu/devices/cpu6/online
echo 1 > /sys/bus/cpu/devices/cpu5/online
echo 1 > /sys/bus/cpu/devices/cpu4/online
echo 1 > /sys/bus/cpu/devices/cpu3/online
echo 1 > /sys/bus/cpu/devices/cpu2/online
echo 1 > /sys/bus/cpu/devices/cpu1/online

echo performance >  /sys/bus/cpu/devices/cpu7/cpufreq/scaling_governor
echo performance >  /sys/bus/cpu/devices/cpu6/cpufreq/scaling_governor
echo performance >  /sys/bus/cpu/devices/cpu5/cpufreq/scaling_governor
echo performance >  /sys/bus/cpu/devices/cpu4/cpufreq/scaling_governor
echo performance >  /sys/bus/cpu/devices/cpu3/cpufreq/scaling_governor
echo performance >  /sys/bus/cpu/devices/cpu2/cpufreq/scaling_governor
echo performance >  /sys/bus/cpu/devices/cpu1/cpufreq/scaling_governor

写成[shell脚本](linuxloopback.sh)


iperf2 回环测试
-----

	/data/iperf -u -c 192.168.48.172 -b 38M -t 666 

意思是
	
	-u UDP测速
	-c 192.168.48.172 客户端模式，192.168.48.172 是模块的IP
	-b UDP 测速的时候，表示灌包的带宽。 比如 loopback 测试的时候，我们用 1G 的网卡来测试，则最好灌包 66M，这样回环的数据就是 14*66M
	-t 666 测试666秒

另起一个UDP测试

	/data/iperf  -u -s -i 1
	
其中
	-s Server
	-i 时间间隔。UDP 测速，需要看 Server 显示的速率以及丢包率，丢包率高说明 Client 灌包的带宽太大。

启动以后

可以用killall iperf kill掉上面两个iperf进程









)

