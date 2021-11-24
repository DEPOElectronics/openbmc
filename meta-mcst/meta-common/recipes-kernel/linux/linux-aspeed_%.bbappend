FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
             file://reimu.cfg \
             file://aspeed-bmc-mcst-ast2500.dtsi \
           "
