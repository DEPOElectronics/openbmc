require include/gitrevision.inc

do_install_append() {
    sed -i "s/(Phosphor OpenBMC Project Reference Distro)/${ACTUAL_NAME}/g" ${D}/usr/lib/os-release
}
