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
- IP tables；
- rp_filter；
- 是否拨号成功 AT+QNETDEVSTATUS rmnet_data0
- tcpdump 抓包
- usbmon；
- QLog；

## 和主控有关或者和硬件的异常问题

- 枚举异常；
- AT超时不通；
- quectel-CM有些QMI消息不通；
- 大流量的情况下出现网卡崩溃异常；


## 其他

