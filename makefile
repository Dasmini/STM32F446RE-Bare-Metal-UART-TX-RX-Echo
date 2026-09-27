CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
FLASH = st-flash

CFLAGS = -mcpu=cortex-m4 -mthumb -nostdlib
LDFLAGS = -T linker.ld

SRCS = main.c uart_transmit.c uart_recieve.c startup.c
ELF = firmware.elf
BIN = firmware.bin

all: $(BIN)

$(ELF): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $(SRCS)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BIN)

flash: $(BIN)
	sudo $(FLASH) write $(BIN) 0x08000000

clean:
	rm -f $(ELF) $(BIN)

.PHONY: all flash clean