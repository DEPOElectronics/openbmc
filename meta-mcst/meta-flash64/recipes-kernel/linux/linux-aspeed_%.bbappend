FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI += " \
    file://reimu-64.cfg \
    file://aspeed-bmc-mcst-video.dtsi \
    file://aspeed-bmc-mcst-flash64.dtsi \
    file://mcst-reimu64-flash-layout.dtsi \
    file://0001-usb-gadget-aspeed-fix-dma-map-failure.patch \
"
