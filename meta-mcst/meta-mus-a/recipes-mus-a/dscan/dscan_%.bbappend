
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://dscan.conf \
           "

do_install_append() {
    install -d ${D}${sysconfdir}/dscan
    install -m 644 ../dscan.conf ${D}${sysconfdir}/dscan/dscan.conf
}
