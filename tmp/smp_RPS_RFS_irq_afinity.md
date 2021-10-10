
当一个硬件(如磁盘控制器或者以太网卡), 需要打断CPU的工作时, 它就触发一个中断. 该中断通知CPU发生了某些事情并且CPU应该放下当前的工作去处理这个事情. 为了防止多个设置发送相同的中断, Linux设计了一套中断请求系统, 使得计算机系统中的每个设备被分配了各自的中断号, 以确保它的中断请求的唯一性. 从2.4 内核开始, Linux改进了分配特定中断到指定的处理器(或处理器组)的功能. 这被称为SMP IRQ affinity, 它可以控制系统如何响应各种硬件事件. 允许你限制或者重新分配服务器的工作负载, 从而让服务器更有效的工作. 以网卡中断为例，在没有设置SMP IRQ affinity时， 所有网卡中断都关联到CPU0, 这导致了CPU0负载过高，而无法有效快速的处理网络数据包，导致了瓶颈。 通过SMP IRQ affinity， 把网卡多个中断分配到多个CPU上，可以分散CPU压力，提高数据处理速度。

不使用SMP IRQ affinity


使用SMP IRQ affinity


使用前提
需要多CPU的系统
需要大于等于2.4的Linux 内核
相关设置文件
1. /proc/irq/IRQ#/smp_affinity
/proc/irq/IRQ#/smp_affinity 和 /proc/irq/IRQ#/smp_affinity_list 指定了哪些CPU能够关联到一个给定的IRQ源. 这两个文件包含了这些指定cpu的cpu位掩码(smp_affinity)和cpu列表(smp_affinity_list). 不允许关闭所有CPU， 同时如果IRQ控制器不支持中断请求亲和(IRQ affinity)，则这些设置的值将保持不变(既关联到所有CPU). 设置方法如下
1
echo $bitmask > /proc/irq/IRQ #/smp_affinity
示例(把44号中断绑定到前4个CPU(CPU0-3)上面)
1
echo f > /proc/irq/44/smp_affinity
2. /proc/irq/IRQ#/smp_affinity_list
设置该文件取得的效果与/proc/irq/IRQ#/smp_affinity是一致的，它们两者是联动关系(既设置其中之一，另一个文件也随着改变), 有些系统可能没有该文件, 设置方法如下
1
echo $cpuindex1-$cpuindex2 > /proc/irq/IRQ #/smp_affinity_list
示例(把44号中断绑定到前4个CPU(CPU0-3)上面)
1
echo 0-3 > /proc/irq/44/smp_affinity_list
3. /proc/irq/default_smp_affinity
/proc/irq/default_smp_affinity 指定了默认情况下未激活的IRQ的中断亲和掩码(affinity mask).一旦IRQ被激活，它将被设置为默认的设置(即default_smp_affinity中记录的设置). 该文件能被修改. 默认设置是0xffffffff.
bitmask计算方法
首先我们来看看smp_affinity文件的内容

1
2
root@ hostname : /home/igi # cat /proc/irq/76/smp_affinity
ffffff
这个bitmask表示了76号中断将被路由到哪个指定处理器. bit mask转换成二进制后，其中的每一位代表了一个CPU. smp_affinity文件中的数值以十六进制显示。为了操作该文件，在设置之前我们需要把CPU位掩码从二进制转换到十六进制。

上面例子中每一个”f”代表了4个CPU的集合，最靠右边的值是最低位的意思。 以4个CPU的系统为例:

“f” 是十六进制的值， 二进制是”1111”. 二进制中的每个位代表了服务器上的每个CPU. 那么能用以下方法表示每个CPU
           二进制  十六进制
   CPU 0   0001    1
   CPU 1   0010    2
   CPU 2   0100    4
   CPU 3   1000    8
结合这些位掩码(简单来说就是直接对十六进制值做加法), 我们就能一次定位多个CPU。 例如， 我想同时表示CPU0和CPU2, bitmask结果就是:
           二进制       十六进制
   CPU 0    0001         1
 + CPU 2    0100         4
   -----------------------
   bitmask  0101         5
如果我想一次性表示所有4个CPU，bitmask结果是:
           二进制       十六进制
   CPU 0    0001         1
   CPU 1    0010         2
   CPU 2    0100         4
 + CPU 3    1000         8
   -----------------------
   bitmask  1111         f
假如有一个4个CPU的系统， 我们能给一个IRQ分配15种不同的CPU组合(实际上有16种，但我们不能给任何中断分配中断亲和为”0”的值， 即使你这么做，系统也会忽略你的做法)

