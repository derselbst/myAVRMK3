AVR_ROOT=
AVR=$(AVR_ROOT)avr-
AVRCC=$(AVR)gcc
AVRCXX=$(AVR)g++
AVRLD=$(AVR)g++
AVROBJDUMP=$(AVR)objdump
AVROBJCOPY=$(AVR)objcopy
AS=$(AVR)as
AVRDUDE=avrdude

CFLAGS=-Wall -Wextra -g -O0 -mmcu=atmega2560 -DF_CPU=16000000UL
LDFLAGS= -mmcu=atmega2560 -Wl,-Map=prog.map,--cref

OBJ= button.o common.o gameoflife.o interrupt.o lcd.o main.o menu.o MK3_2560_LCD.o setup.o time.o breakout/logic.o breakout/ball.o breakout/paddle.o breakout/wall.o
SRC = $(OBJ:%.o=%.c)

DEPENDFILE = .depend

all: prog

dep: $(SRC)
	$(AVRCC) -MM $(SRC) > $(DEPENDFILE)

# include the dependecy file, do not abort if it doesnt exists
-include $(DEPENDFILE)

prog: dep $(OBJ)
	$(AVRLD) $(LDFLAGS) -o $@.elf $(OBJ)
	$(AVROBJDUMP) -h -S $@.elf > $@.lss
	$(AVROBJCOPY) -R .eeprom -R .fuse -R .lock -R .signature -O ihex $@.elf $@.hex

%.o: %.c
	$(AVRCC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf prog.elf $(OBJ) $(DEPENDFILE) prog.map

burn: prog
	$(AVRDUDE) -p m2560 -c avr911 -P /dev/ttyUSB0 -U flash:w:prog.hex:i
