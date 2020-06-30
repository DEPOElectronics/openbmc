SUMMARY = "Overheat check daemon"
DESCRIPTION = "Overheat check daemon"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS += "update-rc.d-native"

RDEPENDS_${PN} += "bash rawi2ctool gpio-funcs"

SRC_URI = " \
            file://LICENSE \
            file://overheatd_gpio \
            file://setled_reimu.sh \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/etc/init.d
  install -d ${D}/usr/bin
  install -m 755 ${S}/overheatd_gpio ${D}/etc/init.d/overheatd
  install -m 755 ${S}/setled_reimu.sh ${D}/usr/bin/setled
  update-rc.d -r ${D} overheatd defaults
}

FILES_${PN} = "/etc /usr/bin"
