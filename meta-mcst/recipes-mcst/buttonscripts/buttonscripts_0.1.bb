SUMMARY = "Button scripts for MUS-A"
DESCRIPTION = "Button scripts for MUS-A"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = " \
            file://LICENSE \
            file://server_pwrbut_h.sh \
            file://server_pwrbut_s.sh \
            file://server_reset.sh \
          "

S = "${WORKDIR}"

do_install() {
  dst="${D}/usr/bin"
  install -d $dst
  install -m 755 *.sh ${dst}
}

FBPACKAGEDIR = "/usr/bin"

FILES_${PN} = "${FBPACKAGEDIR}"

RDEPENDS_${PN} += "bash"
