SUMMARY = "DAZN Elbrus for DEPO"
PR = "r1"

inherit packagegroup

PROVIDES = "${PACKAGES}"
PACKAGES = " \
        ${PN}-chassis \
        ${PN}-fans \
        ${PN}-flash \
        ${PN}-system \
        "

PROVIDES += "virtual/obmc-chassis-mgmt"
PROVIDES += "virtual/obmc-fan-mgmt"
PROVIDES += "virtual/obmc-flash-mgmt"
PROVIDES += "virtual/obmc-system-mgmt"

RDEPENDS:${PN}-chassis = " \
    dazn-powerctrl \
    "

RPROVIDES:${PN}-chassis += " \
    dazn-powerctrl \
    "
RPROVIDES:${PN}-fans += "virtual-obmc-fan-mgmt"
RPROVIDES:${PN}-flash += "virtual-obmc-flash-mgmt"
RPROVIDES:${PN}-system += "virtual-obmc-system-mgmt"

RDEPENDS:${PN}-system = " \
        webui-vue \
        dazn-powerctrl \
	id-button \
	kernel-module-l-pcs-i2c	\
	entity-manager \
        dbus-sensors \
	phosphor-virtual-sensor \
	"
