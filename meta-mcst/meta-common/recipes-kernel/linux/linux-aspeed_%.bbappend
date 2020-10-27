FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
             file://reimu.cfg \
             file://aspeed-bmc-mcst-ast2400.dtsi \
             file://aspeed-bmc-mcst-ast2500.dtsi \
             file://0001-squashfs-generate-uuid-for-superblock.patch \
           "

# Merge source tree by original project with our layer of additional files
do_add_mcst_files () {
    cp -r ${WORKDIR}/${KMACHINE}-bmc-mcst-${MACHINE}.dts \
          ${WORKDIR}/*.dtsi \
          ${STAGING_KERNEL_DIR}/arch/arm/boot/dts
}
addtask do_add_mcst_files after do_kernel_checkout before do_patch
