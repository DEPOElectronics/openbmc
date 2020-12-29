FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI += " \
             file://0001-set-required-gpios.patch \
             file://0002-avoid-using-system-libfdt.patch \
           "
