Linux Shell

### Nohup

nohup - run a command immune to hangups, with output to a non-tty

Run COMMAND, ignoring hangup signals.
--help
display this help and exit
--version
output version information and exit
If standard input is a terminal, redirect it from an unreadable file. If standard output is a terminal, append output to 'nohup.out' if possible, '$HOME/nohup.out' otherwise. If standard error is a terminal, redirect it to standard output. To save output to FILE, use 'nohup COMMAND > FILE'.

NOTE: your shell may have its own version of nohup, which usually supersedes the version described here. Please refer to your shell's documentation for details about the options it supports.

detailed info see also [ https://www.gnu.org/software/coreutils/nohup](https://www.gnu.org/software/coreutils/manual/html_node/nohup-invocation.html#nohup-invocation)


### get the IPv4 addr


	m@m:~$ ifconfig wlan0
	wlan0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
	        inet 10.66.125.234  netmask 255.255.248.0  broadcast 10.66.127.255
	        inet6 fe80::3fe6:3aa2:1250:9af6  prefixlen 64  scopeid 0x20<link>
	        ether 28:c2:dd:1e:fa:8f  txqueuelen 1000  (ÒÔÌ«Íø)
	        RX packets 1895832  bytes 447757542 (447.7 MB)
	        RX errors 0  dropped 10741  overruns 0  frame 0
	        TX packets 34708  bytes 6409732 (6.4 MB)
	        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

	m@m:~$ ifconfig wlan0 | grep "inet " | awk '{print $2}'
	10.66.125.234
	m@m:~$ ifconfig wlan0 | grep "inet " | awk '{print $4}'
	255.255.248.0


### ping with timestamp

	1. ping 115.239.211.112 | awk '{ print $0"\t" strftime("%Y-%m-%d %H:%M:%S",systime())}' 
	2. ping 115.239.211.112 | awk '{ print $0"\t" strftime("%Y-%m-%d %H:%M:%S",systime()); fflush()}' >> outIP.info &  
	3. 

So, this also can be used for dmesg log.

Something else
On windows

	: top
	set  BACKUPDATE=% date :~0,4%% date :~5,2%% date :~8,2%  #   system year month date
	echo  % time % >> flogin.aspx_%BACKUPDATE%log.txt
	ping  -n 1 www.baidu.com | findstr  "TTL"  >> flogin.aspx_%BACKUPDATE%log.txt
	goto  top
 
	: top
	echo  % time % >> gateway_%BACKUPDATE%log.txt    
	ping  -n 1 127.0.0.1 | findstr  "TTL"  >> gateway_%BACKUPDATE%log.txt    
	goto  top

#### crontab

crontab  is  the program used to install, deinstall or list the tables used to drive the cron(8) daemon in Vixie Cron.  Each user can have their own crontab, and though these are files in /var/spool/cron/crontabs, they are not intended to be edited directly.

If the /etc/cron.allow file exists, then you must be listed (one user per line) therein in order to be allowed to use  this  command.   If the  /etc/cron.allow file does not exist but the /etc/cron.deny file does exist, then you must not be listed in the /etc/cron.deny file in order to use this command.

If neither of these files exists, then depending on site-dependent configuration parameters, only the super user will be  allowed  to  use this command, or all users will be able to use this command.

If both files exist then /etc/cron.allow takes precedence.  Which means that /etc/cron.deny is not considered and your user must be listed in /etc/cron.allow in order to be able to use the crontab.

	v@WB:~/GithubPages/QuectelWB.github.io$ crontab --help
	crontab: invalid option -- '-'
	crontab: usage error: unrecognized option
	usage:  crontab [-u user] file
	        crontab [ -u user ] [ -i ] { -e | -l | -r }
	                (default operation is replace, per 1003.2)
	        -e      (edit user's crontab)
	        -l      (list user's crontab)
	        -r      (delete user's crontab)
	        -i      (prompt before deleting user's crontab)


See also 
[Crontab](https://www.runoob.com/linux/linux-comm-crontab.html)

### get the PLMN

Through "AT+QWINFO"

	echo -ne "at+qnwinfo\r" | busybox microcom -t 3 /dev/ttyUSB2 | grep QNWINFO | cut -d, -f2
	
	return "46000"
	
	echo -ne "at+qnwinfo\r" | busybox microcom -t 3 /dev/ttyUSB2 | grep QNWINFO | cut -d, -f2 | sed -r 's/.*"(.+)".*/\1/'
	
	return 46000


Through "AT+CIMI"

	echo -ne "at+cimi\r" | busybox microcom -t 3 /dev/ttyUSB2 | awk 'NR==2 {print}'

	460115468155029
	


	echo -ne "at+cimi\r" |busybox microcom -t 3 /dev/ttyUSB2 |awk 'NR==2 {print}' |cut -b 1-5
	
	46011


### 
