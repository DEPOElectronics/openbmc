SUMMARY = "A support library for REIMU software"
DESCRIPTION = "A support library for REIMU software"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit meson

SRC_URI = "git://github.com/makise-homura/libreimu.git;protocol=https"
SRCREV = "${AUTOREV}"

DEPENDS = "dtc dbus libgpiod"

S = "${WORKDIR}/git"
