USB串口包括

	DM(调试、升级固件)端口;
	GPS（输出GPS定位信息）端口；
	AT端口；
	ppp（调制解调器、ppp拨号）端口；

ppp 端口用于 Linux 侧和模块进行 PPP-Modem 拨号命令及数据业务的交互，同样可以发送AT指令。 
如果当前系统中无其他ttyUSB设备节点，当Linux内核集成option驱动后，接上Quectel模组，会生成设备节点/dev/ttyUSB0、/dev/ttyUSB1、/dev/ttyUSB2、/dev/ttyUSB3。

## 添加VID、PID ##
在${KERNELDIR}/driver/usb/serial/option.c中，添加Quectel模组的VID和PID信息。
    
	static const struct usb_device_id option_ids[] = {   
	#if 1 //Added by Quectel
	{ USB_DEVICE(0x2C7C, 0x0125) }, /* Quectel EC20 R2.0/EC20 R2.1/EC25/EG25-G/EM05 */
	{ USB_DEVICE(0x2C7C, 0x0121) }, /* Quectel EC21/EG21-G */
	{ USB_DEVICE(0x2C7C, 0x0191) }, /* Quectel EG91 */
	{ USB_DEVICE(0x2C7C, 0x0195) }, /* Quectel EG95 */
	{ USB_DEVICE(0x2C7C, 0x0306) }, /* Quectel EG06/EP06/EM06 */
	{ USB_DEVICE(0x2C7C, 0x0512) }, /* Quectel EG12/EM12/EG18 */
	{ USB_DEVICE(0x2C7C, 0x0296) }, /* Quectel BG96 */
	{ USB_DEVICE(0x2C7C, 0x0700) }, /* Quectel BG95/BG77/BG600L-M3/BC69 */   
	{ USB_DEVICE(0x2C7C, 0x0435) }, /* Quectel AG35 */   
	{ USB_DEVICE(0x2C7C, 0x0415) }, /* Quectel AG15 */   
	{ USB_DEVICE(0x2C7C, 0x0452) }, /* Quectel AG520R */   
	{ USB_DEVICE(0x2C7C, 0x0455) }, /* Quectel AG550R */   
	{ USB_DEVICE(0x2C7C, 0x0620) }, /* Quectel EG20 */   
	{ USB_DEVICE(0x2C7C, 0x0800) }, /* Quectel RG500Q/RM500Q/RG510Q/RM510Q */   
	#endif  

## 添加零包支持机制 ##
主机控制器要支持处理0包的能力。
在${KERNELDIR}/driver/usb/serial/usb_wwan.c中

	static struct urb *usb_wwan_setup_urb(struct usb_serial *serial, 
	int endpoint,        
	int dir, 
	void *ctx, 
	char *buf, 
	int len,
	void (*callback) (struct urb *))   
	{   
	……   
	usb_fill_bulk_urb(urb, serial->dev,   
	      usb_sndbulkpipe(serial->dev, endpoint)|dir,buf,len,callback,ctx);
	 #if 1   //Added by Quectel for zero packet
	     if (dir == USB_DIR_OUT) {   
	struct usb_device_descriptor *desc = &serial->dev->descriptor;   
	if (desc->idVendor == cpu_to_le16(0x2C7C))   
	urb->transfer_flags |= URB_ZERO_PACKET;   
	        }   
	#endif    
	return urb;   
	}  

## PM机制 ##
在${KERNELDIR}/driver/usb/serial/option.c中，

	static struct usb_serial_driver option_1port_device = {   
	……   
	#ifdef CONFIG_PM   
	   .suspend           = usb_wwan_suspend,   
	   .resume            = usb_wwan_resume,   
	#if 1  //Added by Quectel   
	   .reset_resume   = usb_wwan_resume,   
	#endif   
	#endif   
	};   

## 预留网络接口 ##
USB接口4一般用作网络设备，在${KERNELDIR}/driver/usb/serial/option.c中

	static int option_probe（  
	...  
	#if 1  //Added by Quectel   
	//Quectel modules’s interface 4 can be used as USB network device   
	       if (serial->dev->descriptor.idVendor == cpu_to_le16(0x2C7C)) {   
	       //some interfaces can be used as USB Network device (ecm, rndis, mbim)   
	       if (serial->interface->cur_altsetting->desc.bInterfaceClass != 0xFF) {   
	                       return -ENODEV;   
	               }
	       //interface 4 can be used as USB Network device (qmi)   
	       else if (serial->interface->cur_altsetting->desc.bInterfaceNumber >= 4) {
	                       return -ENODEV;   
	               }   
	       }   
	#endif   


## 内核编译配置 ##

检查内核编译对应.config中是否有以下选项
USB串口驱动相关的配置项：  

	CONFIG_USB_SERIAL=y  
	CONFIG_USB_SERIAL_OPTION=y 

## 模块源码编译 ##

对Ubuntu PC，或内核中option驱动编译成内核模块，即 <font color="red">*CONFIG_USB_SERIAL_OPTION=m* </font> 的情况，Quectel提供了可以直接在Ubuntu PC上编译安装的USB串口驱动的源码。如<font color="red">*Quectel_Linux_USB_Serial_Option_Driver_20200720.tgz* </font> ，解压可以得到从内核2.6到最新内核版本的USB串口驱动，在Ubuntu PC上，直接make && sudo make install 即成功。对嵌入式系统需要交叉编译的情况，需要修改Makefile，编译得到的option.ko拷贝到目标平台的/lib/modules/xxx/ 目录中。
