#!/bin/bash

if [ -f /mnt/data/etc/network/net.conf ]
then
    . /mnt/data/etc/network/net.conf
else
    mkdir -p /mnt/data/etc/network
    cp /usr/share/netsetup/net.conf.example /mnt/data/etc/network/net.conf
    # We don't source example file, because it does not contain any significant line
fi

[ -f /mnt/data/etc/network/hosts ] && ln -fs /mnt/data/etc/network/hosts /etc/hosts
[ -f /mnt/data/etc/network/resolv.conf ] && ln -fs /mnt/data/etc/network/resolv.conf /etc/resolv.conf

bring_if_up() # ifname enabled mac ip gw
{
    if [ "$2" = "yes" ]
    then
        echo "Setting MAC address $3 for interface $1..."
        ip link set dev $1 address $3
        echo "Setting IP address $4 for interface $1..."
        ip addr add $4 brd + dev $1
        echo "Bringing up interface $1..."
        ip link set dev $1 up
        if [ -n "$5" ]
        then
            echo "Setting up default gateway $5 for interface $1..."
            ip route add default via $5 dev $1
        fi
    else
        echo "Interface $1 is disabled, skipping configuration."
    fi
}

bring_if_down() # ifname enabled
{
    if [ "$2" = "yes" ]
    then
        echo "Bringing down interface $1..."
        ip link set dev $1 down
        echo "Flushing gateway for interface $1..."
        ip route flush dev $1
        echo "Removing IP address from interface $1..."
        ip addr flush dev $1
    else
        echo "Interface $1 is disabled, skipping shutdown."
    fi
}

# Default values

ETH0_ENABLED=${ETH0_ENABLED:-yes}
ETH1_ENABLED=${ETH1_ENABLED:-yes}

MAC0=${MAC0:-98:a7:b0:00:00:f0}
MAC1=${MAC1:-98:a7:b0:00:00:f1}

IP0=${IP0:-192.168.1.1/24}
IP1=${IP1:-192.168.2.1/24}

GW0=${GW0:-}
GW1=${GW1:-}

COMMAND=$1

[ -z "$COMMAND" ] && COMMAND="start"

case $COMMAND in
    stop)
        bring_if_down eth0 $ETH0_ENABLED
        bring_if_down eth1 $ETH1_ENABLED
        ;;

    restart)
        $0 stop
        sleep 1
        $0 start
        ;;

    start)
        bring_if_up eth0 $ETH0_ENABLED $MAC0 $IP0 $GW0
        bring_if_up eth1 $ETH1_ENABLED $MAC1 $IP1 $GW1
        ;;

    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
esac
