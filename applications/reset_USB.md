# ttyUSB序号的问题解决方法 #

遇到一些问题，导致ttyUSB0~ttyUSB3 并非对应DM、GPS、AT、PPP的情况，包括

	接入了另外一个模组（VID/PID不同），无法区分
	接了USB串口，占用了ttyUSB0
	应用程序出错了。打开ttyUSBx出错，但未关闭ttyUSBx，设备重新枚举时，ttyUSBx资源未释放
	
重置USB总线
---------

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

ubind
----


echo '2-1.6' | sudo tee /sys/bus/usb/drivers/usb/unbind



