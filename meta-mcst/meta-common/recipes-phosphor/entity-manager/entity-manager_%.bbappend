FILESEXTRAPATHS:prepend := "${THISDIR}:"
SRC_URI += " file://mcst/blocklist.json "

do_install:append() {
    install -D ${WORKDIR}/mcst/blocklist.json ${D}${datadir}/${BPN}/blacklist.json
}
