NAME=main
SRC=$(wildcard *.c)
SRCMAIN=main.c
FAMILY=pic14
PROC=12F683
#PROC=12F1822
CFLAGS=

OBJS = $(patsubst %.c,%.o,$(patsubst $(SRCMAIN),,$(SRC)))

all: main clean

main: main.c
	xc8 --output=intel --opt=space --chip=$(PROC) -O"$(NAME).hex" $< $(patsubst $<,,$(SRC))
#	xc8-cc -mcpu=$(PROC) -Os $< $(patsubst $<,,$(SRC))
#	srec_cat main.hex -Intel -crop 0x0 0x0FFF -o main.bin -Binary

clean:
	@echo "Removing $(ODIR)..."
	@rm -rf *.asm *.cod *.lst *.o *.cof *.map *.d *.p1 *.cmf *.elf *.hxl *.s *.sdb *.sym *.rlf *.pre *.as *.obj
	@echo "Done."