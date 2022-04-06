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
SYSTEMD_SERVICE_${PN} = "ip-fallback.service"

SRC_URI = "file://ip-fallback \
           file://ip-fallback.service \
           file://fallback-eth0.conf \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/etc/ip-fallback/
  install -d ${D}/libexec
  install -d ${D}${systemd_system_unitdir}
  install -m 755 ip-fallback ${D}/libexec
  install -m 644 ip-fallback.service ${D}${systemd_system_unitdir}
  install -m 644 fallback-eth0.conf ${D}/etc/ip-fallback/
}

FILES_${PN} = "/etc/ip-fallback /libexec ${systemd_system_unitdir}"
