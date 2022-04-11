SUMMARY = "Elbrus PCS I2C kernel driver"
DESCRIPTION = "Elbrus PCS I2C kernel driver"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit module

SRC_URI += "git://github.com/gluhow/l-pcs-i2c.git;branch=master;protocol=https"
SRCREV = "2a76409332ab354d2d45e7abf7e1af442669e637"

S = "${WORKDIR}/git"

RPROVIDES:${PN} += "kernel-module-l-pcs-i2c"

KERNEL_MODULE_AUTOLOAD += "i2c-dev l-pcs-i2c"

