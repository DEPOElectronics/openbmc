SUMMARY = "Read RTC from motherboard"
DESCRIPTION = "A tool for reading RTC from motherboard and updating AST RTC"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit systemd
inherit features_check

DEPENDS = "dtc"
REQUIRED_DISTRO_FEATURES = "systemd"
RDEPENDS_${PN} += "systemd bash busybox"
SYSTEMD_SERVICE_${PN} = "rtcread.service"

SRC_URI = "file://rtcread \
           file://rtcread.service \
           file://Makefile \
           file://find_i2c_device.c \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}${systemd_system_unitdir}
  install -d ${D}/libexec
  install -m 644 rtcread.service ${D}${systemd_system_unitdir}
  install -m 755 rtcread ${D}/libexec
  install -m 755 find_i2c_device ${D}/libexec
}

FILES_${PN} = "/libexec ${systemd_system_unitdir}"
