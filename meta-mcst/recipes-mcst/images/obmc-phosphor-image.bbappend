require include/gitrevision.inc

do_alter_dates() {
    sed -i "s/(Phosphor OpenBMC Project Reference Distro)/(MCST ${MACHINE_STRING} built at `date +'%Y-%m-%d %H:%M'`, git @${ISSUE_COMMIT})/g" ${IMAGE_ROOTFS}/usr/lib/os-release ${IMAGE_ROOTFS}/etc/issue ${IMAGE_ROOTFS}/etc/issue.net
}

addtask do_alter_dates after do_rootfs before do_image