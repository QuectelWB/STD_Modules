Linux内核和驱动，还可以用打patch的方式。因为Linux内核和OpenWrt的其他软件包一样，是在编译过程中下载解压的，OpenWrt对这些软件包的修改方法是将这些软件包编译过程中，解压后可以加上用户自己的patch，可以参考

https://blog.csdn.net/sevenjoin/article/details/50767123

下面演示在添加VID、PID的方法，QUILT是OpenWrt的Patch管理工具。

	<1>安装QUILT
	sudo apt-get install quilt
	<2>准备内核源码树
	cd    $(SDK_DIR)     		//进入顶层目录
	make target/linux/{clean,prepare} V=s QUILT=1     
	 //准备内核源码，用于产生原始patch，会将源码恢复到原始状态并打全存在的patch
	<3>进入kernel源码顶层目录
	<4>安装所有的patch - 这一步可以不执行，通常在执行步骤1的时候已经应用了所以的patch了
	quilt push -a     //应用所有的patch， 在当前patches/目录下
	<5>创建新patch，也可以修改已经存在的patch，
	quilt new platform/998-usb-option.patch
	<6>修改源码文件drivers/usb/serial/option.c
	//通过edit打开要编辑的文件并写入修改信息
	<7>查看修改的内容，可通过下面指令：quilt diff
	<8>产生新修改的patch文件
	quilt refresh  //必须执行，否则将不能生成patch，执行后新patch被创建在patches/platform目录下
	<9>将新生成的patch更新到目标opwert patches目录下，即 target/linux/ipq40xxpatches-4.14
	cd $(SDK_DIR)     //进入opwrt顶层目录
	make target/linux/update V=s        更新patch到目标存储位置。

