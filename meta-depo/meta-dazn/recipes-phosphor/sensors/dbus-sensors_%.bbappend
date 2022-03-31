FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI += " \
		file://0001-del-pwm.patch \
		file://0002-add-drivers.patch \
		"
