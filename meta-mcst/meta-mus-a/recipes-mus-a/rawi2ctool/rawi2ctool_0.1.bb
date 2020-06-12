SUMMARY = "Raw I2C connectivity tool"
DESCRIPTION = "A tool for sending SEND and RECV I2C requests"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS += "i2c-tools"

SRC_URI = "file://rawi2ctool.c \
           file://Makefile \
           file://LICENSE \
          "

S = "${WORKDIR}"

pkgdir = "rawi2ctool"

do_install() {
  dst="${D}/usr/local/fbpackages/${pkgdir}"
  bin="${D}/usr/local/bin"
  install -d $dst
  install -d $bin
  install -m 755 rawi2ctool ${dst}/rawi2ctool
  ln -snf ../fbpackages/${pkgdir}/rawi2ctool ${bin}/rawi2ctool
}

FBPACKAGEDIR = "${prefix}/local/fbpackages"

FILES_${PN} = "${FBPACKAGEDIR}/rawi2ctool ${prefix}/local/bin"

