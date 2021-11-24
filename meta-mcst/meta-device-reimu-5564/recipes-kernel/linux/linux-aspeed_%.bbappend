FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI += " \
	file://aspeed-bmc-depo-reimu-5564.dts \
	file://reimu-64.cfg \
	"
	
# Merge source tree by original project with our layer of additional files
do_add_mcst_files () {
    WD=${WORKDIR}/../oe-local-files/
    [ -d ${WD} ] || WD=${WORKDIR}
    cp -r ${WD}/${KMACHINE}-bmc-depo-${MACHINE}.dts \
          ${WD}/*.dtsi \
          ${STAGING_KERNEL_DIR}/arch/arm/boot/dts
}
addtask do_add_mcst_files after do_patch before do_kernel_configme

