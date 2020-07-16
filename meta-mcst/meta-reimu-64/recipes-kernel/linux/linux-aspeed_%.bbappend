FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
             file://reimu-ast2400-64.cfg \
             file://aspeed-bmc-mcst-reimu-64.dts \
           "
