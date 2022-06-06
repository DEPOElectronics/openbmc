SUMMARY = "Driver for emc2305"
DESCRIPTION = "DEPO hwmon linux driver for smsc emc2305"
LICENSE = "GPL-3.0-or-later"
LIC_FILES_CHKSUM = "file://LICENSE;md5=7702f203b58979ebbc31bfaeb44f219c"

inherit module

#FILESEXTRAPATHS:prepend := "${THISDIR}/FILES:"

#SRC_URI += "	\
#		file://Makefile \
#		file://emc2305.c \
#		file://LICENSE \
#		"

SRC_URI += "git://github.com/DEPOElectronics/emc2305.git;branch=master;protocol=https"
SRCREV = "e2a5e390c3b169c86c0723e7480a33dfd3f36d82"

S = "${WORKDIR}/git"

RPROVIDES:${PN} += "kernel-module-emc2305"
KERNEL_MODULE_AUTOLOAD += "emc2305"
