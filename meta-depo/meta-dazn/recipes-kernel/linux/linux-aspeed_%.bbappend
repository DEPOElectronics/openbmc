FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:dazn = " \
             file://aspeed-bmc-depo-dazn.dts \
             file://dazn-gpio.dtsi \
             file://dazn-led.dtsi \
	     file://dazn-keys.dtsi \
             file://dazn.cfg \
           "

# Merge source tree by original project with our layer of additional files

do_patch:append () {
    WD=${WORKDIR}/../oe-local-files/
    [ -d ${WD} ] || WD=${WORKDIR}
    cp -r ${WD}/${KMACHINE}-bmc-depo-${MACHINE}.dts \
   	  ${WD}/*.dtsi \
          ${STAGING_KERNEL_DIR}/arch/arm/boot/dts
}
