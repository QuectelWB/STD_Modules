# 串口发送AT指令 #

### 串口工具 ###

在ubuntu PC上,使用microcom或者minicom

	$apt-get install -y microcom

在ubuntu 上
 sudo microcom -p /dev/ttyUSB2 操作
 
	root@imx6qdlsabresd:~# busybox microcom /dev/ttyUSB2 
	at+csq;+cgreg?;+cops? 
	+CSQ: 27,99 
	+CGREG: 0,1 
	+COPS: 0,0,"CHINA MOBILE",7 
	OK 
 
	AT$QCRMCALL=1,1 
	$QCRMCALL: 1,V4 
	OK 
 
	AT+QNETDEVSTATUS? 
	+QNETDEVSTATUS: 0,1,4,1 
	OK 
 
	root@imx6qdlsabresd:~# busybox udhcpc -fnq -i wwan0 
	udhcpc (v1.24.1) started 
	Sending discover... 
	Sending select for 10.166.47.120... 
	Lease of 10.166.47.120 obtained, lease time 7200 
	/etc/udhcpc.d/50default: Adding DNS 211.138.180.2 
	/etc/udhcpc.d/50default: Adding DNS 211.138.180.3 
	root@imx6qdlsabresd:~# 


临时调试

### Shell ###

echo -ne "ati\r\n" > /dev/ttyUSB2 

cat /dev/ttyUSB2    
	不推荐cat串口的操作（会阻塞；Android禁止cat AT端口）

如果装有串口工具如microcom
echo -ne "ati\r" | busybox microcom -t 3 /dev/ttyUSB2

### 编程 ###

python之serial类

[testWifi.py](archives/testWifi.py)

C语言之termios

   [myAT.c](archives/myAT.c)

----------








