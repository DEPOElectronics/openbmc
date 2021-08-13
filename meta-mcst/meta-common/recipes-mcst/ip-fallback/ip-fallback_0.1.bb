SUMMARY = "Fallback IP address on DHCP unavailable"
DESCRIPTION = "Fallback IP address on DHCP unavailable"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

RDEPENDS_${PN} += "bash systemd"

inherit systemd
inherit features_check

REQUIRED_DISTRO_FEATURES = "systemd"
SYSTEMD_SERVICE_${PN} = "ip_fallback.service"

SRC_URI = "file://ip_fallback \
           file://ip_fallback.service \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/libexec
  install -d ${D}${systemd_system_unitdir}
  install -m 755 ip_fallback ${D}/libexec
  install -m 644 ip_fallback.service ${D}${systemd_system_unitdir}
}

FILES_${PN} = "/libexec ${systemd_system_unitdir}"
