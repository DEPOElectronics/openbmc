inherit meson

EXTRA_OEMESON += "-Dtests=disabled"

PACKAGECONFIG[max31785-msl] = "-Denable-max31785-msl=true, -Denable-max31785-msl=false"

SRCREV = "9c4053455f479c85221712f790187076669e0d59"
