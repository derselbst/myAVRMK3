#!/bin/sh
~/bin/simavr -t -g -m atmega2560 -f 16000000 prog.elf

# in avr-gdb:
# file prog.elf
# target remote localhost:1234
# break main
# continue
