ModemManager
======

If your modemmanager is the default one or get it through apt-get.

The default version is not suitable for RG500Q and RM500Q.


	q@q-K501LB:~/Carl/carls/ModemManager$ mmcli --version
	mmcli 1.12.8
	Copyright (2011 - 2019) Aleksander Morgado

支持5G的ModemManager
------

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


编译最新的ModemManager
--------






































:)

:w


