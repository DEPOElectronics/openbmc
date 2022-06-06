FILESEXTRAPATHS:append := ":${THISDIR}/${PN}"
SRC_URI:append = " file://dazn-chassis.json \
		   file://dazn-system.json \
		   file://GOSPOWER_G1136_1300WNA_PSU.json \
		   file://FSP800_20FM_PSU.json \
		   file://FSP800_20ERM.json \
		   file://blocklist.json \
                 "

do_install:append() {
     rm -f ${D}/usr/share/entity-manager/configurations/*.json
     install -d ${D}/usr/share/entity-manager/configurations
     install -m 0644 ${WORKDIR}/*.json ${D}/usr/share/entity-manager/configurations
     rm -f ${D}/usr/share/entity-manager/configurations/blocklist.json
}
