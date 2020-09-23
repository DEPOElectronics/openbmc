SUMMARY = "tmux with minicom for serial-over-lan"
DESCRIPTION = "tmux with minicom for serial-over-lan"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit systemd

REQUIRED_DISTRO_FEATURES = "systemd"
RDEPENDS_${PN} += "systemd bash minicom tmux"
SYSTEMD_SERVICE_${PN} = "tmux-logger.service"

SRC_URI = "file://sol.conf \
           file://minirc.dfl \
           file://start-tmux \
           file://tmux-logger.service \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/etc
  install -d ${D}/etc/minicom
  install -d ${D}/libexec
  install -d ${D}${systemd_system_unitdir}
  install -m 644 sol.conf ${D}/etc
  install -m 644 minirc.dfl ${D}/etc/minicom
  install -m 755 start-tmux ${D}/libexec
  install -m 644 tmux-logger.service ${D}${systemd_system_unitdir}
}

FILES_${PN} = "/etc /libexec ${systemd_system_unitdir}"
