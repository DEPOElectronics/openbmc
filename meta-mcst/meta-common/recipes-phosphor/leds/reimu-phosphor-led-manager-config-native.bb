SUMMARY = "Phosphor LED Group Management for REIMU"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit obmc-phosphor-utils
inherit native

PROVIDES += "virtual/phosphor-led-manager-config-native"
FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI += "file://led.yaml"
S = "${WORKDIR}"

do_install() {
    SRC=${S}
    DEST=${D}${datadir}/phosphor-led-manager
    install -D ${SRC}/led.yaml ${DEST}/led.yaml
}
