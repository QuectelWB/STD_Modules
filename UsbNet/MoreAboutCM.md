CM is short for quectel-CM, or Quectel ConnectManager.

quectel-CM is the recommended way for the RMNET interface modules to set up datacall.



``` sequence
title: the QMI between modules and the Host AP
participant Host AP
participant Quectel Modules

Host AP -> Quectel Modules : QMICTL_SYNC_REQ
Quectel Modules-> Host AP : 是啊
Host AP -> Quectel Module : 
Host AP -> Quectel Modules : 哈，小王！\n最近身体怎么样了？
Quectel Modules -> Host AP : 还可以吧
```
