#!/bin/sh

. /libexec/gpio-funcs

checkalert()
{
    alertgpio="`gpio2num $1`"
    gpiostate="`gpioread $alertgpio`"
    gpiounexport $alertgpio
    [ $gpiostate -ne 0 ] && echo "$2 is inactive" || echo -e "$2 is \e[1mACTIVE\e[0m"
}

checkalerts()
{
    checkalert $GPIO_ALERT_CPU CPU_ALERT
    checkalert $GPIO_ALERT_MEM MEM_ALERT
    checkalert $GPIO_ALERT_SMBUS SMBUS_ALERT
    checkalert $GPIO_TCRIT_SMBUS SMBUS_TCRIT
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
