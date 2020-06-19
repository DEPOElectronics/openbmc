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

# Set correct options file for root
[ -f /mnt/data/etc/bashrc ] && ln -fs /mnt/data/etc/bashrc /home/root/.bashrc || touch /home/root/.bashrc
ln -fs /home/root/.bashrc /home/root/.profile
ln -fs /home/root/.bashrc /home/root/.bash_profile

# Set correct home directory for root
rm -rf /root
ln -s /home/root /root

# Configure lo and eth* interfaces
ip link set dev lo up
/usr/share/netsetup/netconf_script.sh
echo "netconf_script.sh done"

# Copy root's authorized_keys file
mkdir -p /home/root/.ssh
if [ -f /mnt/data/etc/authorized_keys ]
then
    cp /mnt/data/etc/authorized_keys /home/root/.ssh
    chmod 600 /home/root/.ssh/authorized_keys
fi
