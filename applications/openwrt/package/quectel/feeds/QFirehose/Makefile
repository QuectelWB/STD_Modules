include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:= QFirehose
PKG_RELEASE:=1
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/QFirehose
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=QFirehose app building test
endef

define Package/QFirehose/description
	QFirehose app building test
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ${TOPDIR}/package/quectel/src/${PKG_NAME}/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS)"
endef

define Package/QFirehose/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/QFirehose $(1)/usr/bin/
endef

$(eval $(call BuildPackage,QFirehose))

