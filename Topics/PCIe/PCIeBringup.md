PCIeBringup
======

一般情况，要求客户的kernel和PCIe模组同时开机，因此

- kernel出现异常重启，PCIe模组也同时跟着重启，同样模组异常，模组需要重启的时候，Host主机也要同时重启。
- 一般Uboot不包含PCIe驱动，无法识别PCIe模组。需要在bootloader跑完后，才给模组上电开机。
- 一般情况，Host只会扫描一次PCIe设备，如果插入PCIe设备后，主动扫描的方法
	
	cd /sys/bus/pci/devices/0001:00:00.0
	echo 1 > rescan 


