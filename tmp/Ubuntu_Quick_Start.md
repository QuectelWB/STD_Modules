快速使用SOP
======

推荐使用QMI方式

## 需要准备的工具

移远提供的

- 驱动 qmi_wwan_q
- 拨号工具 quectel-CM

这两个由Quectel提供源码

串口驱动需要移植option驱动。对EC20应该不用，默认EC20已经添加进Ubuntu内核中了。

Ubuntu上需要的

- udhcpc 建议使用udhcpc，实在搞不定用dhclient（dhclient是Ubuntu自带的）

  如果能联网apt安装，如果不能可以考虑通过编译busybox或者编译udhcpc 的方式。
  编译busybox添加 udhcpc的操作，USB驱动移植文档中有介绍。  

  编译udhcpc
  [udhcpc的源码](https://udhcp.busybox.net/source/udhcp-0.9.8.tar.gz)

  udhcpc 调用default.script 可以直接用quectel-CM源码里的default.script
  [udhcpc的源码](https://udhcp.busybox.net/source/udhcp-0.9.8.tar.gz) 这里的udhcpc调用default.script 的位置是/usr/share/udhcpc/，在Ubuntu上需要创建该目录

  default.script 不能以DOS字符结尾。所以建议执行下dos2unix default.script. 目标机器上不能执行的话，在其他Linux平台上执行再直接拷贝过去也是可以的。


## 驱动和工具的编译

qmi_wwan_q 提供的源码直接make && sudo make install 即可。
quectel-CM源码编译遇到cc1的报错，删掉Makefile中的CFLAGS。
如果搞不定udhcpc，要使用dhclient的话，udhcpc.c 打开 #USE_DHCLIENT 的宏

## 准备就绪

模组默认就是QMI方式，AT指令 AT+QCFG=“usbnet” 返回0.

如果返回不是0, 用AT+QCFG="usbnet",0 配置下，记得模块需要重启（重启命令 AT+CFUN=1,1）.

在Ubuntu上 lsusb 能看到EC20的VID（2C7C）,PID(0125), lsusb -t 能看到接口描述符4（If 4）挂载的驱动是qmi_wwan_q, 如果不是qmi_wwan_q 而是qmi_wwan,执行

	modprobe -r qmi_wwan
	modprobe -r qmi_wwan_q
	modprobe qmi_wwan_q

然后再用lsusb -t 检查下USB挂载是否正确.

模块要正常注网

	AT+COPS? 返回最后一位应该是7 表示LTE
	ATCREG? 返回0,1
	AT+CEREG? 返回0,1
	AT+CSQ  信号正常
	
## 拨号

运行quectel-CM即可。sudo ./quectel-CM &
指定APN的话，sudo ./quectel-CM -s cmnet &

默认情况, quectel-CM 最后会调用 udhcpc.c 将网络下发的DNS写入/etc/resolv.conf.

