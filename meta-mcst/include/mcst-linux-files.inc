SRC_URI += " file://arch "

# Merge source tree by original project with our layer of additional files
do_add_mcst_files () {
    cp -r "${WORKDIR}/arch" \
          "${STAGING_KERNEL_DIR}"
}
addtask do_add_mcst_files after do_kernel_checkout before do_patch
