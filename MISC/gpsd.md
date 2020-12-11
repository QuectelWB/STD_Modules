gpsd
====

gpsd ist ein Service-Daemon, der einen oder mehrere GPS- oder AIS-Empfänger überwacht, die über serielle oder USB-Ports an einen Host-Computer angeschlossen sind, so dass alle Daten über Standort, Kurs und Geschwindigkeit der Sensoren auf dem TCP-Port 2947 des Host-Computers abgefragt werden können. Mit gpsd können mehrere standortbewusste Client-Anwendungen (z.B. Navigations- und Stationssoftware) den Zugriff auf die Empfänger ohne Konkurrenz oder Datenverlust teilen.

GPSD是在Linux下实现动态获取GPS数据并发布到网络的一个服务引擎，它将获得的数据转至2947端口，使得一个GPS设备获取的数据能够为多个GPS数据处理终端共享。 gpsd是一个GPS的守护进程的开源GIS软件，用以侦听来自GPS接收器的位置信息，并将这些位置信息转换成一种简化的格式。

On Debian OS


	sudo apt-get install gpsd gpsd-clients
	
假设/dev/ttyUSB1 输出GPS信息

自行编译

	tar -xzf gpsd-3.18.tar.gz
	cd gpsd-3.18
	scons && scons check && sudo scons udev-install



[树莓派上使用GPS](https://maker.pro/raspberry-pi/tutorial/how-to-use-a-gps-receiver-with-raspberry-pi-4)


[Downloads](https://pkgs.org/download/gpsd)

[SourceCode](http://download.savannah.gnu.org/releases/gpsd/)

[官网](https://www.berlios.de/software/gpsd/)



:)
