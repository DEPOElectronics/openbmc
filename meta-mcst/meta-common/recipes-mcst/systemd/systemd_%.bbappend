do_install_append() {
    rm ${D}/usr/lib/sysctl.d/50-coredump.conf
}
