do_alter_dates() {
    sed -i "s/built at __ISSUE_DATE__/built at `date +'%Y-%m-%d %H:%M'`/g" ${IMAGE_ROOTFS}/usr/lib/os-release ${IMAGE_ROOTFS}/etc/issue ${IMAGE_ROOTFS}/etc/issue.net
}

addtask do_alter_dates after do_rootfs before do_image