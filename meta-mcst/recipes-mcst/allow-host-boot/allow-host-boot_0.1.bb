SUMMARY = "Allow host boot"
DESCRIPTION = "Allow host boot by releasing SPI_CONNECT and pushing power button on if needed"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS += "update-rc.d-native"

SRC_URI = " \
            file://LICENSE \
            file://allow_host_boot \
            file://auto_power_on \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/etc
  install -d ${D}/etc/init.d
  install -m 755 ${S}/auto_power_on ${D}/etc
  install -m 755 ${S}/allow_host_boot ${D}/etc/init.d
  update-rc.d -r ${D} allow_host_boot defaults
}

FILES_${PN} = "/etc"

RDEPENDS_${PN} += "gpio-funcs"
