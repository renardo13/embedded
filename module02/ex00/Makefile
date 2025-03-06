# *.c must be converted into .bin and the .bin converted into .hex

SRC = main.c
OBJ = main.bin
NAME = main.hex
CC = avr-gcc
COPY = avr-objcopy
MMCU = -mmcu=atmega328p
PORT = /dev/ttyUSB0
F_CPU = -F_CPU=16000000UL
BAUDRATE = 115200
PROGRAMMER = arduino
RM = rm -f

all: hex flash

hex:
	$(CC) $(MMCU) $(F_CPU) -o $(OBJ) $(SRC)
	$(COPY) $(OBJ) $(NAME)

flash:
	avrdude -p m328p -b $(BAUDRATE) -c $(PROGRAMMER) -P $(PORT) -D -U $(NAME)

clean:
	$(RM) $(NAME) $(OBJ)

re: clean all

.PHONY: all clean re hex flash