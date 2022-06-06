FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI += 	" \
		file://login-company-logo.svg \
		file://favicon.ico \
		"
		
do_compile:prepend() {
    cp ${WORKDIR}/favicon.ico ${S}/public/
    cp ${WORKDIR}/login-company-logo.svg ${S}/src/assets/images/
}

