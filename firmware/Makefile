
DEVICE     = attiny13a
CLOCK      = 9600000
PROGRAMMER = -c avrispmkII -P usb
OBJECTS    = main.o
# Use http://www.engbedded.com/fusecalc
# Reset Disabled (Enable PB5 as i/o pin)
FUSES      = -U lfuse:w:0x7a:m -U hfuse:w:0xfe:m


ifeq ($(RELEASE),1)
COMPILE_OPT = -Os
else
COMPILE_OPT = -Os -g3 -gdwarf-2
endif

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Ideps -Wl,--relax,--gc-sections -Wall $(COMPILE_OPT) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -fdata-sections -ffunction-sections -ffreestanding -fno-schedule-insns2 -mcall-prologues -fno-tree-scev-cprop -fno-split-wide-types
CXXFLAGS_STD = -std=c++14

# symbolic targets:
all:	main.hex

run:	all
	~/project/simavr/simavr/run_avr -g -mcu $(DEVICE) -freq $(CLOCK) main.hex &
	avr-gdb ; killall run_avr

.c.o:
	$(COMPILE) -c $< -o $@

.cpp.o:
	$(COMPILE) $(CXXFLAGS_STD) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	clean all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS) -lm

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

