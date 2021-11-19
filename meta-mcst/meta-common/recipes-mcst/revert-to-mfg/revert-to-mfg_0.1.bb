SUMMARY = "Revert to manufacturer defaults subsystem"
DESCRIPTION = "Revert to manufacturer defaults subsystem"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

RDEPENDS:${PN} += "bash systemd"

inherit systemd
inherit features_check

REQUIRED_DISTRO_FEATURES = "systemd"
SYSTEMD_SERVICE_${PN} = "revert-to-mfg.service"

SRC_URI = "file://revert-to-mfg \
           file://revert-to-mfg.service \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/libexec
  install -d ${D}${systemd_system_unitdir}
  install -m 755 revert-to-mfg ${D}/libexec
  install -m 644 revert-to-mfg.service ${D}${systemd_system_unitdir}
}

FILES:${PN} = "/libexec ${systemd_system_unitdir}"
