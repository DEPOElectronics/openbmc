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
  glibc-utils \
  mc mc-fish mc-helpers mc-helpers-perl mc-locale-ru \
  tcpdump \
  ethtool \
  "

IMAGE_FEATURES_remove = " \
  ssh-server-dropbear \
  "
