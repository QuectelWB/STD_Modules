IPv6 on OpenWrt
==========


OpenWrt是路由上用的一个Linux系统，架构如下:
- 路由器里WIFI网卡wlan0和有线网卡网卡，被加入到网桥br-lan里，这些网卡LAN
- 路由器通过我们模块的网卡wwan0访问internet，这个网卡叫WAN
<br>路由器里可以用命令ifup/ifdown/ifstatus LAN/WAN/WAN6 查看控制网络状态。
LAN，WAN，WAN6是openwrt里一种逻辑上网卡名字，比如路由器通过我们模块上网，实际网卡名字可能是usb0,wwan0,eth0.ppp0等等。我们可以把可以上IPV4网的是网卡的名字叫WAN网卡，可以把可以上IPV6网的是网卡的名字叫WAN6网卡.
<br>
> IPV4网络时，一般路由器里起个dnsmasq程序，充当DHCP server和DNS server.
<br>连到接到路由器的设备(比如PC，手机)，从DHCP server获取到192.168.1.X这些的IPV4地址，以及获取到的DNS一般为192.168.1.1.
<br>IPV4网络时，当PC要访问internet时候，首先源IP为192.168.1.X的数据被发送给路由器的LAN网卡， Linux系统把数据包转发(forward)给我们模块的网卡wwan0（根据系统当前的路由表设置查找网卡），我们模块再发送到internet.
只不过我们模块发送出去的时候，数据包的源IP会变成wwan0的IP地址，所以也叫NAT(network address translation)。
> IPV6网络时:
<br>前面讲了系统发送路由请求(RS)/公告(RA)，可以获取到前缀，然后生成wwan0网卡的IPV6地址。


quectel-CM
-----

quectel-CM -4 -6 &

	root@OpenWrt:/# [12-16_15:28:23:472] Quectel_QConnectManager_Linux_V1.6.0.16
	[12-16_15:28:23:474] network interface '' or qmidev '' is not exist
	[12-16_15:28:23:474] netcard driver = pcie_mhi, driver version = V1.3.0.13
	[12-16_15:28:23:475] qmap_mode = 1, qmap_version = 9, qmap_size = 16384, muxid = 0x81, qmap_netcard = rmnet_mhi0.1
	[12-16_15:28:23:475] Modem works in QMI mode
	[12-16_15:28:23:504] cdc_wdm_fd = 7
	[12-16_15:28:24:542] QmiThreadSendQMITimeout pthread_cond_timeout_np timeout
	[12-16_15:28:25:565] Get clientWDS = 15
	[12-16_15:28:25:570] Get clientWDS = 16
	[12-16_15:28:25:575] Get clientDMS = 1
	[12-16_15:28:25:580] Get clientNAS = 4
	[12-16_15:28:25:585] Get clientUIM = 1
	[12-16_15:28:25:589] Get clientWDA = 1
	[12-16_15:28:25:592] requestBaseBandVersion RM500QGLABR11A01M4G
	[12-16_15:28:25:596] qmap_settings.rx_urb_size = 16384
	[12-16_15:28:25:596] qmap_settings.ul_data_aggregation_max_datagrams  = 11
	[12-16_15:28:25:596] qmap_settings.ul_data_aggregation_max_size       = 8192
	[12-16_15:28:25:596] qmap_settings.dl_minimum_padding                 = 0
	[12-16_15:28:25:621] requestGetSIMStatus SIMStatus: SIM_READY
	[12-16_15:28:25:626] requestGetProfile[1] ///0
	[12-16_15:28:25:631] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE
	[12-16_15:28:25:635] requestQueryDataCall IPv4ConnectionStatus: DISCONNECTED
	[12-16_15:28:25:639] requestQueryDataCall IPv6ConnectionStatus: DISCONNECTED
	[12-16_15:28:25:639] ifconfig rmnet_mhi0 down
	[12-16_15:28:25:654] ifconfig rmnet_mhi0.1 0.0.0.0
	[12-16_15:28:25:661] ifconfig rmnet_mhi0.1 down
	[12-16_15:28:25:694] requestSetupDataCall WdsConnectionIPv4Handle: 0x4e5233a0
	[12-16_15:28:25:717] requestSetupDataCall WdsConnectionIPv6Handle: 0x4e29bb50
	[   59.206206] net rmnet_mhi0: link_state 0x0 -> 0x1
	[12-16_15:28:25:735] ifconfig rmnet_mhi0 up
	[   59.215856] [I][mhi_netdev_open] Opened net dev interface
	[12-16_15:28:25:746] ifconfig rmnet_mhi0.1 up
	[12-16_15:28:25:768] you are use OpenWrt?
	[12-16_15:28:25:768] should not calling udhcpc manually?
	[12-16_15:28:25:768] should modify /etc/config/network as below?
	[12-16_15:28:25:768] config interface wan
	[12-16_15:28:25:768]    option ifname   rmnet_mhi0.1
	[12-16_15:28:25:768]    option proto    dhcp
	[12-16_15:28:25:769] should use "/sbin/ifstaus wan" to check rmnet_mhi0.1 's status?
	[12-16_15:28:25:769] busybox udhcpc -f -n -q -t 5 -i rmnet_mhi0.1
	udhcpc: started, v1.28.4
	udhcpc: sending discover
	udhcpc: sending select for 10.38.132.27
	udhcpc: lease of 10.38.132.27 obtained, lease time 7200
	[12-16_15:28:26:169] udhcpc: ifconfig rmnet_mhi0.1 10.38.132.27 netmask 255.255.255.248 broadcast +
	[12-16_15:28:26:181] udhcpc: setting default routers: 10.38.132.28
	[12-16_15:28:26:238] ip -6 address add 2409:8930:462:44d4:b536:4031:e069:a685/64 dev rmnet_mhi0.1
	[12-16_15:28:26:258] ip -6 route add default via 2409:8930:462:44d4:34a6:a4ff:5ed9:1a74 dev rmnet_mhi0.1


