# EasyStart

**EC2X篇**

快速使用Quectel模组

This chapter is for those who are not familar with LTE network, have never used AT commands and want to start datacall or connection with the Internet as quick as posible.

	for EC2x/EGxx
	USB connection (include miniPCIe) 
	Ubuntu OS(much better to use, Other OS like Debian/Centos is Ok, not fit for embeded platfom. For embeded platform, it is better to 
	know much more)


1. Check Hardware connection, Make sure the Ubuntu can recognize the USB device. 

硬件连接无问题，主控可以识别到USB模组。

- Before and after the Quectel module was inserted, try  lsusb  command in  your Terminal. After the module was inserted, there will be an USB device of which the vid is 0x2C7C.

lsusb 命令能看到Quectel模组（VID：0x2C7C）

- "sudo dmesg | grep usb", plug and unplug module no kernel error message printing, enumeration information

通过dmesg log 可以看到插拔模组的对应log

	[16310.531091] usb 1-4: new high-speed USB device number 11 using xhci_hcd
	[16310.563123] usb 1-4: New USB device found, idVendor=2c7c, idProduct=0125, bcdDevice= 3.18
	[16310.563125] usb 1-4: New USB device strings: Mfr=1, Product=2, SerialNumber=0
	[16310.563126] usb 1-4: Product: Android
	[16310.563127] usb 1-4: Manufacturer: Android


2.确认是否注网成功

用AT指令确认模组是否注上LTE或者5G网络。先使用
<font color=0xff0000>AT+COPS? </font> 返回当前运营商及其状态信息。
如

	at+cops?
	+COPS：0,0,"CHN-UNICOM",7 
	
看到串口返回信息中 CHN-UNICOM表示当前运营商是中国联通，当前接入的是E-UTRAN，即LTE网络。

3.USB串口驱动

Ubuntu 16.04 及以后的版本，内核支持quectel的模组，许多Quectel模组的信息已经加进Kernel Upstreaming

如在Ubuntu 20.04.1 LTS中，内核驱动的option.c 中支持的模组包括

	static const struct usb_device_id option_ids[] = { 
	#if 1 //Added by Quectel 
	 
	{ USB_DEVICE(0x2C7C, 0x0125) }, /* Quectel EC20 R2.0/EC20 R2.1/EC25/EG25-G/EM05 */ 
	 
	{ USB_DEVICE(0x2C7C, 0x0121) }, /* Quectel EC21/EG21-G */ 
	 
	{ USB_DEVICE(0x2C7C, 0x0191) }, /* Quectel EG91 */ 
	 
	{ USB_DEVICE(0x2C7C, 0x0195) }, /* Quectel EG95 */ 
	 
	{ USB_DEVICE(0x2C7C, 0x0306) }, /* Quectel EG06/EP06/EM06 */ 
	{ USB_DEVICE(0x2C7C, 0x0512) }, /* Quectel EG12/EM12/EG18 */ 
	{ USB_DEVICE(0x2C7C, 0x0296) }, /* Quectel BG96 */ 
	{ USB_DEVICE(0x2C7C, 0x0700) }, /* Quectel BG95/BG77/


或者将Quectel模组的VID、PID信息临时写入option驱动

echo 2C7C 0800 > /sys/bus/usb-serial/drivers/option1/new_id

再插入移远模组的时候，/dev/目录下会生成ttyUSB0~3 的设备节点，其中ttyUSB2用于发送AT指令，ttyUSB3用于ppp拨号。


----------

> Level 1  使用ECM网卡

AT指令将模组配置成ECM接口
	
	echo -ne "at+qcfg=\"usbnet\",1\r\n" > /dev/ttyUSB2
	
重启模组后模组就可以联网了。

如果不可以，如网卡没有IP，用dhcp客户端程序(udhcpc或dhclient)申请IP，如udhcpc -i usb0

拿到IP后，如果发现不能联网，将192.168.225.1设置成默认网关和DNS

----------

> Level 2 ppp拨号

Step 1 确认/dev/ttyUSB3 是否存在


Step 2 修改linux-ppp-scripts_V1.2



- 方式1： 拷贝 quectel-chat-connect quectel-chat-disconnect quectel-ppp 到 /etc/ppp/peers 目录下。
并在 quectel-ppp 里修改你的串口设备名，pppd 拨号使用的 username，password。
在 quectel-chat-connect 里修改你的 APN。APN/username/password 是从你的网络提供商那里获取的。
然后使用下面的命令启动 ppp 拨号， 命令最后的 & 可以让 pppd 后台运行
pppd call quectel-ppp &


- 方式2：使用<font color="red">quectel-pppd.sh </font>  拨号，命令形式如下:
<font color="red">./quectel-pppd.sh </font> 串口设备名(比如/dev/ttyUSB3) APN username password

	#quectel-pppd devname apn user password	
	echo "quectel-pppd options in effect:"	
	QL_DEVNAME=/dev/ttyUSB3	
	QL_APN=3gnet	
	QL_USER=user	
	QL_PASSWORD=passwd

>ip-up：pppd 在获取 ip 和 dns 之后，会自动调用这个脚本文件来设置系统的 DNS
嵌入式系统一般需要拷贝这个文件到 /etc/ppp 目录下。
请确保该文件在你的系统里有可执行权限。
quectel-ppp-kill 用来挂断拨号的，pppd必须被正常的挂断，否则可能会导致你下次ppp拨号失败。
使用下面方式来调用这个脚本 ./quectel-ppp-kill 

----------

> Level 3 RMNET网卡

Ubuntu内核自带qmi_wwan


使用Quectel提供的Quectel\_QConnectManager\_Linux,编译得到quectel-CM, 在quectel-CM源码中存在default.script

	sudo apt-get install -y udhpcpc 
	sudo apt-get install -y dos2unix
	dos2unix default.script
	sudo chmod 777 default.script
	mkdir /etc/udhcpc/
	cp default.script /etc/udhcpc/

执行quectel-CM，即联网成功 
	
	sudo ./quectel-CM &

----------

**EC200X篇**

EC200X和EC2X在使用差别有共通处，也有很多不同处。

	modprobe option
	#如果是EC200S或者EC600S
	echo 0x2C7C 0x6002 > /sys/bus/usb-serial/option1/new_id
	#如果是EC200T
	echo 0x2C7C 0x6002 > /sys/bus/usb-serial/option1/new_id
	#AT指令检查网卡类型，使用ECM网卡
	AT+QCFG="usbnet"返回1
	#AT拨号
	AT+QICSGP=1,5,"cmnet"
	AT+QNETDEVCTL=1,1,1
	#dhcp client 
	udhcpc -i usb0





:)
