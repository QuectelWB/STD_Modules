回环测试
=====


## 目的和原理

X55 支持 rmnet/qmi 网卡的 loopback，即上位机通过 rmnet 网卡发送数据给 X55， X55 可以回环给上位机。 
而且可以定义回环倍数，假设是 7 倍，则效果就是发 10M 的数据给 X55，X55 回环 70M 的数据给你。 
 
5G 的速率非常高，客户的 AP 必须正确的配置之后(比如 CPU 的负载平衡，软件/硬件加速方案等等)，才能测到最高的速率。 
Loopback 测试不需要插 SIM 卡(也就是不需要实网)，也不需要仪器，非常方便测试/调试 AP 的性能。 
建议客户在做实网和仪器 TPUT 之前，都先做 loopback 测试.

![](X55_Loopback.bmp)



## 高通5G

- 使能loopback

	通过QPST传文件，详见文档 AT+QNVFR="/nv/item_files/modem/data/3gpp/ps/loopback_config.txt"
	quectel-Cm -l 回环倍数

- iperf 打流
- mpstat 检查

## 展锐5G

- 使能loopback

	echo 10 > /proc/net/sfp/loopback_enable    /*拷贝份数,0为关闭, >0为开启, 1-15为取值范围*/
	at+qcfg="loopback",10



## Android

ql-ril.conf 中添加

	support_X55_loopback=回环倍数

## 网卡性能FAQ

网卡队列/中断;
mpstat;
CPU性能
TCP/UDP窗口大小

## ref

[Kernel Doc: SMP IRQ affinity](https://www.kernel.org/doc/html/latest/core-api/irq/irq-affinity.html)
