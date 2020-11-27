----------



## How to dial up for Rmnet 

### quectel-CM


After the module is connected and the GobiNet or QMI_WWAN driver is installed successfully, a USB network adapter and a QMI channel will be created. The USB network adapter of the GobiNet driver is named as ethX (or usbX if the kernel version is 2.6.39 or lower), and the QMI channel is /dev/qcqmiX. The USB network adapter of the QMI_WWAN driver is named as wwanX, and the QMI channel is /dev/cdc-wdmX.

quectel-CM will send QMI message to the module via QMI channel to set up data connection. Please refer to the following table about the usage of quectel-CM: 


	root@cqh6:~# ./quectel-CM/quectel-CM & 
	[07-03_06:56:28:172] WCDMA&LTE_QConnectManager_Linux&Android_V1.3.4 
	[07-03_06:56:28:172] ./quectel-CM/quectel-CM profile[1] = (null)/(null)/(null)/0, pincode = (null) 
	[07-03_06:56:28:174] Find /sys/bus/usb/devices/2-1.2 idVendor=2c7c idProduct=0512 
	[07-03_06:56:28:174] Find /sys/bus/usb/devices/2-1.2:1.4/net/wwan0 
	[07-03_06:56:28:174] Find usbnet_adapter = wwan0 
	[07-03_06:56:28:175] Find /sys/bus/usb/devices/2-1.2:1.4/usbmisc/cdc-wdm0 
	[07-03_06:56:28:175] Find qmichannel = /dev/cdc-wdm0 
	[07-03_06:56:28:197] cdc_wdm_fd = 7 
	[07-03_06:56:28:381] Get clientWDS = 18 
	[07-03_06:56:28:445] Get clientDMS = 1 
	[07-03_06:56:28:509] Get clientNAS = 2 
	[07-03_06:56:28:573] Get clientUIM = 2 
	[07-03_06:56:28:637] Get clientWDA = 1 
	[07-03_06:56:28:701] requestBaseBandVersion EM12GPAR01A06M4G
	[07-03_06:56:28:957] requestGetSIMStatus SIMStatus: SIM_READY 
	[07-03_06:56:29:021] requestGetProfile[1] cmnet///0 
	[07-03_06:56:29:085] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[07-03_06:56:29:149] requestQueryDataCall IPv4ConnectionStatus: DISCONNECTED 
	[07-03_06:56:29:277] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[07-03_06:56:29:341] requestSetupDataCall WdsConnectionIPv4Handle: 0x127b42c0 
	[07-03_06:56:29:469] requestQueryDataCall IPv4ConnectionStatus: CONNECTED 
	[07-03_06:56:29:533] ifconfig wwan0 up 
	[07-03_06:56:29:543] busybox udhcpc -f -n -q -t 5 -i wwan0 
	udhcpc: started, v1.27.2 
	udhcpc: sending discover 
	udhcpc: sending select for 10.170.235.201 
	udhcpc: lease of 10.170.235.201 obtained, lease time 7200 
	[07-03_06:56:29:924] /etc/udhcpc/default.script: Resetting default routes 
	[07-03_06:56:29:936] /etc/udhcpc/default.script: Adding DNS 211.138.180.2
	[07-03_06:56:29:936] /etc/udhcpc/default.script: Adding DNS 211.138.180.3



##### quectel-CM for multiple PDN ####


Run quectel-qmi-proxy first

	root@cqh6:~# ./quectel-qmi-proxy -d /dev/cdc-wdm0 & 
	Will use cdc-wdm /dev/cdc-wdm0 
	qmi_proxy_init enter 
	qmi_proxy_loop enter thread_id 548403548656 
	link_prot 2 
	ul_data_aggregation_protocol 5 
	dl_data_aggregation_protocol 5 
	dl_data_aggregation_max_datagrams 10 
	dl_data_aggregation_max_size 4096 
	ul_data_aggregation_max_datagrams 16 
	ul_data_aggregation_max_size 4096 
	qmi_proxy_init finished, rx_urb_size is 4096 
	local server: quectel-qmi-proxy sockfd = 4 
	qmi_start_server: qmi_proxy_server_fd = 4 

