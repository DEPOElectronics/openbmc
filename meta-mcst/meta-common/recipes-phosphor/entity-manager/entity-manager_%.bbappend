FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI += " file://mcst/blocklist.json "

do_install_append() {
    install -D ${WORKDIR}/mcst/blocklist.json ${D}${datadir}/${BPN}/blacklist.json
}
