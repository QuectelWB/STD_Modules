USB ops and sysfs
=====

获取串口对应VID PID
----

	$ $ grep PRODUCT= /sys/bus/usb-serial/devices/ttyUSB0/../uevent
	PRODUCT=67b/2303/300

Disable & Enable USB from terminal
-----

参考

from https://www.linux.com/training-tutorials/disable-enable-usb-terminal/


*lsusb -t* 获取USB Hierarchy
	
	/:  Bus 05.Port 1: Dev 1, class=root_hub, Driver=uhci_hcd/2p, 12M
	/:  Bus 04.Port 1: Dev 1, class=root_hub, Driver=uhci_hcd/2p, 12M
	/:  Bus 03.Port 1: Dev 1, class=root_hub, Driver=uhci_hcd/2p, 12M
	|__ Port 1: Dev 2, If 0, class=HID, Driver=usbhid, 1.5M
	/:  Bus 02.Port 1: Dev 1, class=root_hub, Driver=uhci_hcd/2p, 12M
	|__ Port 1: Dev 6, If 0, class=vend., Driver=usbserial_generic, 12M
	|__ Port 1: Dev 6, If 1, class=vend., Driver=usbserial_generic, 12M
	|__ Port 1: Dev 6, If 2, class=vend., Driver=usbserial_generic, 12M
	|__ Port 1: Dev 6, If 3, class=stor., Driver=usb-storage, 12M
	|__ Port 2: Dev 3, If 0, class=HID, Driver=usbhid, 1.5M
	|__ Port 2: Dev 3, If 1, class=HID, Driver=usbhid, 1.5M
	/:  Bus 01.Port 1: Dev 1, class=root_hub, Driver=ehci_hcd/8p, 480M
	|__ Port 6: Dev 5, If 0, Class=bInterfaceClass 0x0e not yet handled, Driver=uvcvideo, 480M    
	|__ Port 6: Dev 5, If 1, Class=bInterfaceClass 0x0e not yet handled, Driver=uvcvideo, 480M
	
- After we know the actived usb that connect with our modem. Do this command
	
	echo 2-1 |sudo tee /sys/bus/usb/drivers/usb/unbind <- to make usb disable/turn off

这种情况相当于断开USB设备，同理


	echo 2-1 |sudo tee /sys/bus/usb/drivers/usb/bind <- to make usb enable/turn on


重置USB总线
---------

遇到一些问题，导致ttyUSB0~ttyUSB3 并非对应DM、GPS、AT、PPP的情况，包括

	接入了另外一个模组（VID/PID不同），无法区分
	接了USB串口，占用了ttyUSB0
	应用程序出错了。打开ttyUSBx出错，但未关闭ttyUSBx，设备重新枚举时，ttyUSBx资源未释放
	
参考 

[解决自动化测试设备掉线：软件方案](https://testerhome.com/topics/9172)

[How to Reset USB Device in Linux](https://blog.csdn.net/mirkerson/article/details/9047831)

<details>
<summary>reset.c</summary>
<pre><code>	

	/*重启usb硬件端口*/
	#include <stdio.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <sys/ioctl.h>
	#include <linux/usbdevice_fs.h>

	int main(int argc, char **argv)
	{
	    const char *filename;
	    int fd;
	    int rc;
	
	    if (argc != 2) {
	        fprintf(stderr, "Usage: usbreset device-filename\n");
	        return 1;
	    }
	    filename = argv[1];//表示usb的ID
	
	    fd = open(filename, O_WRONLY);
	    if (fd < 0) {
	        perror("Error opening output file");
	        return 1;
	    }
	
	    printf("Resetting USB device %s\n", filename);
	    rc = ioctl(fd, USBDEVFS_RESET, 0);//ioctl是设备驱动中，对I/O设备进行管理的函数
	    if (rc < 0) {
	        perror("Error in ioctl");
	        return 1;
	    }
	    printf("Reset successful\n");
	
	    close(fd);
	    return 0;
	}

</code></pre>
</details>

gcc -o reset reset.c
sudo ./reset /dev/bus/usb/006/002

可以看到设备重新枚举



虚拟机里的USB3.0
--------

[A] Vmware Virtuabox USB3.0

![](imgs/VMwareUSB3.0.jpg)


Virtualbox

[Virtualbox 使能usb3.0 的方法](https://techsviewer.com/how-to-install-virtualbox-extension-pack-and-enable-usb-3-0/)


![](imgs/enble-USB-3.0-in-VirtualBox.jpg)


:)
