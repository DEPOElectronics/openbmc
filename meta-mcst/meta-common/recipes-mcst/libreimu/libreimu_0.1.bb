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
            file://reimu_fdt.c \
            file://reimu_dbus.c \
            file://reimu_gpio.c \
            file://reimu.h \
          "

DEPENDS = "pkgconfig-native dtc dbus libgpiod"

prefix = "/usr"
libdir = "${prefix}/lib"
includedir = "${prefix}/include"
libname = "reimu"
suffixes = "{,_fdt,_gpio,_dbus}"

PV_MAJOR = "${@d.getVar('PV',d,1).split('.')[0]}"

S = "${WORKDIR}"


do_install() {
  install -d ${D}${libdir}
  install -d ${D}${includedir}
  install -m 755 lib${libname}${suffixes}.so.${PV} ${D}${libdir}
  install -m 644 ${S}/${libname}.h ${D}${includedir}
  for lib in lib${libname}${suffixes}.so
  do
      ln -s $lib.${PV} ${D}${libdir}/$lib
      ln -s $lib.${PV} ${D}${libdir}/$lib.${PV_MAJOR}
  done
}

FILES_${PN} = "${libdir}/*.so.*"
FILES_${PN}-dev = "${includedir} ${libdir}/*.so"
