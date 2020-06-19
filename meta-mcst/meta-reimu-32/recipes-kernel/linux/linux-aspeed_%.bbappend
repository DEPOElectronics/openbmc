FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

require include/mcst-linux-files.inc

SRC_URI += " file://reimu-32.cfg "
