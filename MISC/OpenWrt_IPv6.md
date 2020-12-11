penwrt是路由上用的一个Linux系统，架构如下:
路由器里WIFI网卡wlan0和有线网卡网卡，被加入到网桥br-lan里，这些网卡LAN
路由器通过我们模块的网卡wwan0访问internet，这个网卡叫WAN
路由器里可以用命令ifup/ifdown/ifstatus LAN/WAN/WAN6 查看控制网络状态。
LAN，WAN，WAN6是openwrt里一种逻辑上网卡名字，比如路由器通过我们模块上网，
实际网卡名字可能是usb0,wwan0,eth0.ppp0等等。我们可以把可以上IPV4网的是网卡的名字叫WAN网卡，可以把可以上IPV6网的是网卡的名字叫WAN6
网卡.
IPV4网络时，一般路由器里起个dnsmasq程序，充当DHCP server和DNS server.
连到接到路由器的设备(比如PC，手机)，从DHCP server获取到192.168.1.X这些的IPV4地址，以及获取到的DNS一般为192.168.1.1.
IPV4网络时，当PC要访问internet时候，首先源IP为192.168.1.X的数据被发送给路由器的LAN网卡， Linux系统把数据包转发(forward)给我们模块
的网卡wwan0（根据系统当前的路由表设置查找网卡），我们模块再发送到internet.
只不过我们模块发送出去的时候，数据包的源IP会变成wwan0的IP地址，所以也叫NAT(network address translation)。
IPV6网络时:
前面讲了系统发送路由请求(RS)/公告(RA)，可以获取到前缀，然后生成wwan0网卡的IPV6地址。



