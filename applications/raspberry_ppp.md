ppp拨号
======

sudo apt-get install ppp

必要需要更换source_list

follow the readme.

the easist way: run quectel-pppd.sh

or

quectel-chat-connect quectel-chat-disconnect quectel-ppp 到/etc/ppp/peers

修改 quectel-ppp和quectel-chat-connect

最后执行 pppd call quectel-ppp &

pppd
---


