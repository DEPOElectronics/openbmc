SUMMARY = "REIMU - REmote Intelligent Management Unit software"
DESCRIPTION = "A web interface for OpenBMC based BMCs"
SECTION = "net"
PR = "r1"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://git/LICENSE;md5=1ebbd3e34237af26da5dc08a4e440464"

RDEPENDS_${PN} = "nginx php-fpm"

SRCREV = "${AUTOREV}"
SRC_URI = "git://github.com/makise-homura/reimu.git;protocol=http;branch=master \
           file://reimu-nginx.conf \
           file://reimu-platform.conf \
          "

S = "${WORKDIR}"

do_install() {
  nginxconf="${D}/etc/nginx/sites-enabled"
  wwwroot="${D}/opt/reimu"
  reimuconf="${wwwroot}/conf"
  install -d $nginxconf
  install -d $wwwroot
  install -d $reimuconf
  install -m 644 -t ${wwwroot} git/wwwroot/*
  install -m 644 reimu-nginx.conf ${nginxconf}/default.conf
  install -m 644 reimu-platform.conf ${reimuconf}/reimu.conf
}

FILES_${PN} = "/opt/reimu /etc/nginx/sites-enabled"
