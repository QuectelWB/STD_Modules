#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#define LOG_TAG "AT_TEST"

#include <utils/Log.h>
#define MAX_FILE_NAME         256
#define DEVICE_NAME         "/dev/ttyUSB2"
#define SERIAL_PORT_SPEED       B115200
char strDeviceFile[MAX_FILE_NAME];
char strHelpString[MAX_FILE_NAME];
#define INIT_C_CC "\003\034\177\025\004\0\1\0\021\023\032\0\022\017\027\026\0"  
#define BUF_LEN 2048*2
static char buf[BUF_LEN];
static char buf_at[BUF_LEN];
static int fd_at = 0;
static struct termios tty_std_termios;

static const char CTL_START[]    = "ctl.start";
static const char CTL_STOP[]     = "ctl.stop";
// static const char RIL_DAEMON_NAME[]   = "ril-daemon";

//打开modem的对应的AT指令端口设备
static int at_open_dev(char *dev_name)
{
    int fd = -1;
  struct termios newtio;
  fd = open(dev_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0){
     printf ("Failed to open uart %s\n", dev_name);
     return -1;
  }

/*
  * Set up the standard termios.  Individual tty drivers may 
  * deviate from this; this is used as a template.
  */      
  bzero(&newtio, sizeof(newtio));
  memcpy(&newtio,&tty_std_termios,sizeof(newtio));    
  cfsetispeed(&newtio, SERIAL_PORT_SPEED);
  cfsetospeed(&newtio, SERIAL_PORT_SPEED);
  newtio.c_cflag = CS8 | CLOCAL | CREAD | HUPCL;          /* 8N1 */
  newtio.c_cflag &= ~CSIZE;               /* Mask the character size bits */
  newtio.c_cflag &= ~PARENB;
  newtio.c_cflag &= ~CSTOPB;
  newtio.c_cflag &= ~CRTSCTS;             /* disable hardware flow control */
  newtio.c_cflag |= SERIAL_PORT_SPEED;
  newtio.c_cflag |= CS8 | CLOCAL | CREAD | HUPCL ;           /* 8N1 */        
  newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* raw input */
  newtio.c_oflag &= ~OPOST;                          /* raw output */
  tcflush(fd, TCIFLUSH);                             /* clear input buffer */
  tcsetattr(fd, TCSANOW, &newtio);
  
  return fd;
}

/*
*服务的入口函数带两个参数，其中包含书包含需要操作的AT指令端口的路径
*/
int main(int argc, char* argv[])
{
    int len;
    int iRet = 0;
    int i;
    char AtBuf[BUF_LEN];
    struct termios newtio;  
    struct termios tty_std_termios;     
    /*
     * Set up the standard termios.  Individual tty drivers may 
     * deviate from this; this is used as a template.
     */
    memset(&tty_std_termios, 0, sizeof(struct termios));
    memcpy(tty_std_termios.c_cc, INIT_C_CC, NCCS);
    tty_std_termios.c_iflag = ICRNL | IXON;
    tty_std_termios.c_oflag = OPOST | ONLCR;
    tty_std_termios.c_cflag = SERIAL_PORT_SPEED | CS8 | CREAD | HUPCL |CRTSCTS;
    tty_std_termios.c_lflag = ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHOCTL |
        ECHOKE | IEXTEN;
    memset(strDeviceFile, 0x00, sizeof(MAX_FILE_NAME));
    memset(strHelpString, 0x00, sizeof(MAX_FILE_NAME));

  //根据不同的参数选择不同的设备名称
  if (argc < 2){
     strcpy (strDeviceFile, DEVICE_NAME);
    }else if(argc == 2){
       if(strstr((char*)argv[1], "-h") != NULL){
          LOGD("-------------------------------------------------\n");
          LOGD("Usage: \n");
          LOGD("./at_test_android -d ttyDevice\n");
          LOGD("1) Using /dev/ttyS0 device as default\n");
          LOGD("2) Example: ./at_test_android -d /dev/ttyUSB2\n");
          LOGD("-------------------------------------------------\n");
       }
       return 0;
    }else if (argc == 3){
        if(strstr((char*)argv[1], "-d") != NULL)
          strcpy (strDeviceFile, argv[2]);
        else
           return 0;
    }else{
        return 0;
    }
  fd_at = at_open_dev(strDeviceFile);
  if ( fd_at < 0 ){
     return 0;
}else{
    printf( "Open %s success\n", strDeviceFile);
}
// property_set(CTL_STOP, RIL_DAEMON_NAME);//停止掉rild服务
while (1){
   printf("Have entered while loop now!\n");
   memset(AtBuf, 0, BUF_LEN);
     memset(buf, 0, BUF_LEN);
     memset(buf_at, 0, BUF_LEN);
     len = 0;
     struct pollfd read_poll[2];
     read_poll[0].fd = fd_at;   //AT指令端口的ID
     read_poll[0].events = POLLIN;
     read_poll[0].revents = 0;
     read_poll[1].fd = 0;    //系统默认输入端口的ID
     read_poll[1].events = POLLIN;
     read_poll[1].revents = 0;
     printf("poll begin...\n");
     iRet = poll(read_poll, 2, -1);
     printf("poll end! iRet= %d\n", iRet);  
     printf("KKKKKKKKKKKKKKKKKKK   Have a poll again!   KKKKKKKKKKKKKKKKKKK\n\n");
     if (read_poll[0].revents & POLLIN){
        printf("got fd_at   poll\n");
        len = read(fd_at, buf_at, BUF_LEN - 10);
        if (len <= 0){
           printf("read nothing \n");
           continue;
        }
        buf_at[len] = 0;
        printf("read len= %d :", len);
        for (i = 0; i < len; i++)
           printf("%02x ", buf_at[i]);
        printf("\nThe read buf is: %s \n", buf_at);
     }
     if (read_poll[1].revents & POLLIN){ //从系统读入数据
        len = 0;
        printf("got 0 fd poll now\n");
        len = read(0, AtBuf, BUF_LEN - 10);
        AtBuf[--len] = 0;
        printf("Read len = %d\nYou have inputted: %s \n", len, AtBuf);
        if (len){
           printf("Read character over!\n");             
		   sprintf(AtBuf, "%s\r", AtBuf);
           len = strlen(AtBuf);
           if (strncmp(AtBuf, "exit", 4) == 0)
              goto kk_exit;
              usleep(100 * 1000);
              len = write(fd_at, AtBuf, len);   //发送数据到模组
              memset(AtBuf, 0, BUF_LEN);    
              printf("\nPlease input AT_CMD again:");
              len = 0;
            }
        }
    }
    kk_exit : 
    close(fd_at);
 // property_set(CTL_START, RIL_DAEMON_NAME);//重新开启rild服务
  return 0;
} 