SUMMARY = "Activate/deactivate ACTIVE# gpio"
DESCRIPTION = "Activate/deactivate ACTIVE# gpio line, which indicates that manager is ready"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS += "bash"

SRC_URI = " \
            file://LICENSE \
            file://activeledd \
          "

S = "${WORKDIR}"

do_install() {
  mkdir -p ${D}/etc/init.d ${D}/etc/rc{0,3,5,6}.d
  cp ${S}/activeledd ${D}/etc/init.d
  ln -fs ../init.d/activeledd ${D}/etc/rc0.d/K01activeledd
  ln -fs ../init.d/activeledd ${D}/etc/rc6.d/K01activeledd
  ln -fs ../init.d/activeledd ${D}/etc/rc3.d/S99activeledd
  ln -fs ../init.d/activeledd ${D}/etc/rc5.d/S99activeledd
}

FILES_${PN} = "/etc/init.d /etc/rc3.d /etc/rc5.d /etc/rc6.d /etc/rc0.d"