root@OpenWrt:/# ip -6 route show

	2409:8930:462:44d4::/64 dev rmnet_mhi0.1  metric 256
	fe80::/64 dev br-lan  metric 256
	fe80::/64 dev wlan1  metric 256
	fe80::/64 dev wlan0  metric 256
	fe80::/64 dev rmnet_mhi0  metric 256
	fe80::/64 dev rmnet_mhi0.1  metric 256
	default via 2409:8930:462:44d4:34a6:a4ff:5ed9:1a74 dev rmnet_mhi0.1  metric 1024
	anycast 2409:8930:462:44d4:: dev rmnet_mhi0.1  metric 0
	anycast fe80:: dev br-lan  metric 0
	anycast fe80:: dev wlan1  metric 0
	anycast fe80:: dev wlan0  metric 0
	anycast fe80:: dev rmnet_mhi0  metric 0
	anycast fe80:: dev rmnet_mhi0.1  metric 0
	ff00::/8 dev br-lan  metric 256
	ff00::/8 dev wlan1  metric 256
	ff00::/8 dev wlan0  metric 256
	ff00::/8 dev rmnet_mhi0  metric 256
	ff00::/8 dev rmnet_mhi0.1  metric 256


淘宝和QQ有ipv6地址

	root@OpenWrt:~# ping -6 www.taobao.com
	PING www.taobao.com (2409:8c20:a12:104:2::3fa): 56 data bytes
	64 bytes from 2409:8c20:a12:104:2::3fa: seq=0 ttl=54 time=43.280 ms
	64 bytes from 2409:8c20:a12:104:2::3fa: seq=1 ttl=54 time=39.299 ms
	64 bytes from 2409:8c20:a12:104:2::3fa: seq=2 ttl=54 time=37.485 ms
	64 bytes from 2409:8c20:a12:104:2::3fa: seq=3 ttl=54 time=37.586 ms
	64 bytes from 2409:8c20:a12:104:2::3fa: seq=4 ttl=54 time=57.953 ms
	^C
	--- www.taobao.com ping statistics ---
	5 packets transmitted, 5 packets received, 0% packet loss
	round-trip min/avg/max = 37.485/43.120/57.953 ms

但是LAN下面的设备没法使用ipv6地址！

Odhcpd
-----

