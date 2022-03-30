SUMMARY = "DEPO LM63 I2C kernel driver"
DESCRIPTION = "DEPO LM63 I2C kernel driver"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit module

SRC_URI += "git://github.com/gluhow/dp_lm63.git;branch=master;protocol=https"
SRCREV = "a722616c331c21f62eac00d86f1ca112a3339d89"

S = "${WORKDIR}"

RPROVIDES:${PN} += "kernel-module-dp_lm63"
KERNEL_MODULE_AUTOLOAD += "dp_lm63"
