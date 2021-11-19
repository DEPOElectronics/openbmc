SUMMARY = "Preload timeriomem_rng to get entropy earlier"
DESCRIPTION = "Preload timeriomem_rng to get entropy earlier"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"
SRC_URI = "file://LICENSE file://timeriomem_rng.conf"
S = "${WORKDIR}"
do_install() {
  install -d ${D}/etc/modules-load.d
  install -m 644 timeriomem_rng.conf ${D}/etc/modules-load.d
}
FILES:${PN} = "/etc"
