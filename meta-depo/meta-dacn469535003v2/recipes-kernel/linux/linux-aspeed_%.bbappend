FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:dacn469535003v2 = " \
             file://aspeed-bmc-depo-dacn469535003v2.dts \
             file://dacn469535003v2-gpio.dtsi \
             file://dacn469535003v2-led.dtsi \
	     file://dacn469535003v2-keys.dtsi \
	     file://dacn469535003v2-i2c.dtsi \
             file://dacn469535003v2.cfg \
           "

# Merge source tree by original project with our layer of additional files

do_patch:append () {
    WD=${WORKDIR}/../oe-local-files/
    [ -d ${WD} ] || WD=${WORKDIR}
    cp -r ${WD}/${KMACHINE}-bmc-depo-${MACHINE}.dts \
   	  ${WD}/*.dtsi \
          ${STAGING_KERNEL_DIR}/arch/arm/boot/dts
}
