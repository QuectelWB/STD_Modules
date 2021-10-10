# SMP IRQ affinity

APIC表示高级可编程中断控制器（Advanced Programmable Interrupt Controlle）,APIC是SMP体系的核心，通过APIC可以将中断分发到不同的CPU来处理.即将中断绑定CPU或者把中断处理平摊到CPU上，这个过程叫做 SMP IRQ Affinity

使用前提

- 需要多CPU的系统，使用uname -a可以查询是否支持SMP
- 需要大于等于2.4的Linux 内核 ，使用uname -r可以查询内核版本
- 相关设置文件，设置/proc/irq/{IRQ}/smp_affinity和/proc/irq/{IRQ}/smp_affinity_list指定了哪些CPU能够关联到一个给定的IRQ源. 这两个文件包含了这些指定cpu的cpu位掩码(smp_affinity)和cpu列表(smp_affinity_list)
- 绑定硬件中断到CPU
- 手动绑定 IRQ 到不同 CPU，需要先停掉 IRQ 自动调节的服务进程，否则自己手动绑定做的更改将会被自动调节进程给覆盖掉。
- SMP_AFFINITY文件:在/proc/irq/{IRQ}/目录下都有一个smp_affinity文件，这个文件中，所表示的CPU核心以十六进制来表示的。

## 1、查找硬件IRQ号

[root@wmstianjin16172 ~]# cat /proc/interrupts 
           CPU0       CPU1 ...       CPU10      CPU11      
  0:        151          0 ...          0          0  IR-IO-APIC-edge      timer
  4:          1          0 ...          0          0  IR-IO-APIC-edge    
  8:         65          0 ...          0          0  IR-IO-APIC-edge      rtc0
  9:   14986870          0 ...          0          0  IR-IO-APIC-fasteoi   acpi
 16:         26          0 ...          0          0  IR-IO-APIC-fasteoi   ehci_hcd:usb1
 23:        711          0 ...          0          0  IR-IO-APIC-fasteoi   ehci_hcd:usb2
 48:          0          0 ...          0          0  DMAR_MSI-edge      dmar0
                            …………
 74:          0          0 ...          0          0  IR-PCI-MSI-edge      ahci
 75:          1          0 ...          0          0  IR-PCI-MSI-edge      eth0
 76:    1008671   86184098 ...   34556579          0  IR-PCI-MSI-edge      eth0-TxRx-0
 77:    1821647      29869 ...      63607          0  IR-PCI-MSI-edge      eth0-TxRx-1
 78:         10       8431 ...  211622276          0  IR-PCI-MSI-edge      eth0-TxRx-2
 79:         10          0 ...          0  360545261  IR-PCI-MSI-edge      eth0-TxRx-3
 80:   77837858  229674917 ...          0          0  IR-PCI-MSI-edge      eth0-TxRx-4
 81:  184905831   70950472 ...    4972306          0  IR-PCI-MSI-edge      eth0-TxRx-5
 82:  112830765   16260506 ...  128387722          0  IR-PCI-MSI-edge      eth0-TxRx-6
 83:         10          0 ...          0          0  IR-PCI-MSI-edge      eth0-TxRx-7
NMI:       1547        678 ...        493        698   Non-maskable interrupts
LOC:  424126712  457610969 ...  289672679  301760023   Local timer interrupts
SPU:          0          0 ...          0          0   Spurious interrupts
PMI:       1547        678 ...        493        698   Performance monitoring interrupts
IWI:          0          0 ...          0          0   IRQ work interrupts
RES:  387358708  463695594 ...  111027119   98766294   Rescheduling interrupts
CAL:       3009       5228 ...       5265       5278   Function call interrupts
TLB:    1432171    1441738 ...    3160227    2003518   TLB shootdowns
TRM:          0          0 ...          0          0   Thermal event interrupts
THR:          0          0 ...          0          0   Threshold APIC interrupts
MCE:          0          0 ...          0          0   Machine check exceptions
MCP:      22452      22452 ...      22452      22452   Machine check polls
ERR:          0            
MIS:          0            


## 2、修改/proc/irq/{IRQ}/smp_affinity
注意smp_affinity这个值是一个十六进制的bitmask，它和cpu No.序列的“与”运算结果就是将affinity设置在那个（那些）CPU了。（也即smp_affinity中被设置为1的位为CPU No.）
如：8个逻辑core，那么CPU#的序列为11111111 （从右到左依次为CPU0~CPU7）
如果cat /proc/irq/76/smp_affinity的值为：20（20是16进制对应的二进制为：00100000与11111111求与），则76这个IRQ的亲和性为#5号CPU。
每个IRQ的默认的smp affinity在这里：cat /proc/irq/default_smp_affinity
另外，cat /proc/irq/{IRQ}/smp_affinity_list(此文件为10进制)得到的即是CPU的一个List。

	[root@wmstianjin16172 ~]# cat /proc/irq/75/smp_affinity
	080
	[root@wmstianjin16172 ~]# cat /proc/irq/75/smp_affinity_list 
	7
	[root@wmstianjin16172 ~]# cat /proc/irq/78/smp_affinity
	400
	[root@wmstianjin16172 ~]# cat /proc/irq/78/smp_affinity_list 
	10

