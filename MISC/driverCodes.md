Code Issuses
=====

GobiNet qcqmiX从0计数
----

客户使用CentOS 7，接3片EC20模组，当重启其中一片EC20后，遇到


![](GobiNet_qcqmiX.png)

![](GobiNet_qcqmiX_duplicated.jpg)

GobiNetDriverBind
...
	{

		struct net_device*d;
		struct net*net = dev_net(pDev->net);
		for_each_netdev(net,d)
			printk("%s net device name = %s \n",__func__,d->name);
	}

	[418370.783287] creating qcqmi0
	[418370.783831] GobiNetDriverBind net device name = lo
	[418370.783832] GobiNetDriverBind net device name = eth0
	[418370.783833] GobiNetDriverBind net device name = wlan0
	[418370.783834] GobiNetDriverBind net device name = docker0
	[418370.783835] GobiNetDriverBind net device name = usb0


