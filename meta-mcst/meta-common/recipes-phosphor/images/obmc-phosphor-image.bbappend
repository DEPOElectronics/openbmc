inherit reimu-version

def getstatusoutput(cmd):
    import subprocess
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    return p.wait(), p.communicate()

def get_openbmc_commit(d):
    import os
    version = 'unknown-rev'

    cur = os.path.realpath(os.getcwd())
    is_openbmc_root = lambda cur: \
        os.path.isdir(os.path.join(cur, '.git')) and \
        os.path.isfile(os.path.join(cur, 'oe-init-build-env'))

    while cur and cur != '/' and not is_openbmc_root(cur):
        cur = os.path.dirname(cur)

    bb.debug(2, 'Found OpenBMC root %s, is_openbmc=%s'
             % (cur, cur and is_openbmc_root(cur)))
    gitdir = os.path.join(cur, '.git')

    if cur and is_openbmc_root:
        cmd = ['git', '--git-dir=%s' % gitdir,
               '--work-tree=%s' % cur,
               'rev-parse', '--short', 'HEAD']
        exitstatus, output = getstatusoutput(cmd)
        if exitstatus == 0:
            version = output[0].decode("utf-8")[:7]
    return version

ISSUE_COMMIT := "${@get_openbmc_commit(d)}"

do_alter_dates() {
    sed -i "s/(Phosphor OpenBMC Project Reference Distro) 0.1.0/(MCST ${MACHINE_STRING} ${REIMU_VERSION} built at `date +'%Y-%m-%d %H:%M %Z'`, git @${ISSUE_COMMIT})/g" ${IMAGE_ROOTFS}/usr/lib/os-release ${IMAGE_ROOTFS}/etc/issue ${IMAGE_ROOTFS}/etc/issue.net
}

do_rootfs[vardepsexclude] += "ISSUE_COMMIT"

ROOTFS_POSTPROCESS_COMMAND += "do_alter_dates; "

OBMC_KERNEL_MODULES = " \
  kernel-module-rng-core \
  kernel-module-timeriomem-rng \
  l-pcs-i2c \
  kernel-module-tinyspi \
  "

OBMC_IMAGE_EXTRA_INSTALL:append = " \
  ${OBMC_KERNEL_MODULES} \
  active-gpio-on \
  allow-host-boot \
  profile-d \
  openssh \
  buttonscripts \
  glibc-localedata-en-us \
  glibc-localedata-ru-ru \
  glibc-binary-localedata-en-us \
  glibc-binary-localedata-ru-ru \
  wget \
  firmware-updater \
  i2c-tools \
  rawi2ctool \
  net-initial \
  glibc-utils \
  overheatd \
  autofs \
  mcst-fruid \
  astclkinfo \
  nfs-utils-client \
  nss-pam-ldapd \
  sysstat \
  preload-hwrng \
  revert-to-mfg \
  "

IMAGE_FEATURES:remove = "ssh-server-dropbear"
IMAGE_FEATURES:remove = "phosphor-state-manager-chassis"
