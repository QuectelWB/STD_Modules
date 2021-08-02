##
#    Copyright By Schips, All Rights Reserved
#    https://gitee.com/schips/
#    File Name:  make.sh
#    Created  :  Tue 24 Dec 2019 04:20:51 PM CST
##
#!/bin/sh
BASE=`pwd`
BUILD_HOST=arm-linux
OUTPUT=${BASE}/install/
make_dirs() {
    cd ${BASE}
    mkdir  compressed  install  source -p
    sudo ls
}

tget () { #try wget
    filename=`basename $1`
    echo "Downloading [${filename}]..."
    if [ ! -f ${filename} ];then
        wget $1
    fi

    echo "[OK] Downloaded [${filename}] "
}

download_package () {
    cd ${BASE}/compressed
    #下载包
    tget  https://udhcp.busybox.net/source/udhcp-0.9.8.tar.gz
}

tar_package () {
    cd ${BASE}/compressed
    ls * > /tmp/list.txt
    for TAR in `cat /tmp/list.txt`
    do
        tar -xf $TAR -C  ../source
    done
    rm -rf /tmp/list.txt
}

make_udhcp () {
    cd ${BASE}/source/udhcp*
    sed -i '5, 12{s/COMBINED_BINARY=/#COMBINED_BINARY=/}'  Makefile
    sed -i '130, 135{s/case INIT_SELECTING:/case  INIT_SELECTING:;/}' dhcpc.c
    make   CROSS_COMPILE=${BUILD_HOST}-
}

do_copy () {
    cd ${BASE}/source/udhcp*
    mkdir ${BASE}/install/udhcp -p
    mkdir ${BASE}/install/udhcp/sbin -p
    mkdir ${BASE}/install/udhcp/config -p
    cp ${BASE}/source/udhcp*/udhcpc   ${OUTPUT}/udhcp/sbin -v
    cp ${BASE}/source/udhcp*/udhcpd   ${OUTPUT}/udhcp/sbin -v
    # 默认的配置路径 /usr/share/udhcpc/default.script
    # 写进了代码中 dhcpc.c:62:#define DEFAULT_SCRIPT        "/usr/share/udhcpc/default.script"
    cp ${BASE}/source/udhcp*/samples/simple.script  ${OUTPUT}/udhcp/config/default.script -v
    #cp ${BASE}/source/udhcp*/samples/udhcpd.conf    ${OUTPUT}/udhcp/config/ -v
}

make_dirs
#download_package
tar_package
make_udhcp
do_copy

exit 0

#以下内容针对板子
#mkdir /usr/share/udhcpc/
#cp default.script /usr/share/udhcpc/default.script
#chmod +x /usr/share/udhcpc/default.script

