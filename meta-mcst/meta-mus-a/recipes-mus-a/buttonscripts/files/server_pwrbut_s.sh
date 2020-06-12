#!/bin/sh

echo out > /sys/class/gpio/gpio109/direction

echo 0 > /sys/class/gpio/gpio109/value

sleep 1

echo 1 > /sys/class/gpio/gpio109/value

echo in > /sys/class/gpio/gpio109/direction

