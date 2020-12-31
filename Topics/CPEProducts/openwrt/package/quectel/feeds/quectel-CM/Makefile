include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:= quectel-CM
PKG_RELEASE:=1
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/quectel-CM
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=quectel-CM app building test
endef

define Package/quectel-CM/description
	quectel-CM app building test
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ${TOPDIR}/package/quectel/src/${PKG_NAME}/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS)"
endef

define Package/quectel-CM/install
		$(INSTALL_DIR) $(1)/usr/bin $(1)/lib/netifd/proto $(1)/lib/netifd
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/quectel-CM $(1)/usr/bin
		$(INSTALL_BIN) ./files/rmnet_init.sh $(1)/usr/bin
		$(INSTALL_BIN) ./files/rmnet.sh $(1)/lib/netifd/proto
		$(INSTALL_BIN) ./files/rmnet.script $(1)/lib/netifd
		$(INSTALL_BIN) ./files/rmnet6.sh $(1)/lib/netifd/proto
		$(INSTALL_BIN) ./files/rmnet6.script $(1)/lib/netifd
endef

$(eval $(call BuildPackage,quectel-CM))