Run quectel-CM to setup datacall then

	# ./quectel-CM -n 2 & 
	[06-04_05:13:16:473] WCDMA&LTE_QConnectManager_Linux&Android_V1.3.3 
	[06-04_05:13:16:473] ./quectel-CM profile[2] = (null)/(null)/(null)/0, pincode = (null) 
	[06-04_05:13:16:475] Find /sys/bus/usb/devices/2-1.2 idVendor=2c7c idProduct=0435 
	[06-04_05:13:16:476] Find /sys/bus/usb/devices/2-1.2:1.4/net/wwan0 
	[06-04_05:13:16:476] Find usbnet_adapter = wwan0 
	[06-04_05:13:16:476] Find /sys/bus/usb/devices/2-1.2:1.4/usbmisc/cdc-wdm0 
	[06-04_05:13:16:476] Find qmichannel = /dev/cdc-wdm0 
	[06-04_05:13:16:477] qmap_mode = 4, muxid = 0x82, qmap_netcard = wwan0.2 
	[06-04_05:13:16:477] connect to quectel-qmi-proxy sockfd = 7 
	[06-04_05:13:16:478] cdc_wdm_fd = 7 
	[06-04_05:13:16:535] Get clientWDS = 17 
	[06-04_05:13:16:567] Get clientDMS = 1 
	[06-04_05:13:16:599] Get clientNAS = 3 
	[06-04_05:13:16:631] Get clientUIM = 1 
	[06-04_05:13:16:663] requestBaseBandVersion AG35CEVAR05A06T4G 
	[06-04_05:13:16:791] requestGetSIMStatus SIMStatus: SIM_READY 
	[06-04_05:13:16:823] requestGetProfile[2] ///0 
	[06-04_05:13:16:855] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[06-04_05:13:16:887] requestQueryDataCall IPv4ConnectionStatus: DISCONNECTED 
	[06-04_05:13:16:950] requestRegistrationState2 MCC: 460, MNC: 0, PS: Attached, DataCap: LTE 
	[06-04_05:13:16:983] requestSetupDataCall WdsConnectionIPv4Handle: 0x86e513b0 
	[06-04_05:13:17:047] requestQueryDataCall IPv4ConnectionStatus: CONNECTED 
	[06-04_05:13:17:079] ifconfig wwan0 up 
	[06-04_05:13:17:089] ifconfig wwan0.2 up 
	[06-04_05:13:17:100] busybox udhcpc -f -n -q -t 5 -i wwan0.2 
	udhcpc: started, v1.27.2 
	udhcpc: sending discover 
	udhcpc: sending select for 10.247.97.242 
	udhcpc: lease of 10.247.97.242 obtained, lease time 7200 
	[06-04_05:13:17:449] /etc/udhcpc/default.script: Resetting default routes 
	[06-04_05:13:17:465] /etc/udhcpc/default.script: Adding DNS 211.138.180.2 
	[06-04_05:13:17:465] /etc/udhcpc/default.script: Adding DNS 211.138.180.3 


GobiNet Driver works much the same .


### AT cmd to set up data call 

AT+QCRMCALL=1,1

Supported on EC20/EC21/EC25/EG25/EG06 ...

Not Supported on RG500Q/RM500Q

### opensource qmi tools 

such as mmcli and libqmi

Module support that way .But Quectel provides limited technical support, especially for 5G modules.

----------

## ECM #



to be done

----------

## MBIM #
	 

to be done

----------



### quectel-CM


- Check quectel-CM and the port driver version.The latest drivers and tools are required.Problems encountered with older versions of the test are not handled.
- Confirm the status of injection network.The quectel-CM log print contains the SIM card status and network status.
- APN inspection.Check APN carefully for Internet of Things card, private network card and special network card.Let's find out what APN is going to be issued by the network in the case of null APN.
- DHCP Client tool. It is recommended to use udhcpc or busybox udhcpc. Quectel-CM also supports dhclient. To use dhclient, modify the source file udhcpc.c, by openning the micro "#define USE_DHCPCLIENT". On Ubuntu or Centos PC platform, it is recommended to use dhclient.
- The Udhcpc default script cannot be opened in Windows.Execute DOS2UNIX default.script and chmod 777 default.script on Ubuntu and copy them to the embedded platform.
- Default.script is usually /usr/share/udhcpc and /etc/udhcpc. It is recommended to make a copy of both.
- If the client USES default.script instead of the default.script in the quectel-cm, try using the default.script in the quectel-cm
- Default.script the Udhcpc has the -s option to specify default.script.
- Encountered ioctl and ptheread_cond_timeout_NP such log, generally is QMI not.The -v option for CM allows you to print QMI messages, providing logs like this.It is necessary to submit usbmon and modem log for further analysis.
- Quectel-cm and ModemManger have conflicts. Try "apt remove -- Purge Modemmanger" .Modemmanger is available by default on both Debian and Centos classes.Some modules (EC2X) support ModemManger dialing, 5G unverified.
- Specify the quectel-CM in one specified PID file. Just open this micro "CONFIG_PID_FILE_FORMAT".

## The different between Rmnet and ECM 

to be done
