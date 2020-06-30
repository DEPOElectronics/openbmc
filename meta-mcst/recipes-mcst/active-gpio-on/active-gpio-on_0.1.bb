SUMMARY = "Activate/deactivate ACTIVE# gpio"
DESCRIPTION = "Activate/deactivate ACTIVE# gpio line, which indicates that manager is ready"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS += "update-rc.d-native"

SRC_URI = " \
            file://LICENSE \
            file://activeledd \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/etc/init.d
  install -m 755 ${S}/activeledd ${D}/etc/init.d
  update-rc.d -r ${D} activeledd defaults
}

FILES_${PN} = "/etc"

RDEPENDS_${PN} += "gpio-funcs"
