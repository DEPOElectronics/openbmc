#!/bin/bash
#
# This script is used to get the MAC Address from FRU Inventory information

ETHERNET_INTERFACE="eth0"
ENV_ETH="ethaddr"

#eth1 not connected
ENV_ETH1="eth1addr"
MAC1_ADDR="AA:AA:AA:AA:AA:01"

ENV_MAC_ADDR=$(fw_printenv | grep $ENV_ETH)

# Read MAC Addr from MCST FRUID
FRUIDPATH="/sys/bus/i2c/devices/3-0057/eeprom"
SFRUIDMAC="mac_addr"
BMCMAC_SHIFT=0x6

split_mac() { echo ${1:0:2}:${1:2:2}:${1:4:2}:${1:6:2}:${1:8:2}:${1:10:2}; }

create_initfile()
{
	echo "Create init file"
	mkdir -p "/var/lib/first-boot-set-mac"
	touch "/var/lib/first-boot-set-mac/$ETHERNET_INTERFACE"
}

if [ -e $FRUIDPATH ]; then
        READ_FULL_FRUIDMAC=$(hexdump $FRUIDPATH -s 10 -n 15 -v -e '9 "%_p" 6/1 "%02X" "\n"')
        READ_CAPT_FRUIDMAC=$(echo ${READ_FULL_FRUIDMAC:0:8})

	if [ $READ_CAPT_FRUIDMAC != $SFRUIDMAC ]; then
		echo "Bad MCST FRUID format MAC addr"
		exit 1
	else
		#GET FRUID BASE MAC
		READ_VAL_FRUIDMAC=$(echo ${READ_FULL_FRUIDMAC:9:12})
		#FRUID BASE MAC+ BMC SHIFT
		VALMAC=$(printf %X $((0x$READ_VAL_FRUIDMAC + $BMCMAC_SHIFT)))
		MAC_ADDR=$(split_mac $VALMAC)
	fi
fi

# Check if BMC MAC address is exported
if [ -z "${MAC_ADDR}" ]; then
	echo "ERROR: No BMC MAC address is detected from FRU Inventory information!"
	# Return 1 so that systemd knows the service failed to start
	exit 1
fi

# Check if BMC MAC address is exported
if [[ $ENV_MAC_ADDR =~ $MAC_ADDR ]]; then
	echo "WARNING: BMC MAC address already exist!"
	create_initfile
	exit 0
fi

# Request to update the MAC address
if ! fw_setenv ${ENV_ETH} "${MAC_ADDR}";
then
	echo "ERROR: Fail to set MAC address to ${ENV_ETH}"
	exit 1
else
	create_initfile
fi

fw_setenv ${ENV_ETH1} "${MAC1_ADDR}"

# Request to restart the service
ifconfig ${ETHERNET_INTERFACE} down
if ! ifconfig ${ETHERNET_INTERFACE} hw ether "${MAC_ADDR}";
then
	echo "ERROR: Can not update MAC ADDR to ${ETHERNET_INTERFACE}"
	exit 1
fi
ifconfig ${ETHERNET_INTERFACE} up

echo "Successfully update the MAC address ${MAC_ADDR} to ${ENV_ETH} and ${ETHERNET_INTERFACE}"
