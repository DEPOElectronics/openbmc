SUMMARY = "Button scripts for MCST management system"
DESCRIPTION = "Button scripts for MCST management system"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit systemd

REQUIRED_DISTRO_FEATURES = "systemd"
SYSTEMD_SERVICE_${PN} = "host-poweroff-hard.service host-poweroff.service host-poweron.service host-reset.service"

SRC_URI = " \
            file://LICENSE \
            file://server_pwrbut_h \
            file://server_pwrbut_s \
            file://server_reset \
            file://server_uid \
            file://server_watchdog_reset \
            file://host-poweroff-hard.service \
            file://host-poweroff.service \
            file://host-poweron.service \
            file://host-reset.service \
          "

S = "${WORKDIR}"

do_install() {
  dst="${D}/usr/bin"
  install -d $dst
  install -d ${D}${systemd_system_unitdir}
  install -m 755 server_pwrbut_h ${dst}
  install -m 755 server_pwrbut_s ${dst}
  install -m 755 server_reset ${dst}
  install -m 755 server_uid ${dst}
  install -m 755 server_watchdog_reset ${dst}
  install -m 644 ${S}/host-poweroff-hard.service ${D}${systemd_system_unitdir}
  install -m 644 ${S}/host-poweroff.service ${D}${systemd_system_unitdir}
  install -m 644 ${S}/host-poweron.service ${D}${systemd_system_unitdir}
  install -m 644 ${S}/host-reset.service ${D}${systemd_system_unitdir}
}

FILES_${PN} = "/usr/bin ${systemd_system_unitdir}"

RDEPENDS_${PN} += "bash gpio-funcs reimu-conf systemd"
