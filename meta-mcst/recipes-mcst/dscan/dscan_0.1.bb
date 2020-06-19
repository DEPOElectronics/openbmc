DESCRIPTION = "R-BD-E5R-V4 i2c devices scanner"
SECTION = "examples"
DEPENDS = ""
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=1f30d10c522a80b1aa517769feb0293d"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

SRCREV = "52d2676d3abe131bd6e18ff14475a64d0d3c4657"
SRC_URI = "git://github.com/ShajTester/dscan.git;protocol=http;branch=master \
           file://dscan.conf"

S = "${WORKDIR}/git"

inherit cmake
EXTRA_OECMAKE += " -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_SHARED_LIBS=ON "

BBCLASSEXTEND = "native"
