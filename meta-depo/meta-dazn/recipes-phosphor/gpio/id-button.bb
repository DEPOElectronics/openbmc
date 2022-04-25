SUMMARY = "Romulus ID Button pressed application"
PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit obmc-phosphor-systemd

DEPENDS += "virtual/obmc-gpio-monitor"
RDEPENDS:${PN} += "virtual/obmc-gpio-monitor"

S = "${WORKDIR}"
SRC_URI += "file://toggle_identify_led.sh"

do_install() {
        install -d ${D}${bindir}
        install -m 0755 ${WORKDIR}/toggle_identify_led.sh \
            ${D}${bindir}/toggle_identify_led.sh
}

SYSTEMD_SERVICE:${PN} += "id-button-pressed.service"
