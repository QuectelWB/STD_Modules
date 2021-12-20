#更多关于Ubuntu

## 制作驱动的deb


因为提供的源码只有Makefile，没有autoconf

	make && make install 
	checkinstall -D --pkgname=color-compile --pkgversion=2014-12-20 --install=no  --pkgsource=../color-compile  # 制作deb


## DKMS

DynamicKernel ModuleSupport


