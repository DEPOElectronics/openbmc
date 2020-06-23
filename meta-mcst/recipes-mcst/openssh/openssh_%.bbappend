FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += " file://sshd_config "

do_configure_append() {
  sed -ri "s/__OPENBMC_VERSION__/${OPENBMC_VERSION}/g" sshd_config
}
