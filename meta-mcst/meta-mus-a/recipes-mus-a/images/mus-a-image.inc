OBMC_IMAGE_EXTRA_INSTALL_32MB = " \
  active-gpio-on \
  init-ifupdown \
  flashrom \
  buttonscripts \
  glibc-localedata-en-us \
  glibc-localedata-ru-ru \
  glibc-binary-localedata-en-us \
  glibc-binary-localedata-ru-ru \
  pv \
  wget \
  firmware-updater \
  i2c-tools \
  overheatd \
  rawi2ctool \
  dscan \
  netsetup \
  kernel-module-tinyspi \
  kernel-module-rtc-cy14b101p \
  initsc-lt \
  "

OBMC_IMAGE_EXTRA_INSTALL_64MB = " \
  ${OBMC_IMAGE_EXTRA_INSTALL_32MB} \
  glibc-utils \
  nginx \
  php-fpm \
  reimu \
  mc \
  ethtool \
  sqlite3 \
  "