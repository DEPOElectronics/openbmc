LINUX_VERSION_EXTENSION = "-mus-a"

COMPATIBLE_MACHINE = "mus-a"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://defconfig \
           "

KERNEL_MODULE_AUTOLOAD += " \
"
