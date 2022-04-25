FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append = " \
    file://phosphor-multi-gpio-monitor.json \
    "

do_install:append() {
    install -d ${D}/usr/share/phosphor-gpio-monitor
    install -m 0644 ${WORKDIR}/phosphor-multi-gpio-monitor.json ${D}/usr/share/phosphor-gpio-monitor/
}

pkg_postinst:${PN}() {
    LINK="$D$systemd_system_unitdir/multi-user.target.wants/"
    TARGET="../phosphor-multi-gpio-monitor.service"
    ln -s $TARGET $LINK
}
