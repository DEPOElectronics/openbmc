require include/image-common.inc

IMAGE_FEATURES_remove += " \
  obmc-bmc-state-mgmt \
  obmc-chassis-mgmt \
  obmc-chassis-state-mgmt \
  obmc-fan-control \
  obmc-fan-mgmt \
  obmc-flash-mgmt \
  obmc-host-ctl \
  obmc-host-state-mgmt \
  obmc-host-ipmi \
  obmc-inventory \
  obmc-leds \
  obmc-logging-mgmt \
  obmc-remote-logging-mgmt \
  obmc-net-ipmi \
  obmc-network-mgmt \
  obmc-sensors \
  obmc-debug-collector \
  obmc-software \
  obmc-system-mgmt \
  obmc-user-mgmt \
  obmc-settings-mgmt \
  "

CORE_IMAGE_EXTRA_INSTALL_remove += " \
  packagegroup-obmc-apps-extras \
  "
