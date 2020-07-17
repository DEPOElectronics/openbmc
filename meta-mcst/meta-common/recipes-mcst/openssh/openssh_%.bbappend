FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

RDEPENDS_${PN} += "tmux-logger"

SRC_URI += " file://sshd_config \
             file://sshd.socket \
           "
