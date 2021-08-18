SUMMARY = "A support library for REIMU software"
DESCRIPTION = "A support library for REIMU software"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = " \
            file://LICENSE \
            file://Makefile \
            file://reimu.c \
            file://reimu.h \
          "

DEPENDS = "dtc"

prefix = "/usr"
libdir = "${prefix}/lib"
includedir = "${prefix}/include"
libname = "reimu"

PV_MAJOR = "${@d.getVar('PV',d,1).split('.')[0]}"

S = "${WORKDIR}"

do_install_append() {
  install -d ${D}${libdir}
  install -d ${D}${includedir}
  install -m 755 lib${libname}.so.${PV} ${D}${libdir}
  install -m 644 lib${libname}.a ${D}${libdir}
  install -m 644 ${libname}.h ${D}${includedir}
  ln -s lib${libname}.so.${PV} ${D}${libdir}/lib${libname}.so
  ln -s lib${libname}.so.${PV} ${D}${libdir}/lib${libname}.so.${PV_MAJOR}
}

FILES_${PN} = "${prefix}/lib"
