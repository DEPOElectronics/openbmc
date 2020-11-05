do_install_append() {
    install -d ${D}/var/volatile/log/redfish
}

FILES_${PN} += " /var/volatile/log/redfish "