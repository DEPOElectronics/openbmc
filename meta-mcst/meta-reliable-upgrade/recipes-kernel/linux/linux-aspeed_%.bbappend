FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI += " \
    file://aspeed-bmc-mcst-additional-flash.dtsi \
    file://openbmc-flash-layout-64-secondary.dtsi \
    "
