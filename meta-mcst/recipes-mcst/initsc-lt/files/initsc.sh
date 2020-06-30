#!/bin/bash
### BEGIN INIT INFO
# Provides:          initsc
# Required-Start:    power-on
# Required-Stop:
# Default-Start:     S
# Default-Stop:
# Short-Description: Power on micro-server
### END INIT INFO

# Disable watchdog
echo "X" > /dev/watchdog
echo "Watchdog disabled"

# Initialize value for CTRL_RDY (ACTIVE# is now controlled by activeledd)
[ -d /sys/class/gpio/gpio111 ] || echo 111 > /sys/class/gpio/export
echo 1 > /sys/class/gpio/gpio111/value

# Set correct debug mode for console
echo 4 > /proc/sys/kernel/printk

# Set TinySPI GPIO to correct mode
# 0x1e6e2000 = base address of SCU regs; we need bits 0, 1, 2, and 6 of SCU88 to be zeroed.
devmem 0x1e6e2088 w 0x01C00000

# Set SPI master CS1# to correct mode (SCU80[15]=1, SCU90[31]=1)
devmem 0x1e6e2090 w 0x8fffa004
devmem 0x1e6e2080 w 0xffc08002
