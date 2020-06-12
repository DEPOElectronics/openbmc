SUMMARY = "Network configuration script"
DESCRIPTION = "Network configuration script"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "file://netconf_script.sh \
           file://net.conf.example \
           file://LICENSE \
          "

S = "${WORKDIR}"

do_install() {
  dst="${D}/usr/share/netsetup"
  install -d $dst
  install -m 755 netconf_script.sh ${dst}
  install -m 644 net.conf.example ${dst}
}

FBPACKAGEDIR = "/usr/share/netsetup"

FILES_${PN} = "${FBPACKAGEDIR}"

RDEPENDS_${PN} += "bash"
