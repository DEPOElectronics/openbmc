FILESEXTRAPATHS:append := ":${THISDIR}/${PN}"
SRC_URI:append = " file://dazn-baseboard.json \
		   file://GOSPOWER_G1136_1300WNA_PSU.json \
		   file://FSP800_20FM_PSU.json \
                 "

do_install:append() {
     rm -f ${D}/usr/share/entity-manager/configurations/*.json
     install -d ${D}/usr/share/entity-manager/configurations
     install -m 0444 ${WORKDIR}/GOSPOWER_G1136_1300WNA_PSU.json ${D}/usr/share/entity-manager/configurations
     install -m 0444 ${WORKDIR}/FSP800_20FM_PSU.json ${D}/usr/share/entity-manager/configurations
     install -m 0444 ${WORKDIR}/dazn-baseboard.json ${D}/usr/share/entity-manager/configurations
}
