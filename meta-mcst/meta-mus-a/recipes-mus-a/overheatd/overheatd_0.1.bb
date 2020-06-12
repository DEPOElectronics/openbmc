SUMMARY = "Overheat check daemon"
DESCRIPTION = "Overheat check daemon"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

RDEPENDS_${PN} += "bash rawi2ctool tinyspi"

SRC_URI = " \
            file://LICENSE \
            file://overheatd \
            file://setled.sh \
          "

S = "${WORKDIR}"

do_install() {
  mkdir -p ${D}/etc/init.d ${D}/usr/local/bin ${D}/etc/rc{0,3,5,6}.d
  cp ${S}/overheatd ${D}/etc/init.d
  cp ${S}/setled.sh ${D}/usr/local/bin
  ln -fs ../init.d/overheatd ${D}/etc/rc0.d/K01overheatd
  ln -fs ../init.d/overheatd ${D}/etc/rc6.d/K01overheatd
  ln -fs ../init.d/overheatd ${D}/etc/rc3.d/S03overheatd
  ln -fs ../init.d/overheatd ${D}/etc/rc5.d/S03overheatd
}

FILES_${PN} = "/etc/init.d /etc/rc3.d /etc/rc5.d /etc/rc6.d /etc/rc0.d /usr/local/bin"
