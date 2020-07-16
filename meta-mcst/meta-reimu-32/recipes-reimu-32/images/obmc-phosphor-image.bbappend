require include/image-32MB.inc

OBMC_IMAGE_EXTRA_INSTALL_append = " \
  overheatd \
  kernel-module-tinyspi \
  "
