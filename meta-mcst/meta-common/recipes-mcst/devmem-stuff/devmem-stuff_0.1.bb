SUMMARY = "Create /dev/mem and do several stuff with it"
DESCRIPTION = "Create /dev/mem and do several stuff with it"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit systemd

REQUIRED_DISTRO_FEATURES = "systemd"
RDEPENDS_${PN} += "systemd bash"
SYSTEMD_SERVICE_${PN} = "devmem-stuff.service"

SRC_URI = "file://devmem-stuff \
           file://devmem-stuff.service \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/libexec
  install -d ${D}${systemd_system_unitdir}
  install -m 755 devmem-stuff ${D}/libexec
  install -m 644 devmem-stuff.service ${D}${systemd_system_unitdir}
}

FILES_${PN} = "/libexec ${systemd_system_unitdir}"