<br>
[https://openwrt.org/docs/techref/odhcpd](https://openwrt.org/docs/techref/odhcpd)
<br>
[odhcpd 源码](https://github.com/openwrt/odhcpd)
<br>
[在openwrt中使用odhcpd配置IPv6](https://www.asmodeus.cn/archives/296)
<br>

直接在OpenWrt上安装<br>

	opkg update
	opkg install odhcpd


odhcpd配置一般位于 /etc/config/dhcp 用uci 命令配置

	uci set dhcp.lan.ra='relay' 
	uci set dhcp.lan.dhcpv6='disabled' 
	uci set dhcp.lan.ndp='relay' 
	uci set dhcp.wan.ra='relay' 
	uci set dhcp.wan.dhcpv6='disabled' 
	uci set dhcp.wan.ndp='relay' 
	uci set dhcp.wan.ndproxy_routing='0' #不从NDP包探测 并配置路由信息 
	uci set dhcp.wan6=dhcp 
	uci set dhcp.wan6.interface='wan6' 
	uci set dhcp.wan6.ra='relay' 
	uci set dhcp.wan6.dhcpv6='disabled' 
	uci set dhcp.wan6.ndp='relay' 
	uci set dhcp.wan6.ndproxy_routing='0' #不从NDP包探测 并配置路由信息 
	uci set dhcp.wan6.master='1' 
	uci set dhcp.odhcpd=odhcpd 
	uci set dhcp.odhcpd.loglevel='7' #提供打印级别，调试用



> 开机启动

vi /etc/rc.local

在 exit 0 前添加以下内容

	/etc/init.d/odhcpd start
	/etc/init.d/odhcpd enable



odhcpd的配置
-----


<table class="inline table table-striped table-bordered table-condensed">
	<thead>
	<tr class="row0">
		<th class="col0 leftalign"> Name                   </th><th class="col1 leftalign"> Type      </th><th class="col2 leftalign"> Required   </th><th class="col3 leftalign"> Default                       </th><th class="col4 leftalign"> Description                                                                                                       </th>
	</tr>
	</thead>
	<tr class="row1">
		<td class="col0 leftalign"> <code>interface</code>          </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>&lt;name of UCI section&gt;</code>     </td><td class="col4 leftalign"> Logical OpenWrt interface.                                                                                        </td>
	</tr>
	<tr class="row2">
		<td class="col0 leftalign"> <code>ifname</code>             </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>&lt;resolved from logical&gt;</code>   </td><td class="col4 leftalign"> Physical network interface.                                                                                       </td>
	</tr>
	<tr class="row3">
		<td class="col0 leftalign"> <code>networkid</code>          </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>&lt;same as ifname&gt;</code>          </td><td class="col4 leftalign"> Alias of <code>ifname</code> for compatibility.                                                                            </td>
	</tr>
	<tr class="row4">
		<td class="col0 leftalign"> <code>ignore</code>             </td><td class="col1 leftalign"> boolean   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> do not serve this interface unless overridden by <code>ra</code>, <code>ndp</code>, <code>dhcpv4</code> or <code>dhcpv6</code> options.               </td>
	</tr>
	<tr class="row5">
		<td class="col0 leftalign"> <code>master</code>             </td><td class="col1 leftalign"> boolean   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> is a master interface for relaying                                                                                </td>
	</tr>
	<tr class="row6">
		<td class="col0 leftalign"> <code>ra</code>                 </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>disabled</code>                  </td><td class="col4 leftalign"> Router Advert service. Set to <code>disabled</code>, <code>server</code>, <code>relay</code> or <code>hybrid</code>.                                  </td>
	</tr>
	<tr class="row7">
		<td class="col0 leftalign"> <code>dhcpv6</code>             </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>disabled</code>                  </td><td class="col4 leftalign"> DHCPv6 service. Set to <code>disabled</code>, <code>server</code>, <code>relay</code> or <code>hybrid</code>.                                         </td>
	</tr>
	<tr class="row8">
		<td class="col0 leftalign"> <code>dhcpv4</code>             </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>disabled</code>                  </td><td class="col4 leftalign"> DHCPv4 service. Set to <code>disabled</code> or <code>server</code>.                                                                </td>
	</tr>
	<tr class="row9">
		<td class="col0 leftalign"> <code>ndp</code>                </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>disabled</code>                  </td><td class="col4 leftalign"> Neighbor Discovery Proxy. Set to <code>disabled</code>, <code>relay</code> or <code>hybrid</code>                                            </td>
	</tr>
	<tr class="row10">
		<td class="col0 leftalign"> <code>dynamicdhcp</code>        </td><td class="col1 leftalign"> boolean   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>1</code>                         </td><td class="col4 leftalign"> Leases for DHCPv4 and DHCPv6 are created dynamically.                                                             </td>
	</tr>
	<tr class="row11">
		<td class="col0 leftalign"> <code>dns</code>                </td><td class="col1 leftalign"> list      </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>&lt;local address&gt;</code>           </td><td class="col4 leftalign"> <abbr title="Domain Name System">DNS</abbr> servers to announce on the network. <abbr title="Internet Protocol version 4">IPv4</abbr> and <abbr title="Internet Protocol version 6">IPv6</abbr> addresses are accepted.                                     </td>
	</tr>
	<tr class="row12">
		<td class="col0 leftalign"> <code>domain</code>             </td><td class="col1 leftalign"> list      </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>&lt;local search domain&gt;</code>     </td><td class="col4 leftalign"> Search domains to announce on the network.                                                                        </td>
	</tr>
	<tr class="row13">
		<td class="col0 leftalign"> <code>leasetime</code>          </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>12h</code>                       </td><td class="col4 leftalign"> DHCPv4 address leasetime                                                                                          </td>
	</tr>
	<tr class="row14">
		<td class="col0 leftalign"> <code>start</code>              </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>100</code>                       </td><td class="col4 leftalign"> Starting address of the DHCPv4 pool.                                                                              </td>
	</tr>
	<tr class="row15">
		<td class="col0 leftalign"> <code>limit</code>              </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>150</code>                       </td><td class="col4 leftalign"> Number of addresses in the DHCPv4 pool.                                                                           </td>
	</tr>
	<tr class="row16">
		<td class="col0 leftalign"> <code>ra_default</code>         </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> Override default route- Set to  <code>0</code> (default), <code>1</code> (ignore, no public address) or <code>2</code> (ignore all).         </td>
	</tr>
	<tr class="row17">
		<td class="col0 leftalign"> <code>ra_management</code>      </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>1</code>                         </td><td class="col4 leftalign"> RA management mode. See the RA Management Modes table below.                                                    </td>
	</tr>
	<tr class="row18">
		<td class="col0 leftalign"> <code>ra_offlink</code>         </td><td class="col1 leftalign"> boolean   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> Announce prefixes off-link                                                                                        </td>
	</tr>
	<tr class="row19">
		<td class="col0 leftalign"> <code>ra_preference</code>      </td><td class="col1 leftalign"> string    </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>medium</code>                    </td><td class="col4 leftalign"> Route preference <code>medium</code>, <code>high</code> or <code>low</code>                                                                  </td>
	</tr>
	<tr class="row20">
		<td class="col0 leftalign"> <code>ra_maxinterval</code>     </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>600</code>                       </td><td class="col4 leftalign"> Maximum time allowed between sending unsolicited Router Advertisements (RA).                                      </td>
	</tr>
	<tr class="row21">
		<td class="col0 leftalign"> <code>ra_mininterval</code>     </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>200</code>                       </td><td class="col4 leftalign"> Minimum time allowed between sending unsolicited Router Advertisements (RA).                                      </td>
	</tr>
	<tr class="row22">
		<td class="col0 leftalign"> <code>ra_lifetime</code>        </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>1800</code>                      </td><td class="col4 leftalign"> Router Lifetime published in Router Advertisement (RA) messages.                                                  </td>
	</tr>
	<tr class="row23">
		<td class="col0 leftalign"> <code>ra_useleasetime</code>    </td><td class="col1 leftalign"> boolean   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> If set, the configured DHCPv4 <code>leasetime</code> is also used as limit and preferred lifetime of the <abbr title="Internet Protocol version 6">IPv6</abbr> prefix.      </td>
	</tr>
	<tr class="row24">
		<td class="col0 leftalign"> <code>ra_reachabletime</code>   </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> Reachable Time in milliseconds to be published in Router Advertisement (RA) messages&#039;.                            </td>
	</tr>
	<tr class="row25">
		<td class="col0 leftalign"> <code>ra_retranstime</code>     </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> Retransmit Time in milliseconds to be published in Router Advertisment (RA) messages.                             </td>
	</tr>
	<tr class="row26">
		<td class="col0 leftalign"> <code>ra_hoplimit</code>        </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> The maximum hops to be published in Router Advertisement (RA) messages.                                           </td>
	</tr>
	<tr class="row27">
		<td class="col0 leftalign"> <code>ra_mtu</code>             </td><td class="col1 leftalign"> integer   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> The <abbr title="Maximum Transmission Unit">MTU</abbr> to be published in Router Advertisement (RA) messages.                                                    </td>
	</tr>
	<tr class="row28">
		<td class="col0 leftalign"> <code>ndproxy_routing</code>    </td><td class="col1 leftalign"> boolean   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>1</code>                         </td><td class="col4 leftalign"> Learn routes from ND.P                                                                                            </td>
	</tr>
	<tr class="row29">
		<td class="col0 leftalign"> <code>ndproxy_slave</code>      </td><td class="col1 leftalign"> boolean   </td><td class="col2 leftalign">            </td><td class="col3 leftalign"> <code>0</code>                         </td><td class="col4 leftalign"> NDProxy external slave.                                                                                           </td>
	</tr>
	<tr class="row30">
		<td class="col0 leftalign"> <code>ndproxy_static</code>     </td><td class="col1 leftalign"> list      </td><td class="col2 leftalign">            </td><td class="col3 leftalign">                               </td><td class="col4 leftalign"> Static NDProxy prefixes.                                                                                          </td>
	</tr>
</table>




