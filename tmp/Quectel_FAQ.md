# FAQ

## usbnet 类型和通信的注意事项

- 每种网卡类型的特点
- 和OpenLinux的联系
- IP地址、MAC 地址、子网掩码、MTU大小的问题

## quectel-CM的注意事项

- 和qmicli uqmi之间的关系，可以借鉴的地方libqmi
- raw_ip 
- quectel-CM 中能判断到的信息
- udhcpc 的问题
- 客户喜欢研究的东西。进程管理，IP通信问题。

## option驱动的注意事项 （重点）

- 串口少了或者多了
- 串口不通
- 串口的顺序不对

客户遇到串口节点数量不对，缺少网卡，adb功能不对，uac功能无法使用的情况。第一步就要检查驱动挂载对不对。

cat /sys/kernel/debug/usb/devices
或者抓插入模组时候的内核log， dmesg 或者 cat /proc/kmsg

最常见的就是使用EC200x、EC600x、Rx500U 出现只有2个ttyUSB的情况。

## 网卡驱动驱动注意事项

- 驱动匹配不上或者不对
- 网卡名称
- 流量统计
- 设备节点异常
- 网卡驱动崩溃
- 性能优化 

## 网络不通的问题分析流程 （重点）

- IP地址；
- DNS；
- 域名；
- ping 网关；
- route表；
- iptables；
- rp_filter；
- AT+QPING
- 是否拨号成功 AT+QNETDEVSTATUS rmnet_data0
- tcpdump 抓包
- usbmon；
- QLog；

客户反馈网络不通，具体是什么现象？

	ping 不通地址还是ping不通域名，还是无法上网（http访问，curl）； 
	理解RMNET/ECM网卡的特征，判断是模块问题还是的主控的问题；
	网关在RMNET这种接口是ping不通的，但是ECM/RNDIS(nat 主控拿到局域网ip 192.168.x.x)这种情况是可以ping通的；
	能ping通网关的情况下，就是模块问题；
	如果AT+QPING能够ping通，一般来说，模块也是没问题；
	rmnet一直要求dhcp包不能带以太网头部，只能是纯ip包;
	echo Y > /sys/class/net/qmi_wwan/qmi/raw_ip 对qmi_wwan_q不需要，对最新CM也不需要；
	AT都不通，AT指令都延时，ppp拨号肯定不行;

### 例如 Android rmnet拨号

在一个rk的android 11的平台上出现拿到了ip地址，RIL拨号log看起来也正常的情况。但是无法上网。将模组切换成ECM是正常的。AT+QPING也没问题。抓包发现
usbmon 的log

	289 ffffffc017941d00 1074237291 S Bo:2:002:5 -115 98 = d286e6c7 8cfed286 e6c78cfe 08004500 005432c0 40004001 449a0a58 a8e70808
	290 ffffffc017941d00 1074237565 C Bo:2:002:5 0 98 >
	291 ffffffc017941f00 1075250563 S Bo:2:002:5 -115 98 = d286e6c7 8cfed286 e6c78cfe 08004500 00543381 40004001 43d90a58 a8e70808
	292 ffffffc017941f00 1075250836 C Bo:2:002:5 0 98 >
	293 ffffffc017941800 1076263916 S Bo:2:002:5 -115 98 = d286e6c7 8cfed286 e6c78cfe 08004500 00543386 40004001 43d40a58 a8e70808
	294 ffffffc017941800 1076264185 C Bo:2:002:5 0 98 >
	295 ffffffc017941900 1077277171 S Bo:2:002:5 -115 98 = d286e6c7 8cfed286 e6c78cfe 08004500 005434b2 40004001 42a80a58 a8e70808

tcpdump/wireshark 看到的也是只有出的没有进的。更新qmi_wwan后解决。

	


## 和主控有关或者和硬件的异常问题

- 枚举异常；
- AT超时不通；
- quectel-CM有些QMI消息不通；
- 大流量的情况下出现网卡崩溃异常；


## 其他