16进制080对应的2进制为10000000（快速算法一位16进制转换成4位2进制左边补0，即0000 1000 0000）所以对应为编号为8的CPU即CPU7（第8颗）
同理算400编号为11的CPU即CPU10

	[root@wmstianjin16172 ~]# cat /proc/irq/0/smp_affinity_list 
	0-11
	[root@wmstianjin16172 ~]# cat /proc/irq/0/smp_affinity
	fff


这是设置多颗CPU的样例：fff（1111 1111 1111）
修改的时候可以都好隔开或者-表示范围

	[root@wmstianjin16172 ~]# echo 3,8 > /proc/irq/78/smp_affinity_list
	[root@wmstianjin16172 ~]# echo 0-4 > /proc/irq/78/smp_affinity_list


## 网卡负载均衡

对单队列网卡而言，smp_affinity和smp_affinity_list配置多CPU是没有效果的。

- 但可以用RPS/RFS（Receive Packet Steering/Receive flow steering）来搞定：**RPS/RFS主要是针对单队列网卡多CPU环境**。虽然有这些虚拟队列的支撑，但是毕竟是软件模拟的，由于RPS只是单纯把数据包均衡到不同的cpu，这个时候如果应用程序所在的cpu和软中断处理的cpu不是同一个，此时对于cpu cache的影响会很大，那么RFS确保应用程序处理的cpu跟软中断处理的cpu是同一个，这样就充分利用cpu的cache

- smp_affinity主要针对 **多队列网卡多CPU环境**，如网卡支持多队列则可使用SMP irq affinity直接绑定硬中断，要是不支持多队列，那就用RPS解决网络软中断的负载均衡，即单个网卡的软中断分散到多个CPU处理，避免单个CPU负载过大导致性能瓶颈。

确定网卡是否支持多队列:

cat /proc/interrupts |grep eth0
 75:          1          0 ...          0          0  IR-PCI-MSI-edge      eth0
 76:    1008671   86184098 ...   34556579          0  IR-PCI-MSI-edge      eth0-TxRx-0
 77:    1821647      29869 ...      63607          0  IR-PCI-MSI-edge      eth0-TxRx-1
 78:         10       8431 ...  211622276          0  IR-PCI-MSI-edge      eth0-TxRx-2
 79:         10          0 ...          0  360545261  IR-PCI-MSI-edge      eth0-TxRx-3
 80:   77837858  229674917 ...          0          0  IR-PCI-MSI-edge      eth0-TxRx-4
 81:  184905831   70950472 ...    4972306          0  IR-PCI-MSI-edge      eth0-TxRx-5
 82:  112830765   16260506 ...  128387722          0  IR-PCI-MSI-edge      eth0-TxRx-6
 83:         10          0 ...          0          0  IR-PCI-MSI-edge      eth0-TxRx-7


## 使用总结

- 1、通过手动改变smp_affinity文件中的值来将IRQ绑定到指定的CPU核心上，或者启用irqbalance服务来自动绑定IRQ到CPU核心上。没有启动irqbalance 也没有合理的做手动 irq 绑定的话会有性能问题。手动 irq 只推荐给很 heavy、很特殊的情况，比如带多网卡多硬盘的网络存储服务器，一般机器一般应用还是用irqbalance 省心

- 2、在一个大量小包的系统上，irqbalance优化几乎没有效果，而且还使得cpu消耗分配的不均衡，导致机器性能得不到充分的利用，这个时候需要把它给结束掉。

- 3、对于文件服务器、高流量 Web 服务器这样的应用来说，把不同的网卡 IRQ 均衡绑定到不同的 CPU 上将会减轻某个 CPU 的负担，提高多个 CPU 整体处理中断的能力；

- 4、对于数据库服务器这样的应用来说，把磁盘控制器绑到一个 CPU、把网卡绑定到另一个 CPU 将会提高数据库的响应时间、优化性能。合理的根据自己的生产环境和应用的特点来平衡 IRQ 中断有助于提高系统的整体吞吐能力和性能。
- 5、DB可以设置一部分CPU处理硬盘IO，一部分CPU处理网络IO，多网卡服务器可以设置对应CPU对应网卡。当然具体情况看生产情况而定。

性能优化是一个过程而不是结果，需要大量的测试、监测以及持续的观察和改进。
