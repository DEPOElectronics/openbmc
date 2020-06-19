SUMMARY = "CY14B101P RTC kernel driver"
DESCRIPTION = "CY14B101P RTC kernel driver"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit module

SRC_URI = " \
            file://Makefile  \
            file://rtc-cy14b101p.c \
            file://LICENSE \
          "

S = "${WORKDIR}"

RPROVIDES_${PN} += "kernel-module-rtc-cy14b101p"
