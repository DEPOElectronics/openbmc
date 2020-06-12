SUMMARY = "Compound init script for MUS-A specifics"
DESCRIPTION = "Compound init script for MUS-A specifics"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS += "update-rc.d-native"
RDEPENDS_${PN} += "bash"

SRC_URI = "file://initsc.sh \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}${sysconfdir}/init.d
  install -d ${D}${sysconfdir}/rcS.d
  install -m 755 initsc.sh ${D}${sysconfdir}/init.d/initsc.sh
  update-rc.d -r ${D} initsc.sh start 99 S .
}

FILES_${PN} = " ${sysconfdir} "
