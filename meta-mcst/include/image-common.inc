OBMC_KERNEL_MODULES = " \
  kernel-module-ip6table-filter \
  kernel-module-ip6table-mangle \
  kernel-module-ip6-tables \
  kernel-module-ip6t-ipv6header \
  kernel-module-ip6t-reject \
  kernel-module-iptable-filter \
  kernel-module-iptable-mangle \
  kernel-module-iptable-nat \
  kernel-module-ip-tables \
  kernel-module-ipt-reject \
  kernel-module-rng-core \
  kernel-module-timeriomem-rng \
  kernel-module-x-tables \
  kernel-module-xt-addrtype \
  kernel-module-xt-conntrack \
  kernel-module-xt-log \
  kernel-module-xt-mark \
  kernel-module-xt-masquerade \
  kernel-module-xt-nat \
  kernel-module-xt-nflog \
  kernel-module-xt-state \
  kernel-module-xt-tcpmss \
  kernel-module-xt-tcpudp \
  "

OBMC_IMAGE_EXTRA_INSTALL_append = " \
  ${OBMC_KERNEL_MODULES} \
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
  tcpdump \
  triggerhappy \
  ethtool \
  mc mc-fish mc-helpers mc-helpers-perl mc-locale-ru \
  overheatd \
  kernel-module-tinyspi \
  "

IMAGE_FEATURES_remove = " \
  ssh-server-dropbear \
  "
