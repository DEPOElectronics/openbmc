SUMMARY = "Python bindings for DBus AT-SPI2 accessibility"
DESCRIPTION = "AT-SPI2 is a protocol over DBus, toolkit widgets use to provide content to screen readers such as Orca"
SECTION = "devel/python"
HOMEPAGE = "https://www.freedesktop.org/wiki/Accessibility/AT-SPI2/"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=db29218e6ba3794c56df7d4987dc7e4d \
                    file://COPYING.GPL;md5=751419260aa954499f7abaabaa882bbe"
DEPENDS = "python3-dbus-native glib-2.0 dbus-glib libxml2 atk gtk+ python3-pygobject"

SRC_URI = "git://github.com/GNOME/pyatspi2.git;protocol=https;branch=master"
SRCREV = "c9cb2a2289a6eb1fb95b66c25d5351bfea54c47e"

S = "${WORKDIR}/git" 

# Same restriction as gtk+
inherit features_check setuptools3
ANY_OF_DISTRO_FEATURES = "${GTK2DISTROFEATURES}"

inherit pkgconfig autotools python3native

FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/pyatspi/*"
