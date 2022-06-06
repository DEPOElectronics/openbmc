SUMMARY = "Init BMC MAC address"
DESCRIPTION = "Setup BMC MAC address read from FRU"
PR = "r1"

LICENSE = "Apache-2.0"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit systemd
inherit obmc-phosphor-systemd

DEPENDS = "systemd"
RDEPENDS:${PN} = "bash"

FILESEXTRAPATHS:append := "${THISDIR}/${PN}:"

SRC_URI += " \
            file://update_mac.sh \
           "

SYSTEMD_SERVICE:${PN} = "update_mac.service"

do_install:append() {
    install -d ${D}/${bindir}
    install -m 755 ${WORKDIR}/update_mac.sh ${D}/${bindir}
}
