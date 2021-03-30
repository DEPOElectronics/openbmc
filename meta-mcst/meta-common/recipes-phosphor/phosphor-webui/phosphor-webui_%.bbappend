FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI += " \
             file://0001-remove-unused-things.patch \
             file://0002-power-transitions.patch \
             file://0003-add-Domains-support.patch \
             file://0004-fix-DHCPEnabled.patch \
             file://0005-correct-DHCP-enable-disable-order.patch \
           "
