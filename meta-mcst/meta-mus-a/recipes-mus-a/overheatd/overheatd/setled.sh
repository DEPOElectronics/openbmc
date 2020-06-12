#!/bin/sh

modprobe tinyspi # Just in case module is not loaded
sysfsdir="/sys/kernel/tinyspi"

checkalert()
{
    [ $(($1 & $2)) -ne 0 ] && echo "$3 is inactive" || echo -e "$3 is \e[1mACTIVE\e[0m" 
}

checkalerts()
{
    alerts=0x`cat $sysfsdir/state_reg`
    checkalert $alerts 0x00000040 I2C0_ALERT
    checkalert $alerts 0x00000080 I2C0_TCRIT
    checkalert $alerts 0x00000100 I2C1_ALERT
    checkalert $alerts 0x00000200 I2C1_TCRIT
    checkalert $alerts 0x00000400 I2C0_FAULT
    checkalert $alerts 0x00000800 I2C2_ALERT
    checkalert $alerts 0x00001000 I2C3_ALERT
    checkalert $alerts 0x00002000 I2C3_TCRIT
    checkalert $alerts 0x00004000 CPUS_ALERT
    checkalert $alerts 0x00008000 CPUS_TTRIP
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
        ledbits=0x00000040
        ;;
    "pwrfail")
        ledbits=0x00000080
        ;;
    "sysfail1")
        ledbits=0x00000100
        ;;
    "sysfail2")
        ledbits=0x00000200
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
        ledfile=command_bits_reset
        ;;
    "off")
        ledfile=command_bits_set
        ;;
    *)
        usage "LED mode should be 'on' or 'off'."
        ;;
esac

echo $ledbits > $sysfsdir/$ledfile
