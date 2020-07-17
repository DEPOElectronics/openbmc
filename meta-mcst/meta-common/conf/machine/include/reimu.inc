KMACHINE = "aspeed"
KERNEL_DEVICETREE = "${KMACHINE}-bmc-mcst-${MACHINE}.dtb"

require conf/machine/include/ast2400.inc
require conf/machine/include/obmc-bsp-common.inc
require conf/machine/include/flash.inc
require conf/machine/include/serial.inc
