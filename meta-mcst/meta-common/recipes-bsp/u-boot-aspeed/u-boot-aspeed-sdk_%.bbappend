FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI += " file://${UBOOT_DEVICETREE}.dts file://0001-disable-spi-dma-for-qemu.patch "

DEPENDS += "dtc-native"

do_compile_dtb() {
    ln -fs ${WORKDIR}/${UBOOT_DEVICETREE}.dts ${S}/arch/arm/dts
    ${CPP} -nostdinc -I ${S}/arch/arm/dts/include -undef -x assembler-with-cpp ${S}/arch/arm/dts/${UBOOT_DEVICETREE}.dts -o ${B}/${UBOOT_DEVICETREE}.pp.dts
    dtc ${B}/${UBOOT_DEVICETREE}.pp.dts > ${B}/arch/arm/dts/${UBOOT_DEVICETREE}.dtb
}

addtask do_compile_dtb after do_configure before do_compile
