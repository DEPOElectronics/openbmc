SUMMARY = "Button scripts for MCST management system"
DESCRIPTION = "Button scripts for MCST management system"
SECTION = "base"
PR = "r1"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

inherit systemd
inherit features_check

DEPENDS = "pkgconfig-native libgpiod dbus libreimu"

REQUIRED_DISTRO_FEATURES = "systemd"
SYSTEMD_SERVICE:${PN} = "host-poweroff-hard.service host-poweroff.service host-poweron.service host-reset.service"

# overwrite the host shutdown to graceful shutdown
HOST_SHUTDOWN_TMPL = "host-poweroff-hard.service"
HOST_SHUTDOWN_TGTFMT = "obmc-host-shutdown@{0}.target"
HOST_SHUTDOWN_FMT = "../${HOST_SHUTDOWN_TMPL}:${HOST_SHUTDOWN_TGTFMT}.requires/${HOST_SHUTDOWN_TMPL}"
SYSTEMD_LINK:${PN} += "${@compose_list_zip(d, 'HOST_SHUTDOWN_FMT', 'OBMC_HOST_INSTANCES')}"

# Force the power cycle target to run the ampere power cycle
HOST_REBOOT_SVC = "host-reset.service"
HOST_REBOOT_SVC_TGTFMT = "obmc-host-reboot@{0}.target"
HOST_REBOOT_SVC_FMT = "../${HOST_REBOOT_SVC}:${HOST_REBOOT_SVC_TGTFMT}.requires/${HOST_REBOOT_SVC}"
SYSTEMD_LINK:${PN} += "${@compose_list_zip(d, 'HOST_REBOOT_SVC_FMT', 'OBMC_HOST_INSTANCES')}"

# chassis power control
CHASSIS_POWERON_SVC = "host-poweron.service"
CHASSIS_POWERON_TGTFMT = "obmc-chassis-poweron@{0}.target"
CHASSIS_POWERON_FMT = "../${CHASSIS_POWERON_SVC}:${CHASSIS_POWERON_TGTFMT}.requires/${CHASSIS_POWERON_SVC}"
SYSTEMD_LINK:${PN} += "${@compose_list_zip(d, 'CHASSIS_POWERON_FMT', 'OBMC_CHASSIS_INSTANCES')}"

CHASSIS_POWEROFF_SVC = "host-poweroff.service"
CHASSIS_POWEROFF_TGTFMT = "obmc-chassis-poweroff@{0}.target"
CHASSIS_POWEROFF_FMT = "../${CHASSIS_POWEROFF_SVC}:${CHASSIS_POWEROFF_TGTFMT}.requires/${CHASSIS_POWEROFF_SVC}"
SYSTEMD_LINK:${PN} += "${@compose_list_zip(d, 'CHASSIS_POWEROFF_FMT', 'OBMC_CHASSIS_INSTANCES')}"

CHASSIS_POWERCYCLE_SVC = "host-reset.service"
CHASSIS_POWERCYCLE_TGTFMT = "obmc-chassis-powercycle@{0}.target"
CHASSIS_POWERCYCLE_FMT = "../${CHASSIS_POWERCYCLE_SVC}:${CHASSIS_POWERCYCLE_TGTFMT}.requires/${CHASSIS_POWERCYCLE_SVC}"
SYSTEMD_LINK:${PN} += "${@compose_list_zip(d, 'CHASSIS_POWERCYCLE_FMT', 'OBMC_CHASSIS_INSTANCES')}"

TMPL = "phosphor-gpio-monitor@.service"
INSTFMT = "phosphor-gpio-monitor@{0}.service"
TGT = "multi-user.target"
FMT = "../${TMPL}:${TGT}.requires/${INSTFMT}"
SYSTEMD_LINK:${PN} += "${@compose_list(d, 'FMT', 'OBMC_HOST_MONITOR_INSTANCES')}"


SRC_URI = " \
            file://LICENSE \
            file://Makefile \
            file://server_ctl.c \
            file://host-poweroff-hard.service \
            file://host-poweroff.service \
            file://host-poweron.service \
            file://host-reset.service \
          "

S = "${WORKDIR}"

do_install() {
  install -d ${D}/usr/bin ${D}/libexec ${D}${systemd_system_unitdir}
  for symlink in pwrbut_s pwrbut_h reset uid pwr_on pwr_off pwr_off_hard watchdog_reset
  do
    ln -s /libexec/server_ctl ${D}/usr/bin/server_${symlink}
  done
  install -m 755 ${S}/server_ctl ${D}/libexec
  for service in poweron poweroff poweroff-hard reset
  do
    install -m 644 ${S}/host-${service}.service ${D}${systemd_system_unitdir}
  done
}

FILES:${PN} = "/usr/bin /libexec ${systemd_system_unitdir}"

RDEPENDS:${PN} += "systemd gpio-funcs"
