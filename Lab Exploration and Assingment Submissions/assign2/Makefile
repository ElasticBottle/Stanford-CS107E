#
# A simple makefile for building program composed of C/asm source files.
# Builds for ARM target using cross-development toolchain. 

NAME = apps/clock

CFLAGS = -Og -g -Wall -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap
LDLIBS =

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: abort.o %.o start.o cstart.o gpio.o timer.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c 
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

install:$(NAME).bin
	rpi-install.py $<

test: tests/test_gpio_timer.bin

testelf: tests/test_gpio_timer.elf

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean test
