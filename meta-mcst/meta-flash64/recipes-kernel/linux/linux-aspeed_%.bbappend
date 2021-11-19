FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI += " \
    file://reimu-64.cfg \
    file://aspeed-bmc-mcst-flash64.dtsi \
    file://mcst-reimu64-flash-layout.dtsi \
"
