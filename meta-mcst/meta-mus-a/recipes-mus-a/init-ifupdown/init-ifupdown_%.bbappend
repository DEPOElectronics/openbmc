FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://interfaces \
            file://networking \
            "

do_install_append() {
  install -m 755 ${WORKDIR}/networking ${D}${sysconfdir}/init.d/networking
  install -m 644 ${WORKDIR}/interfaces ${D}${sysconfdir}/network/interfaces
  update-rc.d -r ${D} networking defaults
}

RDEPENDS_${PN} += "bash"
DEPENDS += "update-rc.d-native"
