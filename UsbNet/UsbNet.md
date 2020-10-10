----------
移远模组可以通过AT指令配置网卡类型

<font color=0x00FFFF>
AT+QCFG=”usbnet”
</font>


<!-- Row Highlight Javascript -->
<script type="text/javascript">
	window.onload=function(){
	var tfrow = document.getElementById('tfhover').rows.length;
	var tbRow=[];
	for (var i=1;i<tfrow;i++) {
		tbRow[i]=document.getElementById('tfhover').rows[i];
		tbRow[i].onmouseover = function(){
		  this.style.backgroundColor = '#f3f8aa';
		};
		tbRow[i].onmouseout = function() {
		  this.style.backgroundColor = '#ffffff';
		};
	}
};
</script>

<style type="text/css">
table.tftable {font-size:12px;color:#333333;width:100%;border-width: 1px;border-color: #729ea5;border-collapse: collapse;}
table.tftable th {font-size:12px;background-color:#acc8cc;border-width: 1px;padding: 8px;border-style: solid;border-color: #729ea5;text-align:left;}
table.tftable tr {background-color:#ffffff;}
table.tftable td {font-size:12px;border-width: 1px;padding: 8px;border-style: solid;border-color: #729ea5;}
</style>

<table id="tfhover" class="tftable" border="1">
<tr><th>模块网卡类型</th><th>Linux 驱动</th><th>拨号方式</th><th>物理层数据</th></tr>
<tr><td>rmnet/qmi/ndis
at+qcfg=”usbnet”,0
</td><td>qmi_wwan&cdc_wdm
CONFIG_USB_NET_QMI_WWAN
 添加移远模块的vid和pid
内核版本 >= 3.4
；
GobiNet
 移远提供源码

</td><td>libqmi(Ubuntu)
uqmi(openWRT)
quectel-CM
AT$QCRMCALL=1,1
Autoconnect(不推荐, 不适用于5G)
</td><td>IP帧</td></tr>
<tr><td>ecm
a+qcfg=”usbnet”,1
车载方案OpenLinux
</td><td>CONFIG_USB_NET_CDCETHER</td><td>autoconnect</td><td>以太网帧</td></tr>
<tr><td>mbim
at+qcfg=”usbnet”,2
Win10平板EM/EP系列
</td><td>CONFIG_USB_NET_CDC_MBIM
内核版本 >= 3.18
</td><td>mbim协议拨号
libmbim(Ubuntu)
，quectel-CM也支持</td><td>MBIM帧</td></tr>
<tr><td>rndis
at+qcfg=”usbnet”,3
</td><td>CONFIG_USB_NET_RNDIS_HOST</td><td>autoconnect</td><td>以太网帧</td></tr>
</table>

其他如NCM和ECM、RNDIS类似。
NCM内核开启

----------

# RMNET #
### QMI ###
内核自带的
[qmi_wwan.c](https://elixir.bootlin.com/linux/v4.14.181/source/drivers/net/usb/qmi_wwan.c)
可以用于LTE模组。
但是该驱动不支持Quectel IP聚合和IP复用功能，并且不支持5G模组（5G模组强制打开IP聚合）


### GobiNet ###
如果要用AT指令拨号，将驱动中的qcrmcall_mode 设置成1. （5G模组暂不支持AT指令拨号）


<table><tr><td bgcolor=yellow>请向Quectel索要最新版本的QMI驱动和GobiNet驱动 </td></tr></table>

	Mailto:support@quectel.com


##拨号方法


### quectel-CM


After the module is connected and the GobiNet or QMI_WWAN driver is installed successfully, a USB network adapter and a QMI channel will be created. The USB network adapter of the GobiNet driver is named as ethX (or usbX if the kernel version is 2.6.39 or lower), and the QMI channel is /dev/qcqmiX. The USB network adapter of the QMI_WWAN driver is named as wwanX, and the QMI channel is /dev/cdc-wdmX.

quectel-CM will send QMI message to the module via QMI channel to set up data connection. Please refer to the following table about the usage of quectel-CM: 


	root@cqh6:~# ./quectel-CM/quectel-CM & 
	[07-03_06:56:28:172] WCDMA&LTE_QConnectManager_Linux&Android_V1.3.4 
	[07-03_06:56:28:172] ./quectel-CM/quectel-CM profile[1] = (null)/(null)/(null)/0, pincode = (null) 
	[07-03_06:56:28:174] Find /sys/bus/usb/devices/2-1.2 idVendor=2c7c idProduct=0512 
	[07-03_06:56:28:174] Find /sys/bus/usb/devices/2-1.2:1.4/net/wwan0 
	[07-03_06:56:28:174] Find usbnet_adapter = wwan0 
	[07-03_06:56:28:175] Find /sys/bus/usb/devices/2-1.2:1.4/usbmisc/cdc-wdm0 
	[07-03_06:56:28:175] Find qmichannel = /dev/cdc-wdm0 
	[07-03_06:56:28:197] cdc_wdm_fd = 7 
	[07-03_06:56:28:381] Get clientWDS = 18 
	[07-03_06:56:28:445] Get clientDMS = 1 
	[07-03_06:56:28:509] Get clientNAS = 2 
	[07-03_06:56:28:573] Get clientUIM = 2 
	[07-03_06:56:28:637] Get clientWDA = 1 
	[07-03_06:56:28:701] requestBaseBandVersion EM12GPAR01A06M4G
	[07-03_06:56:28:957] requestGetSIMStatus SIMStatus: SIM_READY 
	[07-03_06:56:29:021] requestGetProfile[1] cmnet///0 
	[07-03_06:56:29:085] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[07-03_06:56:29:149] requestQueryDataCall IPv4ConnectionStatus: DISCONNECTED 
	[07-03_06:56:29:277] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[07-03_06:56:29:341] requestSetupDataCall WdsConnectionIPv4Handle: 0x127b42c0 
	[07-03_06:56:29:469] requestQueryDataCall IPv4ConnectionStatus: CONNECTED 
	[07-03_06:56:29:533] ifconfig wwan0 up 
	[07-03_06:56:29:543] busybox udhcpc -f -n -q -t 5 -i wwan0 
	udhcpc: started, v1.27.2 
	udhcpc: sending discover 
	udhcpc: sending select for 10.170.235.201 
	udhcpc: lease of 10.170.235.201 obtained, lease time 7200 
	[07-03_06:56:29:924] /etc/udhcpc/default.script: Resetting default routes 
	[07-03_06:56:29:936] /etc/udhcpc/default.script: Adding DNS 211.138.180.2
	[07-03_06:56:29:936] /etc/udhcpc/default.script: Adding DNS 211.138.180.3

#### 什么是IP聚合、IP复用 ####

Quectel网卡支持IP聚合和IP复用。
IP聚合（IP aggregation protocol）指的是一次传输urb中携带多个IP数据包，相比于传统的QMI和GobiNet驱动一个urb只能传输一个IP数据包，IP聚合减少CPU中断数量和频率，降低了CPU负载。IP聚合功能对应网卡驱动qmap_mode为1或者大于1的情况。
IP复用（IP Multiplexing Protocol）指是通过USB Bus传输qmap数据包，支持多路PDN拨号的情况，对应qmap_mode 大于1。
usb网卡驱动默认没有打开IP聚合功能和使用qmap功能。推荐打开IP聚合，将qmi_wwan_q.c中的qmap_mode = 0 改成qmap_mode = 1。
多路拨号

	[244838.067277] usbcore: registered new interface driver qmi_wwan_q 
	[244844.970420] usb 2-1.2: new high-speed USB device number 22 using sunxi-ehci 
	[244845.090167] usb 2-1.2: New USB device found, idVendor=2c7c, idProduct=0435 
	[244845.090193] usb 2-1.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0 
	[244845.090212] usb 2-1.2: Product: LTE Module 
	[244845.090229] usb 2-1.2: Manufacturer: Quectel, Incorporated 
	[244845.129638] qmi_wwan_q 2-1.2:1.4: cdc-wdm0: USB WDM device 
	[244845.129666] qmi_wwan_q 2-1.2:1.4: Quectel modules work on RawIP mode 
	[244845.135800] qmi_wwan_q 2-1.2:1.4 wwan0: register 'qmi_wwan_q' at usb-sunxi-ehci-1.2, WWAN/QMI 
	device, 2e:75:af:df:48:50 
	[244845.140199] net wwan0: qmap_register_device wwan0.1 
	[244845.142132] net wwan0: qmap_register_device wwan0.2 
	[244845.143211] net wwan0: qmap_register_device wwan0.3 
	[244845.144309] net wwan0: qmap_register_device wwan0.4 
	[244845.144329] qmi_wwan_q 2-1.2:1.4: rx_urb_size = 4096 

拨号先运行quectel-qmi-proxy

	root@cqh6:~# ./quectel-qmi-proxy -d /dev/cdc-wdm0 & 
	Will use cdc-wdm /dev/cdc-wdm0 
	qmi_proxy_init enter 
	qmi_proxy_loop enter thread_id 548403548656 
	link_prot 2 
	ul_data_aggregation_protocol 5 
	dl_data_aggregation_protocol 5 
	dl_data_aggregation_max_datagrams 10 
	dl_data_aggregation_max_size 4096 
	ul_data_aggregation_max_datagrams 16 
	ul_data_aggregation_max_size 4096 
	qmi_proxy_init finished, rx_urb_size is 4096 
	local server: quectel-qmi-proxy sockfd = 4 
	qmi_start_server: qmi_proxy_server_fd = 4 

再用quectel-CM 发起数据连接

	# ./quectel-CM -n 2 & 
	[06-04_05:13:16:473] WCDMA&LTE_QConnectManager_Linux&Android_V1.3.3 
	[06-04_05:13:16:473] ./quectel-CM profile[2] = (null)/(null)/(null)/0, pincode = (null) 
	[06-04_05:13:16:475] Find /sys/bus/usb/devices/2-1.2 idVendor=2c7c idProduct=0435 
	[06-04_05:13:16:476] Find /sys/bus/usb/devices/2-1.2:1.4/net/wwan0 
	[06-04_05:13:16:476] Find usbnet_adapter = wwan0 
	[06-04_05:13:16:476] Find /sys/bus/usb/devices/2-1.2:1.4/usbmisc/cdc-wdm0 
	[06-04_05:13:16:476] Find qmichannel = /dev/cdc-wdm0 
	[06-04_05:13:16:477] qmap_mode = 4, muxid = 0x82, qmap_netcard = wwan0.2 
	[06-04_05:13:16:477] connect to quectel-qmi-proxy sockfd = 7 
	[06-04_05:13:16:478] cdc_wdm_fd = 7 
	[06-04_05:13:16:535] Get clientWDS = 17 
	[06-04_05:13:16:567] Get clientDMS = 1 
	[06-04_05:13:16:599] Get clientNAS = 3 
	[06-04_05:13:16:631] Get clientUIM = 1 
	[06-04_05:13:16:663] requestBaseBandVersion AG35CEVAR05A06T4G 
	[06-04_05:13:16:791] requestGetSIMStatus SIMStatus: SIM_READY 
	[06-04_05:13:16:823] requestGetProfile[2] ///0 
	[06-04_05:13:16:855] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[06-04_05:13:16:887] requestQueryDataCall IPv4ConnectionStatus: DISCONNECTED 
	[06-04_05:13:16:950] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[06-04_05:13:16:983] requestSetupDataCall WdsConnectionIPv4Handle: 0x86e513b0 
	[06-04_05:13:17:047] requestQueryDataCall IPv4ConnectionStatus: CONNECTED 
	[06-04_05:13:17:079] ifconfig wwan0 up 
	[06-04_05:13:17:089] ifconfig wwan0.2 up 
	[06-04_05:13:17:100] busybox udhcpc -f -n -q -t 5 -i wwan0.2 
	udhcpc: started, v1.27.2 
	udhcpc: sending discover 
	udhcpc: sending select for 10.247.97.242 
	udhcpc: lease of 10.247.97.242 obtained, lease time 7200 
	[06-04_05:13:17:449] /etc/udhcpc/default.script: Resetting default routes 
	[06-04_05:13:17:465] /etc/udhcpc/default.script: Adding DNS 211.138.180.2 
	[06-04_05:13:17:465] /etc/udhcpc/default.script: Adding DNS 211.138.180.3 


GobiNet网卡同理
	/*
	Test QMAP on GobiNet or QMI WWAN 
    0 - no QMAP 
    1 - QMAP (Aggregation protocol) 
    X - QMAP (Multiplexing and Aggregation protocol) 
	*/ 
	static uint __read_mostly qmap_mode = 4; 
	module_param( qmap_mode, uint, S_IRUGO | S_IWUSR );

GobiNet网卡生成的多路网卡名字usb0.X

	usb0.1 for data call setup on PDN-1 
	usb0.2 for data call setup on PDN-2 
	usb0.3 for data call setup on PDN-3 
	usb0.4 for data call setup on PDN-4 

the dmesg log

	[247365.213865] GobiNet: Quectel_WCDMA&LTE_Linux&Android_GobiNet_Driver_V1.5.0 
	[247365.214209] usbcore: registered new interface driver GobiNet 
	[247369.390423] usb 2-1.2: new high-speed USB device number 25 using sunxi-ehci 
	[247369.510166] usb 2-1.2: New USB device found, idVendor=2c7c, idProduct=0435 
	[247369.510193] usb 2-1.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0 
	[247369.510212] usb 2-1.2: Product: LTE Module 
	[247369.510229] usb 2-1.2: Manufacturer: Quectel, Incorporated 
	[247369.560313] GobiNet 2-1.2:1.4 usb0: register 'GobiNet' at usb-sunxi-ehci-1.2, GobiNet Ethernet Device, 
	2e:75:af:df:48:50 
	[247369.561690] creating qcqmi0 
	[247369.563154] GobiNet::qmap_register_device usb0.1 
	[247369.564861] GobiNet::qmap_register_device usb0.2 
	[247369.566014] GobiNet::qmap_register_device usb0.3 
	[247369.567818] GobiNet::qmap_register_device usb0.4 
	[247379.851678] GobiNet::QMIWDASetDataFormat qmap settings qmap_enabled=5, rx_size=4096, tx_size=4096

执行quectel-CM

	# ./quectel-CM -n 2 & 
	 [06-04_05:52:13:869] WCDMA&LTE_QConnectManager_Linux&Android_V1.3.3 
	[06-04_05:52:13:869] ./quectel-CM profile[2] = (null)/(null)/(null)/0, pincode = (null) 
	[06-04_05:52:13:870] Find /sys/bus/usb/devices/2-1.2 idVendor=2c7c idProduct=0435 
	[06-04_05:52:13:870] Find /sys/bus/usb/devices/2-1.2:1.4/net/usb0 
	[06-04_05:52:13:870] Find usbnet_adapter = usb0 
	[06-04_05:52:13:870] Find /sys/bus/usb/devices/2-1.2:1.4/GobiQMI/qcqmi0 
	[06-04_05:52:13:870] Find qmichannel = /dev/qcqmi0 
	[06-04_05:52:13:870] qmap_mode = 4, muxid = 0x82, qmap_netcard = usb0.2 
	[06-04_05:52:13:882] Get clientWDS = 7 
	[06-04_05:52:13:914] Get clientDMS = 8 
	[06-04_05:52:13:946] Get clientNAS = 9 
	[06-04_05:52:13:978] Get clientUIM = 10 
	[06-04_05:52:14:010] requestBaseBandVersion AG35CEVAR05A06T4G 
	[06-04_05:52:14:138] requestGetSIMStatus SIMStatus: SIM_READY 
	[06-04_05:52:14:170] requestGetProfile[2] ///0 
	[06-04_05:52:14:202] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[06-04_05:52:14:234] requestQueryDataCall IPv4ConnectionStatus: DISCONNECTED 
	[06-04_05:52:14:298] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[06-04_05:52:15:002] requestSetupDataCall WdsConnectionIPv4Handle: 0x86e4e3e0 
	[06-04_05:52:15:066] requestQueryDataCall IPv4ConnectionStatus: CONNECTED 
	[06-04_05:52:15:098] ifconfig usb0 up 
	[06-04_05:52:15:118] ifconfig usb0.2 up 
	[06-04_05:52:15:130] busybox udhcpc -f -n -q -t 5 -i usb0.2 
	udhcpc: started, v1.27.2 
	udhcpc: sending discover 
	udhcpc: sending select for 10.220.124.20 
	udhcpc: lease of 10.220.124.20 obtained, lease time 7200 
	[06-04_05:52:15:468] /etc/udhcpc/default.script: Resetting default routes 
	[06-04_05:52:15:480] /etc/udhcpc/default.script: Adding DNS 211.138.180.2 
	[06-04_05:52:15:480] /etc/udhcpc/default.script: Adding DNS 211.138.180.3

### 开源qmi工具

模组支持，quectel提供有限度技术支持

----------

# ECM #

CDC ECM 驱动是模块适配标准的 ECM 通用驱动，无需额外的代码修改，直接配置编译项即可。 
CDC ECM 驱动的相关配置项：
	
	CONFIG_USB_USBNET=y 
	CONFIG_NETDEVICES=y 
	CONFIG_USB_NET_CDCETHER=y

kernel\drivers\net\usb\Makefile

	obj-$(CONFIG_USB_NET_CDCETHER)	+= cdc_ether.o

----------

# MBIM #
CDC MBIM 驱动是华为模块适配标准的通用驱动，无需额外的代码修改，直接配置编
译项即可。（Kernel 3.18以后才支持）
CDC MBIM 驱动的相关配置项：
	 
	CONFIG_USB_USBNET=y 
	CONFIG_NETDEVICES=y 
	CONFIG_USB_NET_CDC_MBIM=y

kernel\drivers\net\usb

Makefile

	obj-$(CONFIG_USB_NET_CDC_MBIM)	+= cdc_mbim.o

Kconfig

	config USB_NET_CDC_MBIM
		tristate "CDC MBIM support"
		depends on USB_USBNET
		select USB_WDM
		select USB_NET_CDC_NCM
----------
#其他



### RmNet和CDC-ECM区别：
RmNet获取公网IP，ECD-ECM获取局域网IP。
在高通平台上，rmnet driver 和标准的CDC-ECM是有区别的，rmnet 也是属于CDC-ECM
从模块角度出发，对USB命令的封装、使用的USB接口、端点定义方式不同；
使用rmnet,发起data call是通过QMI工具发的QMI命令，而通过标准的CDC-ECM发起data call，则是发送标准的ECM命令。
使用rmnet建立的data call，不走router的，IP地址的是公网IP。
而通过标准的CDC-ECM建立的data call，是走router的，获得的IP地址是私有的IP如192.168开头。

### quectel-CM

常见问题

- 检查quectel-CM和网口驱动版本。要求使用最新的驱动和工具。老版本测试遇到的问题不予处理。
- 确认注网情况。Quectel-CM log打印的内容包含了SIM卡状态，注网情况。
- APN检查。物联网卡和私网卡、专网卡慎重检查APN。先查设空APN情况下网络下发的APN是什么。
- DHCP client工具。推荐使用udhcpc，确实没有udhcpc，CM支持dhcpclient，修改udhcpc.c，打开#define USE_DHCPCLIENT
- Udhcpc 默认的default.script 不能在Windows系统里打开。在ubuntu上执行dos2unix default.script 和chmod 777 default.script 再拷贝到嵌入式系统中。
- Udhcpc使用default.script 通常是/usr/share/udhcpc/和/etc/udhcpc/，建议两处都拷贝一份。
- 如果客户使用default.script 不是我们提供的quectel-CM里带的default.script ，试试用quectel-CM里的default.script
- Udhcpc 有-s 选项指定default.script.
- 遇到ioctl和ptheread_cond_timeout_np这样的log，一般是QMI不通。CM的-v选项可以打印QMI消息，提供这样的log。必要提交usbmon和modem log进一步分析。
- quectel-CM 和modemmanger 有冲突。Apt remove –purge modemmanger 卸载。Debian类和centos类的系统上默认都有modemmanger。部分模块(EC2X)支持modemmanger拨号，5G未验证。


扩展

1.指定PID文件

/var/run/目录下的*.pid文件是一个文本文件，其内容只有一行，即某个进程的PID。.pid文件的作用是防止进程启动多个副本，只有获得特定pid文件（固定路径和文件名）的写入权限（F_WRLCK）的进程才能正常启动并将自身的进程PID写入该文件，其它同一程序的多余进程则自动退出。