对比测试
类别	测试客户端	测试服务端
型号	BladeCenter HS22	BladeCenter HS22
CPU	Xeon E5640	Xeon E5640
网卡	Broadcom NetXtreme II BCM5709S Gigabit Ethernet	Broadcom NetXtreme II BCM5709S Gigabit Ethernet
内核	2.6.38-2-686-bigmem	2.6.38-2-686-bigmem
内存	24GB	24GB
系统	Debian 6.0.3	Debian 6.0.3
驱动	bnx2	bnx2
客户端: netperf
服务端: netserver
测试分类: 不开启IRQ affinity和RPS/RFS, 单独开启IRQ affinity, 单独开启RPS/RFS，同时开启IRQ affinity和RPS/RFS, 不同分类设置值如下
不开启IRQ affinity和RPS/RFS

	/proc/irq/74/smp_affinity 00ffff
	/proc/irq/75/smp_affinity 00ffff
	/proc/irq/76/smp_affinity 00ffff
	/proc/irq/77/smp_affinity 00ffff
	/proc/irq/78/smp_affinity 00ffff
	/proc/irq/79/smp_affinity 00ffff
	/proc/irq/80/smp_affinity 00ffff
	/proc/irq/81/smp_affinity 00ffff

	/sys/class/net/eth0/queues/rx-0/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-1/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-2/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-3/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-4/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-5/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-6/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-7/rps_cpus 00000000

	/sys/class/net/eth0/queues/rx-0/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-1/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-2/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-3/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-4/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-5/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-6/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-7/rps_flow_cnt 0

	/proc/sys/net/core/rps_sock_flow_entries 0
	单独开启IRQ affinity
	/proc/irq/74/smp_affinity 000001
	/proc/irq/75/smp_affinity 000002
	/proc/irq/76/smp_affinity 000004
	/proc/irq/77/smp_affinity 000008
	/proc/irq/78/smp_affinity 000010
	/proc/irq/79/smp_affinity 000020
	/proc/irq/80/smp_affinity 000040
	/proc/irq/81/smp_affinity 000080

	/sys/class/net/eth0/queues/rx-0/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-1/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-2/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-3/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-4/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-5/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-6/rps_cpus 00000000
	/sys/class/net/eth0/queues/rx-7/rps_cpus 00000000

	/sys/class/net/eth0/queues/rx-0/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-1/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-2/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-3/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-4/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-5/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-6/rps_flow_cnt 0
	/sys/class/net/eth0/queues/rx-7/rps_flow_cnt 0

	/proc/sys/net/core/rps_sock_flow_entries 0
	单独开启RPS/RFS
	/proc/irq/74/smp_affinity 00ffff
	/proc/irq/75/smp_affinity 00ffff
	/proc/irq/76/smp_affinity 00ffff
	/proc/irq/77/smp_affinity 00ffff
	/proc/irq/78/smp_affinity 00ffff
	/proc/irq/79/smp_affinity 00ffff
	/proc/irq/80/smp_affinity 00ffff
	/proc/irq/81/smp_affinity 00ffff

	/sys/class/net/eth0/queues/rx-0/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-1/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-2/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-3/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-4/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-5/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-6/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-7/rps_cpus 0000ffff

	/sys/class/net/eth0/queues/rx-0/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-1/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-2/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-3/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-4/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-5/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-6/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-7/rps_flow_cnt 4096

	/proc/sys/net/core/rps_sock_flow_entries 32768
	同时开启IRQ affinity和RPS/RFS
	/proc/irq/74/smp_affinity 000001
	/proc/irq/75/smp_affinity 000002
	/proc/irq/76/smp_affinity 000004
	/proc/irq/77/smp_affinity 000008
	/proc/irq/78/smp_affinity 000010
	/proc/irq/79/smp_affinity 000020
	/proc/irq/80/smp_affinity 000040
	/proc/irq/81/smp_affinity 000080

	/sys/class/net/eth0/queues/rx-0/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-1/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-2/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-3/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-4/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-5/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-6/rps_cpus 0000ffff
	/sys/class/net/eth0/queues/rx-7/rps_cpus 0000ffff

	/sys/class/net/eth0/queues/rx-0/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-1/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-2/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-3/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-4/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-5/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-6/rps_flow_cnt 4096
	/sys/class/net/eth0/queues/rx-7/rps_flow_cnt 4096

/proc/sys/net/core/rps_sock_flow_entries 32768
测试方法: 每种测试类型执行3次，中间睡眠10秒, 每种测试类型分别执行100、500、1500个实例， 每实例测试时间长度为60秒

