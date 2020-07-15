#!/bin/sh

. /libexec/gpio-funcs

checkalert()
{
    alertgpio="`gpio2num $3`"
    gpiostate="`gpioread $alertgpio`"
    gpiounexport $alertgpio
    [ $gpiostate -ne 0 ] && echo -e "$4 $1" || echo -e "$4 $2"
}

checkalerts()
{
    ON="is \e[32;1mON[0m"
    OFF="is OFF"
    INACT="is inactive"
    ACT="is \e[31;1mACTIVE\e[0m"
    NRST="is online"
    RST="is \e[33;1munder reset\e[0m"
    NDET="not detected"
    DET="\e[31;1mDETECTED\e[0m"

    checkalert "$ON"    "$OFF"  $GPIO_POWER_IN    "System power      "
    checkalert "$NRST"  "$RST"  $GPIO_RESET_IN    "System            "
    checkalert "$INACT" "$ACT"  $GPIO_ALERT_CPU   "Alert 1 for CPU   "
    checkalert "$INACT" "$ACT"  $GPIO_ALERT_CPU2  "Alert 2 for CPU   "
    checkalert "$INACT" "$ACT"  $GPIO_ALERT_MEM   "Alert for DIMM    "
    checkalert "$INACT" "$ACT"  $GPIO_ALERT_PCIE  "Alert for PCIe    "
    checkalert "$INACT" "$ACT"  $GPIO_ALERT_FRU   "Alert for FRU bus "
    checkalert "$INACT" "$ACT"  $GPIO_ALERT_SMBUS "Alert for SMBus   "
    checkalert "$INACT" "$ACT"  $GPIO_TCRIT_SMBUS "SMBus overheat    "
    checkalert "$DET"   "$NDET" $GPIO_INTRUSION   "Chassis intrusion "

    exit 0
}

usage()
{
[ "$@" ] && echo $@
cat << EOF
    Thermal and LED management utility for MUS-A, version 0.1.
    Usage:
        $0 checkalerts       - to list all ALERT, TCRIT, and FAULT conditions.
        $0 <ledname> <mode>  - to turn specific LED on or off.

    <ledname> can be fanfail, pwrfail, sysfail1, or sysfail2.
    <mode> can be on or off.
EOF
exit 0
}

ledtype="$1"
case "$ledtype" in
    "fanfail")
        ledgpio=`gpio2num $GPIO_OVERHEAT_LED`
        ;;
    "pwrfail")
        ledgpio=`gpio2num $GPIO_PWR_FAIL_LED`
        ;;
    "sysfail1")
        ledgpio=`gpio2num $GPIO_SYS_FAIL1_LED`
        ;;
    "sysfail2")
        ledgpio=`gpio2num $GPIO_SYS_FAIL2_LED`
        ;;
    "checkalerts")
        checkalerts
        ;;
    ""|"-h"|"--help")
        usage
        ;;
    *)
        usage "You should specify either 'checkalerts' operation or LED name and mode."
        ;;
esac

ledmode="$2"
case "$ledmode" in
    "on")
        gpiowrite $ledgpio 0
        gpiounexport $ledgpio
        ;;
    "off")
        gpiowrite $ledgpio 1
        gpiounexport $ledgpio
        ;;
    *)
        usage "LED mode should be 'on' or 'off'."
        ;;
esac
