AVR_ROOT=
AVR=$(AVR_ROOT)avr-
AVRCC=$(AVR)gcc
AVRCXX=$(AVR)g++
AVRLD=$(AVR)gcc
AVROBJDUMP=$(AVR)objdump
AVROBJCOPY=$(AVR)objcopy
AS=$(AVR)as
AVRDUDE=avrdude

INCLUDES=-Igames/breakout/ -Igames/gameoflife/ -Ihal/ -Ilogic/

CFLAGS=-Wall -Wextra -O0 -mmcu=atmega2560 -DF_CPU=16000000UL $(INCLUDES)
LDFLAGS= -mmcu=atmega2560 -O0 -Wl,-Map=prog.map,--cref

OBJ= hal/button.o logic/common.o games/gameoflife/gameoflife.o hal/interrupt.o main.o logic/menu.o hal/lcd.o hal/setup.o logic/time.o logic/vqc10.o games/breakout/logic.o games/breakout/ball.o games/breakout/paddle.o games/breakout/wall.o
SRC = $(OBJ:%.o=%.c)

DEPENDFILE = .depend

all: prog

dep: $(SRC)
	$(AVRCC) -MM $(INCLUDES) $(SRC) > $(DEPENDFILE)

# include the dependecy file, do not abort if it doesnt exists
-include $(DEPENDFILE)

prog: dep $(OBJ)
	$(AVRLD) $(LDFLAGS) -o $@.elf $(OBJ)
	$(AVROBJDUMP) $(INCLUDES) --all-headers --source $@.elf > $@.lss
	$(AVROBJCOPY) -R .eeprom -R .fuse -R .lock -R .signature -O ihex $@.elf $@.hex

%.o: %.c
	$(AVRCC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf prog.elf $(OBJ) $(DEPENDFILE) prog.map prog.hex prog.lss

burn: prog
	$(AVRDUDE) -p m2560 -c avr911 -P /dev/ttyUSB0 -U flash:w:prog.hex:i
