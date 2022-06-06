FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

DEPENDS:append:dacn469535003v2= " dacn469535003v2-yaml-config"

EXTRA_OECONF:dacn469535003v2= " \
    SENSOR_YAML_GEN=${STAGING_DIR_HOST}${datadir}/dacn469535003v2-yaml-config/ipmi-sensors.yaml \
    "
