模组的网络服务
=====

一般来说，实现LTE/5G拨号成功后，同时给连接到客户主控上的WIFI、Ethernet网卡的其他设备提供网络，通常方法包括两种

- NAT
- Bridge

NAT
-----

the Quectel network card as the external network interface, to meet the network connection needs of the whole system.

There is an optional methods


![](NAT.png)

Same like

![](windows_internel_share.jpg)

Bridge
----

to be done

同 tap/tun、veth-pair 一样，Bridge 也是一种虚拟网络设备，所以具备虚拟网络设备的所有特性，比如可以配置 IP、MAC 等。
除此之外，Bridge 还是一个交换机，具有交换机所有的功能。




