
本节内容包括与模组相关的USB和网络基础知识，

USB基础部分适用于

	希望理解Quectel模组USB工作原理；
	希望通过单片机（STM32、AVR、C51）USB方式和模组通讯；
	理解调试USB模组和Host AP之间连接的更基本原理；
	USB基础硬件知识；

网络基础适用于
	
	计算机网络基础概念；
	软路由系统；
	调试网络通顺、卡顿问题；

对大多数客户使用，本节可以直接跳过。


----------
# ***USB***

*[USB](https://elixir.bootlin.com/linux/v4.14.199/source/Documentation/usb)*

*[Gadget Framework](https://www.kernel.org/doc/htmldocs/gadget/)*


----------
# ***NetWork***

---

<kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>Del</kbd> 

<table border="1">
<tr>
<td>Class</td>
<td>Leading bits</td>
<td> Size of network number bit field</td>
<td>Size of rest bit field </td>

<td>Start address</td>
<td>End address</td>
</tr>
<tr>
<td>A</td>
<td>0</td>
<td></td>

<td></td>
<td></td>
<td></td>

</tr>

</tr>
<tr>
<td>B </td>
<td>10 </td>
<td>24 </td>
<td>8 </td>
<td> </td>

<td> </td>

</tr>

<tr>
<td>C </td>
<td>110 </td>
<td>24 </td>
<td>8 </td>
<td> </td>

<td> </td>

</tr>

</table>

# 术语

https://blog.csdn.net/evan_love/article/details/79981574

## what is IP?
	
	normally ip is short for Internet Protocol, and sometime is for An Internet Protocol address (IP address) is a numerical label assigned to each device (e.g., computer, printer) participating in a computer network that uses the Internet Protocol for communication. An IP address serves two principal functions: host or network interface identification and location addressing. Its role has been characterized as follows: "A name indicates what we seek. An address indicates where it is. A route indicates how to get there."

	通常，ip是因特网协议的缩写，有时是指因特网协议地址(ip地址)，是分配给参与使用因特网协议进行通信的计算机网络的每个设备(如计算机、打印机)的数字标签。
	IP地址有两个主要功能:主机或网络接口识别和位置寻址。
	它的作用被描述为:“名字表明我们所追求的东西。IP地址表示它的位置。路由Route指示如何到达那里。”

## 子网掩码Subnet

	IP地址=网络地址+主机地址

	IP地址的表示方式，譬如为这个子网分配一个地址：233.1.1.0/24，其中的/24记法，有时称为子网掩码（networkmask）,指示了IP地址32比特中的最左侧24Bit定义了子网地址。通俗来讲，子网掩码用来指明一个IP地址的哪些位标识的是主机所在的子网，以及哪些位标识的是主机的位掩码。
	子网掩码作用，就是将某个IP地址划分成网络地址和主机地址两部分。	
	子网掩码——屏蔽一个IP地址的网络部分的“全1”比特模式。对于A类地址来说，默认的子网掩码是255.0.0.0；对于B类地址来说默认的子网掩码是255.255.0.0；对于C类地址来说默认的子网掩码是255.255.255.0。
	将IP地址和子网掩码都换算成二进制,然后进行与运算,结果就是网络地址

<a href="https://sm.ms/image/iW1ORDJIfStYhV8" target="_blank"><img src="https://i.loli.net/2020/09/30/iW1ORDJIfStYhV8.png" /></a>

## 网关

	网关(Gateway)又称网间连bai接器、协议转换器。
	网关和路由的区别：
		网关分为传输型网关和应用型网关俩类，主要作用是充当转换重任，它实质上是一个网络通向其他网络的IP地址。
		路由器的作用主要有：连通不同的网络和信息传输作用。按照使用可分为：接入、企业级、骨干级、太比特、多WAN以及3G无线等。

## IP地址、子网掩码、网关之间的联系


----------

# 常用的网络命令

