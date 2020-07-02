OBMC_IMAGE_EXTRA_INSTALL_append = " \
  active-gpio-on \
  flashrom \
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
  dscan \
  net-initial \
  initsc-lt \
  tcpdump \
  ethtool \
  mc mc-fish mc-helpers mc-helpers-perl mc-locale-ru \
  kernel-module-rtc-cy14b101p \
  "

IMAGE_FEATURES_remove = " \
  ssh-server-dropbear \
  phosphor-state-manager \
  obmc-bmc-state-mgmt \
  obmc-chassis-mgmt \
  obmc-chassis-state-mgmt \
  obmc-fan-control \
  obmc-fan-mgmt \
  obmc-flash-mgmt \
  obmc-host-ipmi \
  obmc-inventory \
  obmc-leds \
  obmc-logging-mgmt \
  obmc-remote-logging-mgmt \
  obmc-net-ipmi \
  obmc-sensors \
  obmc-debug-collector \
  bmcweb \
  obmc-software \
  obmc-system-mgmt \
  obmc-user-mgmt \
  obmc-settings-mgmt \
  "
