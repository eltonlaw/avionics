# .PHONY: all build flash
# 
# MCU =
# F_CPU =
# BAUD =
# PORT = /dev/ttyACM0
# 
# all: build flash
# 
# build:
# 	# Generates assembly code
# 	avr-gcc -g -Os -Wall -mcall-prologues -mmcu=atmega328 -S -o main.s main.c
# 	# Generate object code
# 	avr-gcc -g -Os -Wall -mcall-prologues -mmcu=atmega328 -c -o main.o main.c
# 	avr-gcc -g -Os -Wall -mcall-prologues -mmcu=atmega328 main.o -o main.obj
# 	# Remove eeprom section and translate to intel hex
# 	avr-objcopy -R .eeprom -O ihex main.obj main.hex
# 
# flash:
# 	avrdude -c arduino -p m328p -P $(PORT) -e -v -U flash:w:main.hex
# 
# clean:
# 	rm -f *.elf *.obj *.hex *.o *.s

.PHONY: flash contest clean

TARGET=main
BAUD=9600
F_CPU=1000000UL
MCU=atmega328
HEX_FILE=src/$(TARGET).hex

PROGRAMMER_TYPE=arduino_as_isp
# AVR_DEVICE=m328p
AVR_DEVICE=m328
PROGRAMMER_PORT=/dev/ttyACM0
PROGRAMMER_OPTS= -c $(PROGRAMMER_TYPE) -p $(AVR_DEVICE) -P $(PROGRAMMER_PORT) -b 19200

SRC_VARS= TARGET=$(TARGET)
SRC_VARS += BAUD=$(BAUD) F_CPU=$(F_CPU) MCU=$(MCU)

build:
	cd src && make $@ $(SRC_VARS)

$(HEX_FILE): build

flash: $(HEX_FILE)
	avrdude $(PROGRAMMER_OPTS) -e -v -U flash:w:$(HEX_FILE)

contest:
	avrdude $(PROGRAMMER_OPTS) -t

clean:
	cd src && rm -f *.elf *.obj *.hex *.o *.s *.map
