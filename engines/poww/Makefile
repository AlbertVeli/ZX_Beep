# (k) ALL RIGHTS REVERSED - Reprint what you like
#
# Boomtime, the 66th day of Confusion in the YOLD 3179
#
# Albert Veli

PROGGY = poww

# Main source file
SRC = $(PROGGY).asm

# Sources included from main source file
INCSRCS = music.asm

# Probably no need to change anything below this line

TAP = $(PROGGY).tap
BIN = $(PROGGY).bin
ZXTAP = ../../ZX_Tap/zxtap

ASDIR = ../../z80asm
AS = $(ASDIR)/z80asm

all: $(TAP)

$(TAP): $(BIN) $(ZXTAP)
	$(ZXTAP) -o $@ $(BIN)

$(BIN): $(SRC) $(AS) $(INCSRCS)
	$(AS) -o $@ $(SRC)

.PHONY: clean

clean:
	rm -f $(TAP) $(BIN) *~
