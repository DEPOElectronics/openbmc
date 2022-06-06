SUMMARY = "Elbrus PCS I2C kernel driver"
DESCRIPTION = "Elbrus PCS I2C kernel driver"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit module

SRC_URI += "git://github.com/DEPOElectronics/l-pcs-i2c.git;branch=master;protocol=https"
SRCREV = "ec2e38bba18a4cc1a7ed7e7eb9634f188b85c016"

S = "${WORKDIR}/git"

RPROVIDES:${PN} += "kernel-module-l-pcs-i2c"

KERNEL_MODULE_AUTOLOAD += "i2c-dev l-pcs-i2c"

