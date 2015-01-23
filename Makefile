AVR_ROOT=
AVR=$(AVR_ROOT)avr-
AVRCC=$(AVR)gcc
AVRCXX=$(AVR)g++
AVRLD=$(AVR)g++
AS=$(AVR)as
AVRDUDE=avrdude

CFLAGS=-Wall -Wextra -g -mmcu=atmega2560 -DF_CPU=16000000UL
LDFLAGS=-L/usr/lib -mmcu=atmega2560 -Wl,-Map=prog.map,--cref

OBJ= button.o common.o gameoflife.o interrupt.o lcd.o main.o menu.o MK3_2560_LCD.o setup.o time.o
SRC = $(OBJ:%.o=%.c)

DEPENDFILE = .depend

all: prog

dep: $(SRC)
	$(AVRCC) -MM $(SRC) > $(DEPENDFILE)

# include the dependecy file, do not abort if it doesnt exists
-include $(DEPENDFILE)

prog: dep $(OBJ)
	$(AVRLD) $(LDFLAGS) -o $@.elf $(OBJ)

%.o: %.c
	$(AVRCC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf prog.elf $(OBJ) $(DEPENDFILE) prog.map

burn: prog
	$(AVRDUDE) 
