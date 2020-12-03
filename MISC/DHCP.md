DHCP
====

关于DHCP和dhcp客户端程序

DHCP 动态主机配置协议，局域网网络协议，使用UDP协议工作，端口67（Server）、68（Client）。

- 主机发送DHCP Discover广播包，寻找DHCP Server;
- DHCP Server 向主机发送 DHCP Offer单拨数据包，包括IP、MAC、域名、Lease Time。
- 主机发送 DHCP Request广播包，正式向 服务器请求分配已经提供的IP地址；
- DHCP Sever向主机发送 DHCP ACK单播包，确认主机的请求

