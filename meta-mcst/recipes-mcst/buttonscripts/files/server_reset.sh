#!/bin/sh

echo out > /sys/class/gpio/gpio123/direction

echo 0 > /sys/class/gpio/gpio123/value

sleep 1

echo 1 > /sys/class/gpio/gpio123/value

echo in > /sys/class/gpio/gpio123/direction