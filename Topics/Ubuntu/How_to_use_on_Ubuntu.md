If you are using Ubuntu , you may encounter some problems as below:

### modemmanager

quectel-CM 拨号的时候，不要运行ModemManager. 建议卸载。

apt-get remove --purge modemmanager

如果坚持使用ModemManager


If your modemmanager is the default one or get it through apt-get.

The default version is not suitable for RG500Q and RM500Q.


	q@q-K501LB:~/Carl/carls/ModemManager$ mmcli --version
	mmcli 1.12.8
	Copyright (2011 - 2019) Aleksander Morgado

**支持5G的ModemManager**

RMNET(usbnet 0,qmi/GobiNet driver) interface cannot use Modemmanager.

	qmi_wwan_q and GobiNet is not in the Kernel Upstreaming flow，so modemmanager never test based on it and not support 5G modules；
	Modemmanager support modules on qmi_wwan, but qmi_wwan doesn't support QMAP. 

It is recommend to use cdc_mbim driver,(at+qcfg=usbnet,2)
         And the modemmanager must apply next patchs.


	commit 5346d3c55c7ffc38af58c879fd426a2c3accd489
	Author: carlyin <carl.yin@quectel.com>
	Date:   Sun Aug 16 16:55:25 2020 +0800
	 	
	    quectel: add port type hints for the Quectel 5G RM500
 
	commit 55ae47a3bb874991ca621e4aa17cf16b08d28139
	Author: Aleksander Morgado <aleksander@aleksander.es>
	Date:   Sun Aug 16 11:06:57 2020 +0200
	 
	    quectel: add udev rules unit tester
	 
	commit d9a64c74ff0244b96a44fa27abcdc411d29f4874
	Author: root <carl.yin@quectel.com>
	Date:   Thu Aug 13 11:25:59 2020 +0800
	 
	    quectel: allow MBIM devices


### make sure qmi_wwan_q have been probed

Mostly, qmi_wwan will be matched, remove qmi_wwan.ko

qmi_wwan.ko is in 

	/lib/modules/`uname -r`/kernel/drivers/net/usb/

ReMove qmi_wwan.ko, and once you reboot your Ubuntu plaform, and qmi_wwan_q will match Quectel modules.

check it by "lsusb -t".


### net interface name

Find your net interface with a strange name?

	vim /etc/default/grub
       		GRUB_CMDLINE_LINUX=net.ifnames=0 biosdevname=0 
	grub-mkconfig -o /boot/grub/grub.cfg


reboot , and everything calm down.

If you don't like the ethX and usbX

[usbnet.c](https://elixir.bootlin.com/linux/latest/source/drivers/net/usb/usbnet.c)


```usbnet.c
int usbnet_probe (struct usb_interface *udev, const struct usb_device_id *prod)
{
...
	strcpy (net->name, "usb%d");

	// allow device-specific bind/init procedures
	// NOTE net->name still not usable ...
	if (info->bind) {
		status = info->bind (dev, udev);
		if (status < 0)
			goto out1;

		// heuristic:  "usb%d" for links we know are two-host,
		// else "eth%d" when there's reasonable doubt.  userspace
		// can rename the link if it knows better.
		if ((dev->driver_info->flags & FLAG_ETHER) != 0 &&
		    ((dev->driver_info->flags & FLAG_POINTTOPOINT) == 0 ||
		     (net->dev_addr [0] & 0x02) == 0))
			strcpy (net->name, "eth%d");
		/* WLAN devices should always be named "wlan%d" */
		if ((dev->driver_info->flags & FLAG_WLAN) != 0)
			strcpy(net->name, "wlan%d");
		/* WWAN devices should always be named "wwan%d" */
		if ((dev->driver_info->flags & FLAG_WWAN) != 0)
			strcpy(net->name, "wwan%d");

		/* devices that cannot do ARP */
		if ((dev->driver_info->flags & FLAG_NOARP) != 0)
			net->flags |= IFF_NOARP;

		/* maybe the remote can't receive an Ethernet MTU */
		if (net->mtu > (dev->hard_mtu - net->hard_header_len))
			net->mtu = dev->hard_mtu - net->hard_header_len;
...
}

```


### dhclient

DHCP client is used for requsting IP ,subnet netmask, Gateway, DNS.

There are udhcpc and dhclient which can be used.

#### udhcpc 

apt-get install udhcpc -y

mkdir /etc/udhcpc/

apt-get instlall -y dos2unix

dos2unix  default.script   // default.script is the script file in the source drectory of quectel-CM

chmod 777 default.script

cp default.script /etc/udhcpc/



将quectel-CM添加到systemd
--------

Systemd 是 Linux 系统工具，用来启动守护进程，已成为大多数发行版的标准配置。
传统的init进程方式添加启动项，存在的缺点：

	启动时间长。init进程是串行启动，只有前一个进程启动完，才会启动下一个进程。
	启动脚本复杂。init进程只是执行启动脚本，不管其他事情。脚本需要自己处理各种情况，这往往使得脚本变得很长。

1.准备好quectel-CM和udhcpc/dhclient/default.script等
假设路径是quectel-CM编译好，push在/usr/bin/目录中。chmod +x /usr/bin/quectel-CM, 执行quectel-CM可以拨号。

2.创建一个service文件
进入/etc/systemd/system/，创建一个CM.service文件，内容如下：
    
    [Unit]
    Description=just for quectel-CM 填简介
    After=Network.service　Resolv.service   前置service，譬如Network，Resolv
    
    [Service]
    ExecStart=/usr/bin/quectel-CM
      
    
    [Install]
    WantedBy=multi-user.target

使用以下命令使能这个服务开机启动：

重新加载配置文件

	$ sudo systemctl daemon-reload              service文件改动后要重新转载一下
	$ sudo systemctl enable my.service          这句是为了设置开机启动

如果你想不重启立刻使用这个sh脚本，就运行下面这句：

重启相关服务

	$ sudo systemctl start my.service           启动服务


CentOS的编译问题
------

usb_serial_port 结构体的内容报错

	下载内核源码
	https://vault.centos.org/7.0.1406/os/Source/SPackages/kernel-3.10.0-123.el7.src.rpm



