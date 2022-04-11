FILESEXTRAPATHS:append := "${THISDIR}/files:"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit systemd
inherit obmc-phosphor-systemd

S = "${WORKDIR}"

SRC_URI = "file://host \
           file://host-poweroff.service \
           file://host-poweroff-hard.service \
           file://host-poweron.service \
           file://host-reset.service \ 
           file://host-state.service \          
           file://shutup \
           file://setpowerstate \
           "

DEPENDS = "systemd"
RDEPENDS:${PN} = "bash"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "host-poweroff.service host-poweroff-hard.service host-poweron.service host-reset.service host-state.service"

do_install() {
    install -d ${D}/${bindir}
    install -m 0755 ${S}/host ${D}/${bindir}/    
    install -m 0755 ${S}/shutup ${D}/${bindir}/    
    install -m 0755 ${S}/setpowerstate ${D}/${bindir}/    
}

pkg_postinst:${PN}() {
    mkdir -p $D$systemd_system_unitdir/obmc-host-startmin@0.target.requires
    LINK="$D$systemd_system_unitdir/obmc-host-startmin@0.target.requires/host-poweron.service"
    TARGET="../host-poweron.service"
    ln -s $TARGET $LINK
    
    TARGET="../host-poweroff.service"
    srvreq=obmc-chassis-poweroff@0.target.requires
    mkdir -p $D$systemd_system_unitdir/$srvreq
    LINK="$D$systemd_system_unitdir/$srvreq/"
    ln -s $TARGET $LINK

    TARGET="../host-poweroff-hard.service"
    srvreq=obmc-chassis-hard-poweroff@.target.requires
    mkdir -p $D$systemd_system_unitdir/$srvreq
    LINK="$D$systemd_system_unitdir/$srvreq/"
    ln -s $TARGET $LINK

    TARGET="../host-reset.service"
    srvreq=obmc-host-reset@0.target.requires
    mkdir -p $D$systemd_system_unitdir/$srvreq
    LINK="$D$systemd_system_unitdir/$srvreq/"
    ln -s $TARGET $LINK
}
