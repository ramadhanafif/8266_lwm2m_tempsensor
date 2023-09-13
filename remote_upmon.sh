#!/bin/bash
set -e

pio run 
rsync -azP .pio/build/esp12e/firmware.bin opihome:~/

ssh opihome -t "\"/home/opi/.platformio/penv/bin/python\" \"/home/opi/.platformio/packages/tool-esptoolpy/esptool.py\" --before default_reset --after hard_reset --chip esp8266 --port \"/dev/ttyUSB0\" --baud 115200 write_flash 0x0 ~/firmware.bin && minicom --device /dev/ttyUSB0 --baudrate 115200"
