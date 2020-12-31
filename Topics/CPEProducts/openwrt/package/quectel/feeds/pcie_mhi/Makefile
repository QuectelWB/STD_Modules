include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=pcie_mhi
PKG_RELEASE:=1

#include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_VERSION:=1.0
endif

QCA_MC_SNOOPING_MAKE_OPTS:= \
        CROSS_COMPILE=$(KERNEL_CROSS) \
        ARCH=$(LINUX_KARCH) \
        KERNELPATH=$(LINUX_SRC_DIR) \
        KBUILDPATH=$(LINUX_DIR) \
        KERNELRELEASE=$(LINUX_RELEASE) \
        MDIR=$(PKG_BUILD_DIR)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/pcie_mhi
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Network Support
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros, Inc.
  TITLE:=Quectel Linux PCIE MHI Driver
  DEPENDS:=
  KCONFIG:=
  FILES:=$(PKG_BUILD_DIR)/pcie_mhi.ko
  AUTOLOAD:=$(call AutoLoad,41,pcie_mhi)
endef

define KernelPackage/pcie_mhi/description
  Quectel Linux PCIE MHI Driver
endef

define Build/InstallDev
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ${TOPDIR}/package/quectel/src/${PKG_NAME}/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(LINUX_DIR) M=$(PKG_BUILD_DIR) $(strip $(QCA_MC_SNOOPING_MAKE_OPTS)) \
		EXTRA_CFLAGS="-I$(STAGING_DIR)/usr/include/qca-nss-drv $(EXTRA_CFLAGS)"
endef

$(eval $(call KernelPackage,pcie_mhi))
