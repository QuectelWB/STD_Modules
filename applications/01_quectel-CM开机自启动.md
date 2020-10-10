# 将quectel-CM添加到systemd #


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

### OpenWrt add quectel-CM 开机启动 ###


[OpenWrt官方添加启动脚本方法介绍](https://oldwiki.archive.openwrt.org/doc/techref/initscripts)

![openwrtinit.png](https://i.loli.net/2020/09/30/rlfP2SQhnIF7dzw.png)

执行/etc/init.d/quectel-CM enable

再次开机后，quectel-CM 会自动执行拨号。



----------
<button onclick="window.location.href = '../Applications.html'"><font color=0x00FF >返回上页</font></button>












