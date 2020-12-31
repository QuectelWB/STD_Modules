USB零包
=====

QFirehose 升级，报错

	inf[0] ep_in -1/1024, errno = 145 (Connection timed out), timeout=600
	qusb_noblock_read read=0, errno=145 (Connection timed out)

Carl 大神说，需要添加零包支持。


about Zero-length packet
------

[What is zero-length packet?](https://stackoverflow.com/questions/48975323/what-is-zero-length-packet)

Both of the terminologies are used for transaction completion mechanism. The maximum size of data packets can be derived from bMaxPacketSize stored in Endpoint descriptors. When data packet is less than that, its called a Short Packet. Similarly when you can send a data packet with length Zero.


