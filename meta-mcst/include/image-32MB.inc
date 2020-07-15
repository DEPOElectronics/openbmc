require include/modules.inc

OBMC_IMAGE_EXTRA_INSTALL_append = " \
  active-gpio-on \
  allow-host-boot \
  devmem-stuff \
  profile-d \
  openssh \
  buttonscripts \
  glibc-localedata-en-us \
  glibc-localedata-ru-ru \
  glibc-binary-localedata-en-us \
  glibc-binary-localedata-ru-ru \
  pv \
  wget \
  firmware-updater \
  i2c-tools \
  rawi2ctool \
  net-initial \
  tcpdump \
  ethtool \
  mc mc-fish mc-helpers mc-helpers-perl mc-locale-ru \
  "

IMAGE_FEATURES_remove = " \
  ssh-server-dropbear \
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

PACKAGE_EXCLUDE = " \
  phosphor-state-manager \
  "