TCP_RR 1 byte: 测试TCP 小数据包 request/response的性能
1
netperf -t TCP_RR -H $serverip -c -C -l 60
UDP_RR 1 byte: 测试UDP 小数据包 request/response的性能
1
netperf -t UDP_RR -H $serverip -c -C -l 60
TCP_RR 256 byte: 测试TCP 大数据包 request/response的性能
1
netperf -t TCP_RR -H $serverip -c -C -l 60 -- -r256,256
UDP_RR 256 byte: 测试UDP 大数据包 request/response的性能
1
netperf -t UDP_RR -H $serverip -c -C -l 60 -- -r256,256
测试结果
TCP_RR 1 byte小包测试


TCP_RR 256 byte大包测试


UDP_RR 1 byte小包测试


UDP_RR 256 byte大包测试


 

CPU核负载的变化
以小数据包1500进程测试的CPU(除了明确指明类型，否则这里的负载都是TCP_RR测试的负载)负载数据为示例

不开启IRQ affinity和RPS/RFS： 软中断集中在第一个CPU上，导致了性能瓶颈
Average:     CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest   %idle
Average:     all    2.15    0.00   11.35    0.00    0.00    5.66    0.00    0.00   80.84
Average:       0    0.12    0.00    0.65    0.00    0.00   90.86    0.00    0.00    8.38
Average:       1    8.36    0.00   37.46    0.00    0.00    0.00    0.00    0.00   54.19
Average:       2    7.92    0.00   32.94    0.00    0.00    0.00    0.00    0.00   59.13
Average:       3    5.68    0.00   23.96    0.00    0.00    0.00    0.00    0.00   70.36
Average:       4    0.78    0.00    9.77    0.07    0.00    0.00    0.00    0.00   89.39
Average:       5    0.67    0.00    8.87    0.00    0.00    0.00    0.00    0.00   90.47
Average:       6    0.77    0.00    6.91    0.00    0.00    0.00    0.00    0.00   92.32
Average:       7    0.50    0.00    5.55    0.02    0.00    0.00    0.00    0.00   93.93
Average:       8    7.29    0.00   37.16    0.00    0.00    0.00    0.00    0.00   55.55
Average:       9    0.35    0.00    2.28    0.00    0.00    0.00    0.00    0.00   97.37
Average:      10    0.27    0.00    2.01    0.00    0.00    0.00    0.00    0.00   97.72
Average:      11    0.25    0.00    1.87    0.00    0.00    0.00    0.00    0.00   97.88
Average:      12    0.23    0.00    2.78    0.00    0.00    0.00    0.00    0.00   96.99
Average:      13    0.27    0.00    2.70    0.00    0.00    0.00    0.00    0.00   97.04
Average:      14    0.55    0.00    3.03    0.02    0.00    0.00    0.00    0.00   96.40
Average:      15    0.10    0.00    2.16    0.00    0.00    0.00    0.00    0.00   97.74
单独开启RPS/RFS: 软中断主要集中在CPU0上，但有少许分布在其他CPU上
Average:     CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest   %idle
Average:     all    4.65    0.00   34.71    0.01    0.00    7.37    0.00    0.00   53.26
Average:       0    0.02    0.00    0.37    0.00    0.00   99.40    0.00    0.00    0.22
Average:       1    5.60    0.00   38.33    0.02    0.00    2.02    0.00    0.00   54.04
Average:       2    5.57    0.00   37.33    0.03    0.00    1.01    0.00    0.00   56.05
Average:       3    5.11    0.00   36.28    0.00    0.00    0.73    0.00    0.00   57.88
Average:       4    4.78    0.00   37.16    0.00    0.00    2.05    0.00    0.00   56.01
Average:       5    4.45    0.00   37.46    0.00    0.00    0.97    0.00    0.00   57.12
Average:       6    4.34    0.00   36.25    0.00    0.00    0.97    0.00    0.00   58.45
Average:       7    4.28    0.00   35.87    0.00    0.00    0.97    0.00    0.00   58.88
Average:       8    5.47    0.00   37.11    0.02    0.00    1.27    0.00    0.00   56.13
Average:       9    5.58    0.00   37.59    0.03    0.00    2.13    0.00    0.00   54.66
Average:      10    5.98    0.00   37.04    0.00    0.00    1.22    0.00    0.00   55.76
Average:      11    4.99    0.00   34.58    0.00    0.00    0.99    0.00    0.00   59.44
Average:      12    4.46    0.00   37.47    0.00    0.00    0.99    0.00    0.00   57.09
Average:      13    4.83    0.00   36.97    0.00    0.00    1.37    0.00    0.00   56.83
Average:      14    4.60    0.00   38.06    0.00    0.00    1.53    0.00    0.00   55.81
Average:      15    4.36    0.00   37.26    0.00    0.00    1.45    0.00    0.00   56.92
单独开启IRQ affinity, TCP_RR测试: 软中断负载比较均匀的分散到前面8个CPU之上(与bitmask设置一致)
Average:     CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest   %idle
Average:     all    6.13    0.00   55.66    0.00    0.00   32.04    0.00    0.00    6.17
Average:       0    6.31    0.00   54.49    0.03    0.00    4.19    0.00    0.00   34.98
Average:       1    2.00    0.00   16.65    0.00    0.00   81.35    0.00    0.00    0.00
Average:       2    3.31    0.00   29.36    0.00    0.00   64.58    0.00    0.00    2.76
Average:       3    2.91    0.00   23.11    0.00    0.00   71.40    0.00    0.00    2.58
Average:       4    2.48    0.00   20.82    0.00    0.00   75.30    0.00    0.00    1.40
Average:       5    2.49    0.00   21.90    0.00    0.00   73.30    0.00    0.00    2.31
Average:       6    3.00    0.00   28.87    0.00    0.00   65.96    0.00    0.00    2.17
Average:       7    2.58    0.00   22.95    0.00    0.00   72.46    0.00    0.00    2.01
Average:       8    6.61    0.00   56.48    0.00    0.00    0.00    0.00    0.00   36.90
Average:       9    9.83    0.00   89.28    0.00    0.00    0.05    0.00    0.00    0.83
Average:      10    9.73    0.00   87.18    0.00    0.00    0.00    0.00    0.00    3.08
Average:      11    9.73    0.00   88.08    0.00    0.00    0.05    0.00    0.00    2.14
Average:      12    9.11    0.00   88.71    0.00    0.00    0.05    0.00    0.00    2.12
Average:      13    9.43    0.00   89.53    0.00    0.00    0.03    0.00    0.00    1.00
Average:      14    9.38    0.00   87.58    0.00    0.00    0.03    0.00    0.00    3.00
Average:      15    9.53    0.00   88.97    0.00    0.00    0.07    0.00    0.00    1.43
单独开启IRQ affinity, UDP_RR测试: 软中断负载不能均匀的分散到前面8个CPU之上，主要分布在CPU2上，导致了瓶颈，性能下降(下降原因请见本文测试的局限性部分)
Average:     CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest   %idle
Average:     all    1.05    0.00    8.21    0.01    0.00    5.83    0.00    0.00   84.90
Average:       0    3.10    0.00   23.25    0.00    0.00    0.33    0.00    0.00   73.31
Average:       1    2.73    0.00   21.15    0.00    0.00    0.37    0.00    0.00   75.75
Average:       2    0.72    0.00    4.93    0.00    0.00   92.35    0.00    0.00    2.00
Average:       3    1.27    0.00    9.81    0.00    0.00    0.10    0.00    0.00   88.82
Average:       4    0.15    0.00    1.87    0.10    0.00    0.07    0.00    0.00   97.82
Average:       5    0.07    0.00    1.04    0.00    0.00    0.07    0.00    0.00   98.82
Average:       6    0.02    0.00    0.53    0.00    0.00    0.03    0.00    0.00   99.42
Average:       7    0.00    0.00    0.23    0.00    0.00    0.00    0.00    0.00   99.77
Average:       8    0.23    0.00    1.96    0.00    0.00    0.00    0.00    0.00   97.81
Average:       9    0.08    0.00    0.60    0.00    0.00    0.00    0.00    0.00   99.32
Average:      10    8.38    0.00   65.07    0.00    0.00    0.00    0.00    0.00   26.56
Average:      11    0.00    0.00    0.36    0.00    0.00    0.00    0.00    0.00   99.64
Average:      12    0.03    0.00    0.23    0.00    0.00    0.00    0.00    0.00   99.73
Average:      13    0.00    0.00    0.19    0.00    0.00    0.00    0.00    0.00   99.81
Average:      14    0.00    0.00    0.12    0.00    0.00    0.00    0.00    0.00   99.88
Average:      15    0.02    0.00    0.10    0.00    0.00    0.00    0.00    0.00   99.88
同时开启IRQ affinity和RPS/RFS: 软中断负载比较均匀的分散到各个CPU之上
Average:     CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest   %idle
Average:     all    6.27    0.00   53.65    0.00    0.00   32.33    0.00    0.00    7.74
Average:       0    7.05    0.00   59.70    0.00    0.00   26.18    0.00    0.00    7.07
Average:       1    5.44    0.00   43.33    0.00    0.00   44.54    0.00    0.00    6.69
Average:       2    5.49    0.00   43.06    0.00    0.00   43.38    0.00    0.00    8.07
Average:       3    5.48    0.00   44.90    0.00    0.00   42.94    0.00    0.00    6.67
Average:       4    4.74    0.00   43.27    0.00    0.00   45.14    0.00    0.00    6.85
Average:       5    4.83    0.00   41.90    0.00    0.00   46.74    0.00    0.00    6.53
Average:       6    5.04    0.00   44.73    0.00    0.00   43.68    0.00    0.00    6.56
Average:       7    4.85    0.00   44.68    0.00    0.00   45.22    0.00    0.00    5.25
Average:       8    7.60    0.00   61.50    0.02    0.00   22.79    0.00    0.00    8.08
Average:       9    7.40    0.00   61.31    0.00    0.00   22.92    0.00    0.00    8.37
Average:      10    7.74    0.00   60.62    0.00    0.00   22.64    0.00    0.00    9.00
Average:      11    8.22    0.00   61.02    0.00    0.00   22.13    0.00    0.00    8.63
Average:      12    6.53    0.00   62.42    0.00    0.00   22.09    0.00    0.00    8.97
Average:      13    6.68    0.00   62.75    0.00    0.00   22.13    0.00    0.00    8.43
Average:      14    6.62    0.00   62.41    0.00    0.00   22.69    0.00    0.00    8.28
Average:      15    6.64    0.00   60.66    0.00    0.00   22.38    0.00    0.00   10.32
结果分析
1. TCP_RR TPS小数据包性能
单独开启RPS/RFS分别提升60%,145%,200%(依次对应100进程，500进程，1500进程,下同)
单独开启IRQ affinity分别提升135%, 343%, 443%
同时开启RPS/RFS和IRQ affinity分别提升148%, 346%, 372%
2. TCP_RR TPS大数据包性能
单独开启RPS/RFS分别提升76%,77%,89%
单独开启IRQ affinity分别提升79%, 77%, 88%
同时开启RPS/RFS和IRQ affinity分别提升79%, 77%, 88%
3. UDP_RR TPS小数据包性能
单独开启RPS/RFS分别提升44%,103%,94%
单独开启IRQ affinity性能分别下降11%, 8%, 8% (这是此次测试的局限造成, 详细分析见: 测试的局限性)
同时开启RPS/RFS和IRQ affinity分别提升65%, 130%, 137%
4. UDP_RR TPS小数据包性能
单独开启RPS/RFS分别提升55%,53%,61%
单独开启IRQ affinity性能分别下降5%, 4%, 4%
同时开启RPS/RFS和IRQ affinity分别提升55%, 51%, 53%
TCP 小数据包应用上，单独开启IRQ affinity能获得最大的性能提升，随着进程数的增加，IRQ affinity的优势越加明显.
UDP 小数据包方面，由于此次测试的局限，无法真实体现实际应用中的单独开启IRQ affinity而获得的性能提升, 但用RPS/RFS配合IRQ affinity，也能获得大幅度的性能提升;
TCP 大数据包应用上，单独开启IRQ affinity性能提升没有小数据包那么显著，但也有接近80%的提升, 基本与单独开启RPS/RFS的性能持平， 根据实验的数据计算所得，此时网卡流量约为88MB，还没达到千兆网卡的极限。
UDP 大数据包应用上，也是同样受测试局限性的影响，无法真实体现实际应用中的单独开启IRQ affinity而获得的性能提升, 但用RPS/RFS配合IRQ affinity，也能获得大幅度的性能提升
测试的局限性
对于UDP测试在IRQ affinity上性能的下降， 查阅了内核源码(drivers/net/bnx2.c)及资料, bnx2 网卡的RSS hash不支持对UDP的端口进行计算，从而导致单独启用IRQ affinity的时候(这时候由硬件进行hash计算), UDP的数据只被hash了IP地址而导致数据包的转发出现集中在某个CPU的现象. 这是此次测试的局限所在，由于测试只是一台服务器端及一台客户端，所有UDP的IP地址都相同，无法体现UDP性能在单独启用IRQ affinity的性能提升. 但RPS/RFS的hash计算不受硬件影响，故而能体现性能提升. 对于实际应用中，服务器与多台客户端交互的情形，应该不受bnx2的RSS hash影响(以上只是针对bnx2网卡的特定问题)


