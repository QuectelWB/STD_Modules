CM is short for quectel-CM, or Quectel ConnectManager.

quectel-CM is the recommended way for the RMNET interface modules to set up datacall.



``` sequence
title: the QMI between modules and the Host AP
participant HostAP
participant QuectelModules

HostAP -> QuectelModules: QMICTL_SYNC_REQ
QuectelModules->HostAP: QMICTL_SYNC_RESP
HostAP->QuectelModule : ...
QuectelModules->HostAP: ...
HostAP->QuectelModules: QMIWDS_GET_PKT_SRVC_STATUS_REQ
QuectelModules->HostAP: QMIWDS_GET_PKT_SRVC_STATUS _RESP
```

detailed please refer to 
[kba-20190602_guide_to_use_qmap](https://knowledge.quectel.com/display/FAEinternalsharing/01-2020.02.11-driver-kba-20190602_guide_to_use_qmap)

This doc can be accessed by the Quectel.
