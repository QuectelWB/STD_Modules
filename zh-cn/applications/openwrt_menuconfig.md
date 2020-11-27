在sdk源码的package目录下创建文件夹quectel，将应用程序的源码和驱动放到quectel目录里，并添加Makefile；在源码根目录下make menuconfig的时候选择对应的工具，最后在源码根目录下的config中可以看到对应选项。
 
OpenWrt添加package
![openwrt.png](https://i.loli.net/2020/10/05/YJf3QkZUSEmdwjN.png)

openwrt_sdk/package/quectel/feeds/GobiNet/
[Makefile](applications/openwrt/package/quectel/feeds/GobiNet/Makefile)

openwrt_sdk/package/quectel/feeds/qmi_wwan_q/
[Makefile](openwrt\package\quectel\feeds\qmi_wwan_q\Makefile)

openwrt_sdk/package/quectel/feeds/pcie_mhi/
[Makefile](openwrt\package\quectel\feeds\pcie_mhi\Makefile)

openwrt_sdk/package/quectel/feeds/QLog/
[Makefile](openwrt\package\quectel\feeds\QLog\Makefile)

openwrt_sdk/package/quectel/feeds/QFirehose/
[Makefile](openwrt\package\quectel\feeds\QFirehose\Makefile)

openwrt_sdk/package/quectel/feeds/quectel-CM/
[Makefile](openwrt\package\quectel\feeds\quectel-CM\Makefile)

----------

在make menuconfig时，

	Utilities --- >
		<*><QFirehose> …………………………..Qfirehose app build test
		<*><QLog>…………………………………… QLog app build test
		<*><quectel-CM>……………………………quectel-CM app build test

编译后最终会生成Qlog可执行文件，并安装到OpenWrt的/usr/bin/目录。


在make menuconfig的时候，选中pcie_mhi驱动。对应上面提供的Makefile，如下配置

	Kernel modules --- >
		Network Support --- >
			<*>kmod-pcie_mhi……………………Quectel Linux PCIE MHI Driver

Make menuconfig后可以检查下.config是否添加了
	
	CONFIG_PACKAGE_quectel-CM=y
	CONFIG_PACKAGE_QFirehose=y
	CONFIG_PACKAGE_QLog=y
	CONFIG_PACKAGE_kmod-pcie_mhi=y
	CONFIG_PACKAGE_kmod-qmi_wwan_q=y
	CONFIG_PACKAGE_kmod-GobiNet=y




