SUMMARY = "Midnight Commander is an ncurses based file manager"
HOMEPAGE = "http://www.midnight-commander.org/"
SECTION = "console/utils"

LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://COPYING;md5=270bbafe360e73f9840bd7981621f9c2"

DEPENDS = "ncurses glib-2.0 util-linux libssh2"
RDEPENDS_${PN} = "ncurses-terminfo python3-core perl glibc-localedata-en-us"

SRC_URI = "http://www.midnight-commander.org/downloads/${BPN}-${PV}.tar.bz2"
SRC_URI[md5sum] = "152927ac29cf0e61d7d019f261bb7d89"
SRC_URI[sha256sum] = "238c4552545dcf3065359bd50753abbb150c1b22ec5a36eaa02c82808293267d"

EXTRA_OECONF = "--with-screen=ncurses --without-gpm-mouse --without-x --enable-vfs-smb --enable-vfs-sftp --with-homedir=/mnt/data/mc"

inherit autotools gettext pkgconfig
