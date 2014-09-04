avr-gcc -Os -c -mmcu=attiny13 $1.c
avr-gcc -mmcu=attiny13 $1.o -o $1.elf
avr-objcopy -O ihex -R .eeprom $1.elf $1.hex
avr-size $1.elf
