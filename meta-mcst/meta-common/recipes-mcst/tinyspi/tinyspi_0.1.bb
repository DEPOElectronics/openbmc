SUMMARY = "TinySPI kernel driver"
DESCRIPTION = "TinySPI kernel driver"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit module

SRC_URI = " \
            file://Makefile  \
            file://tinyspi.c \
            file://LICENSE \
          "

S = "${WORKDIR}"

RPROVIDES:${PN} += "kernel-module-tinyspi"
