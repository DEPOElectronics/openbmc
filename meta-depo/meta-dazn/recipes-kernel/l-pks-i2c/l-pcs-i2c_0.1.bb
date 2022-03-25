SUMMARY = "Elbrus PCS I2C kernel driver"
DESCRIPTION = "Elbrus PCS I2C kernel driver"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit module

SRC_URI += "git://github.com/gluhow/l-pcs-i2c.git;branch=master;protocol=https"
SRCREV = "6cb84a68a99f5d312f1b851413720b600b2a5115"

S = "${WORKDIR}"

RPROVIDES:${PN} += "kernel-module-l-pcs-i2c"

KERNEL_MODULE_AUTOLOAD += "i2c-dev l-pcs-i2c"

