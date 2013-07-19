# All Rights Reversed - No Rights Reserved
#
# Prickle-Prickle, the 53rd day of Confusion in the YOLD 3179
#
# Albert Veli

PROGGY = beep

# Main source file
SRC = $(PROGGY).asm

# Sources included from main source file
INCSRCS = octaves.asm sweep.asm mandom_mod.asm

# Probably no need to change anything below this line

TAP = $(PROGGY).tap
BIN = $(PROGGY).bin
BIN2TAP = bin2tap/bin2tap
# bin2tap options, see bin2tap subdir for documentation
B2TOPTS = -c 32767 -r 32768 -hp -cb 7 -cp 7 -ci 0
ASDIR = z80asm
AS = $(ASDIR)/z80asm

# Flags (for freqs.c)
CFLAGS = -O2 -W -Wall

all: $(TAP)

$(TAP): $(BIN) $(BIN2TAP)
	$(BIN2TAP) -o $@ -b $(B2TOPTS) $(BIN)

$(BIN): $(SRC) $(AS) $(INCSRCS)
	$(AS) -o $@ $(SRC)

$(BIN2TAP):
	make -C bin2tap

$(AS):
	make -C $(ASDIR)

freqs: freqs.o
	$(CC) $(LDFLAGS) -o $@ freqs.o $(LIBS)

.PHONY: clean distclean

clean:
	rm -f $(TAP) $(BIN) freqs freqs.o *~

distclean: clean
	make -C bin2tap clean
	make -C $(ASDIR) clean
