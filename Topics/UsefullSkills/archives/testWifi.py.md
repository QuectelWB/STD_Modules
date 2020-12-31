import serial #导入模块
import time
import serial.tools.list_ports
import re


DEBUG=1   # 1 for debug 




def GetRet(description):
    if(re.findall('.*OK.*',str(description))):
        print("yes")
        return 1
    else:
        print("no")
        return 0
    
def fail():
    print("failed\n")

def testWIFI(Pport,Pworkmode,Pmode,PSSID,Ppasswd,Pchannel):
    
    try:
        '''
        # this way to auto choose serial port
        port_list = list(serial.tools.list_ports.comports())
        print(port_list)
        portx=list(port_list[0])[0]
        '''
    #    portx="COM27"
    
        bps=115200
        portx=Pport
        timex=1   #超时设置,None：永远等待操作，0为立即返回请求结果，其他值为等待超时时间(单位为秒）
        
        ser=serial.Serial(portx,bps,timeout=timex)    # 打开串口，并得到串口对象
        if ser.isOpen():
            print("open\n")
        print(ser)

#if no echo need
        result=ser.write("ATE0\r\n".encode("ascii"))   # 写数据
        time.sleep(2)
        if DEBUG:
            time.sleep(1)
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)
            GetRet(rd)

        
        result=ser.write("ATI\r\n".encode("ascii"))   # 写数据
        if DEBUG:
            time.sleep(1)
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)
            GetRet(rd)
      
        result=ser.write("AT+COPS?\r\n".encode("ascii"))
        time.sleep(3)
        if DEBUG:
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)
            GetRet(rd)

        
        if(Pworkmode=="AP0"):
            workmode=0
        elif(Pworkmode=="STA"):
            workmode=1
        elif(Pworkmode=="AP0+STA"):
            workmode=2
        elif(Pworkmode=="AP0+AP1"):
            workmode=3
        else:
            print("workmode parameter error.\n")

        print("##########")
        result=ser.write(("AT+QWIFICFG=\"workmode\","+"\""+str(workmode)+"\""+"\r\n").encode("ascii"))  # Set work mode to single AP (AP0)
        if DEBUG:
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)
            time.sleep(1)
            GetRet(rd)

        
        result=ser.write(("AT+QWIFICFG=\"ssid\",0,\""+PSSID+"\""+"\r\n").encode("ascii"))     # Set the SSID of AP0 to Test_AP_Mode
        if DEBUG:
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)
            time.sleep(2)

        print("##########")
        result=ser.write(("AT+QWIFICFG=\""+str(Pchannel)+"\",5\r\n").encode("ascii"))  #Set the channel of AP0 to 5
        time.sleep(2)
        if DEBUG:    
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)
            time.sleep(1)


        
        result=ser.write("AT+QWIFICFG=\"mode\",0,4\r\n".encode("ascii"))  # Set the frequency mode of AP0 to 802.11b/g/n
        
        if DEBUG:
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)
            time.sleep(2)

        '''
        Set the authorization type of AP0 to WPA2 personal authentication,
        encryption mode to TKIP and AES
        and password to "1234567890"
        '''


        
        result=ser.write(("AT+QWIFICFG=\"auth\",0,3,2,\""+Ppasswd+"\""+"\r\n").encode("ascii"))
        if DEBUG:
            print("\n写总字节数:",result)
            rd = ser.read(50)
            print(rd)        
        time.sleep(5)


        result=ser.write("AT+QWIFI=1\r\n".encode("ascii"))
        if DEBUG:
            print("\n写总字节数:",result)
            time.sleep(1)
            rd = ser.read(50)
            print(rd)
            time.sleep(1)

        
        ser.close()                   #关闭串口

        
    except Exception as e:
        print("---异常---：",e)
        ser.close()


def main():
    port="COM27"
    workmode="AP0"
    mode=4            # 802.11b/g/n
    SSID="Test_AP_Mode"
    passwd="1234567890"
    channel=5
    testWIFI(port,workmode,mode,SSID,passwd,channel)

main()
