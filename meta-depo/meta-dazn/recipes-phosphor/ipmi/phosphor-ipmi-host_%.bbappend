FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

DEPENDS:append:dazn= " dazn-yaml-config"

EXTRA_OECONF:dazn= " \
    SENSOR_YAML_GEN=${STAGING_DIR_HOST}${datadir}/dazn-yaml-config/ipmi-sensors.yaml \
    "
