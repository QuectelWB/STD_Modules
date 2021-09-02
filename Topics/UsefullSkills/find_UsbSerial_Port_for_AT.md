#找发送AT口的串口

## udev建立软链接
略

##内核里的方法

	usb_serial_device_probe
		...
		if (dev_desc->idVendor == cpu_to_le16(0x10C4)) {
			__u16 idProduct = le16_to_cpu(dev_desc->idProduct);
			__u8 bInterfaceNumber = iface_desc->bInterfaceNumber;
			printk("[%s] idProduct = 0x%x : 0x%x\n",__func__,idProduct,bInterfaceNumber);
			if(bInterfaceNumber == 0x02)
				usb_serial_tty_driver->name = "Test";
			else
				usb_serial_tty_driver->name = "ttyUSB";
	}
	...

## 应用编程


