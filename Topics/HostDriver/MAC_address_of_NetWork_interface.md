实际上网卡工作在raw ip模式，MAC地址是没意义的。并且QMAP分出的多个网卡有相同的MAC。

客户出于产品目的，需要修改MAC地址或者固定IP地址。

客户可以使用命令

	ifconfig wwan0.1 hw ether xx:xx:xx:xx:xx:xx
	
如果需要开机固定，请命令添加到开机脚本中。

但是！！！
客户发现不能修改，报错。需要修改网卡驱动源码qmi_wwan_q.c

## QMAP 分出的多路网卡MAC地址随机

	
	+++ b/qmi_wwan_q.c
	@@ -1158,7 +1158,16 @@ static int qmap_register_device(sQmiWwanQmap * pDev, u8 offset_id)
	priv->mux_id = QUECTEL_QMAP_MUX_ID + offset_id;
	sprintf(qmap_net->name, "%s.%d", real_dev->name, offset_id + 1);
	qmap_net->netdev_ops = &qmap_netdev_ops;

	-memcpy (qmap_net->dev_addr, real_dev->dev_addr, ETH_ALEN);
	
	+ //memcpy (qmap_net->dev_addr, real_dev->dev_addr, ETH_ALEN);
	+ 
	+ eth_hw_addr_random(qmap_net);
	+ if (ether_addr_equal(qmap_net->dev_addr, default_modem_addr))
	+ eth_hw_addr_random(qmap_net);
	+
	+ /* make MAC addr easily distinguishable from an IP header */
	+ if (possibly_iphdr(qmap_net->dev_addr)) {
	+ qmap_net->dev_addr[0] |= 0x02; /* set local assignment bit */
	+ qmap_net->dev_addr[0] &= 0xbf; /* clear "IP" bit */
	+ }


## 支持手动修改

  ifconfig wwan0.1 hw ether xx:xx:xx:xx:xx:xx

if you failed, modify the qmi_wwan_q.c as follow

	+  static int qmi_wwan_mac_addr(struct net_device *dev, void *p);
	static const struct net_device_ops qmap_netdev_ops = {
	.ndo_open = qmap_open,
	.ndo_stop = qmap_stop,
	.ndo_start_xmit = qmap_start_xmit,
	+  .ndo_set_mac_address = qmi_wwan_mac_addr,
	+  .ndo_validate_addr = eth_validate_addr,
	};

对5G模组，以下两个结构体也要加上

	+  .ndo_set_mac_address = qmi_wwan_mac_addr,
	+  .ndo_validate_addr = eth_validate_addr,

qmi_wwan_q.c
	
	static const struct net_device_ops qmap_netdev_ops = {
		.ndo_open       = qmap_open,
		.ndo_stop       = qmap_stop,
		.ndo_start_xmit = qmap_start_xmit,
	};

	static const struct net_device_ops rmnet_vnd_ops = {
		.ndo_open       = qmap_open,
		.ndo_stop       = qmap_stop,
		.ndo_start_xmit = rmnet_vnd_start_xmit,
		.ndo_change_mtu = rmnet_vnd_change_mtu,
	#if defined(MHI_NETDEV_STATUS64)
		.ndo_get_stats64	= rmnet_vnd_get_stats64,
	#endif
	};

