include mus-a-image.inc

python() {
    if d.getVar('FLASH_SIZE') == '32768':
        bb.debug("Creating MUS-A image for 32MB flash chip.")
        d.setVar('OBMC_IMAGE_EXTRA_INSTALL_append', d.getVar('OBMC_IMAGE_EXTRA_INSTALL_32MB'))
    elif d.getVar('FLASH_SIZE') == '65536':
        bb.debug("Creating MUS-A image for 64MB flash chip.")
        d.setVar('OBMC_IMAGE_EXTRA_INSTALL_append', d.getVar('OBMC_IMAGE_EXTRA_INSTALL_64MB'))
    else:
        bb.fatal("MUS-A image can be 32MB or 64MB only.")
}
