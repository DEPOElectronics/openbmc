#!/bin/bash
if [ ! -e "/sys/bus/i2c/devices/i2c-4" ]
then 
  echo "plug i2c-4"
  echo -n "1e78a140.i2c-bus" > /sys/bus/platform/drivers/aspeed-i2c-bus/bind
fi

if [ ! -e "/sys/bus/i2c/devices/i2c-6" ]
then
  echo "plug i2c-6"
  echo -n "1e78a1c0.i2c-bus" > /sys/bus/platform/drivers/aspeed-i2c-bus/bind
fi

host setstate
exit 0
