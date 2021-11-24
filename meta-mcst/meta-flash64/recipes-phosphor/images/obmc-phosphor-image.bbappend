OBMC_KERNEL_MODULES_64M = " \
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


OBMC_IMAGE_EXTRA_INSTALL:append += " \
  perf \
  tcpdump \
  ethtool \
  iptables \
  obmc-phosphor-buttons-signals \
  obmc-phosphor-buttons-handler \
  reimu-power-control \
  hwmon-instantiator \
  phosphor-sel-logger \
  rtcread \
  "
