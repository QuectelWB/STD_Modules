IPV6
=====

[ipv6 ](https://www.cnblogs.com/fzxiaomange/p/ipv6-autoconf-stateless-stateful.html)

地址组成
---------

$\color{#FF3030}{IPV6地址组成}$

IPV6 地址由2部分组成

	前64位通过某种方式获取
	后64位，算法计算获得

1个4G模组的IP地址

	root@ZhuoTK:/# ifconfig wwan0
	wwan0 Link encap:Ethernet HWaddr 2E:EC:C4:9D:7C:82 
	inet addr:10.39.164.188 Mask:255.255.255.248
	inet6 addr: 2409:8930:421:f2b:cc61:967c:945:fccf/64 Scope:Global	 
	inet6 addr: fe80::2cec:c4ff:fe9d:7c82/64 Scope:Link
	UP RUNNING NOARP MTU:1500 Metric:1

<font color=Blue>net6 addr: fe80:: 2cec: c4ff: fe9d: 7c82/64 Scope: Link  </font>
这个地址是本地地址Link-Local Address.<br>
网卡创建的时候，就会有一个对应的本地IPv6地址

生成方法

- 手动配置
- 自动配置

	eui64：根据mac地址换算而来
	stable_secret：跟随网络环境的变化而变化，处于固定网络环境时其值将固定
	random：随机生成

这里的 inet6 addr: <font color=red>fe80 :: 2cec: c4ff: fe9d: 7c82/64 Scope: Link </font>
就是根据MAC地址计算得到的
<br>固定是fe80::(fe80:0000:0000:0000）,后64位根据MAC地址( 2E: EC: C4: 9D: 7C: 82 )生成的。


而 inet6 addr: 2409: 8930: 421: f2b: cc61: 967c: 945: fccf/64 Scope:Global 则是一个全局(Global)地址。

全球地址
-------

全球地址的生成方法

- **无状态**

	根据路由通告报文RA（Router Advertisement）包含的prefix前缀信息自动配置IPv6地址，组成方式是Prefix + (EUI64 or 随机)。Stateless也可以称为SLAAC（Stateless address auto configuration）。RA获取到是Prefix，不能完整的128位IPV6地址。
	
- **有状态**, 通过DHCPv6方式获得IPv6地址

	有状态DHCPv6（Stateful DHCPv6）：IPv6地址、其他参数（如DNS）均通过DHCPv6获取
	无状态DHCPv6（Stateless DHCPv6）：IPv6地址依然通过路由通告RA方式生成，其他参数（如DNS）通过DHCPv6获取
	

有状态、无状态仅针对于IPv6地址分配方式，并不包含其他参数（如DNS）



Quectel
------


- QMI/MBIM方式拨号，可以直接通过QMI/MBIM消息获取到IPV6地址和DNS信息的，不需要的路由请求(RS)和公告(RA)，以及DHCPv6
- PPP拨号的话，可以通过PPP拿到IPV6地址，但通过PPP拿不到DNS信息
- 模块支持RS/RA

	ifconfig net up，Linux Kernel自动发送路由请求(RS)，收到路由公告地RA后，根据RA里面的前缀Prefix，生成Global的IPV6地址。
	Linux内核无法处理RA里面的DNS信息(需要上层应用写到DNS配置文件里，但不同的Linux系统配置方法多不一样)。
	PPP拨号的时候，可以自己写个小程序，发送RS给模块，然后从RA中提起DNS信息。

- 模块不支持DHCPv6


NDP（Neighbor Discovery Protocol）
------


IPV6 域名的解析
-------


Quectel ppp for ipv6
-----

man pppd

	ipv6cp-accept-local
              With this option, pppd will accept the peer's idea of our local IPv6 interface identifier, even if  the
              local IPv6 interface identifier was specified in an option.

       ipv6cp-accept-remote
              With  this  option, pppd will accept the peer's idea of its (remote) IPv6 interface identifier, even if
              the remote IPv6 interface identifier was specified in an option.

       ipv6cp-max-configure n
              Set the maximum number of IPv6CP configure-request transmissions to n (default 10).

       ipv6cp-max-failure n
              Set the maximum number of IPv6CP configure-NAKs returned before starting to send configure-Rejects  in
              stead to n (default 10).

       ipv6cp-max-terminate n
              Set the maximum number of IPv6CP terminate-request transmissions to n (default 3).

       ipv6cp-restart n
              Set the IPv6CP restart interval (retransmission timeout) to n seconds (default 3).

       ipx    Enable  the  IPXCP and IPX protocols.  This option is presently only supported under Linux, and only if
              your kernel has been configured to include IPX support.

	+ipv6  Enable the IPv6CP and IPv6 protocols.


脚本中的AT+CGDCONT配置pdp支持ipv4v6

通过Quectel提供的ppp拨号脚本，获得ipv6 addr、gateway、DNS，需要使用无状态的自动配置和有状态的配置。


无状态的自动配置自动完成，获得ipv6地址。但是这个地址无法进行DNS解析，无法访问域名

通过dibbler-client可以配置DNS信息

	apt-get install dibbler-client  -y


dibbler-client 的常用命令： 
- dibbler-client  run      在终端运行 dibbler-client，使用 ctrl+c 停止运行 
- dibbler-client  satrt     以守护进程的形式运行 dibbler-client 
- dibbler-client  stop     结束 dibbler-client，和 start 配对使用 
- dibbler-client  status    查看 dibbler-client 的状态

run或者start之前，修改配置文件

/etc/dibbler/client.conf

	#client.conf
	iface ppp0
	{
		stateless
		option dns-server
	}


