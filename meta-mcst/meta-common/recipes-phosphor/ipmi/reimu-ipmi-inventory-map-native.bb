SUMMARY = "REIMU IPMI to DBus Inventory mapping."
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

inherit phosphor-ipmi-fru
inherit native

SRC_URI += "file://config.yaml"

PROVIDES += "virtual/phosphor-ipmi-fru-inventory"

S = "${WORKDIR}"

do_install() {
        DEST=${D}${config_datadir}
        install -d ${DEST}
        install config.yaml ${DEST}
}
