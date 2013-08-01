# (k) ALL RIGHTS REVERSED - Reprint what you like
#
# Boomtime, the 66th day of Confusion in the YOLD 3179
#
# Albert Veli

PROGGY = lstest

# Main source file
SRC = $(PROGGY).asm

# Sources included from main source file
INCSRCS = lsengine.asm music.asm

# Probably no need to change anything below this line

TAP = $(PROGGY).tap
BIN = $(PROGGY).bin
ZXTAP = ../../ZX_Tap/zxtap

ASDIR = ../../z80asm
AS = $(ASDIR)/z80asm

CFLAGS = -O2 -W -Wall

all: $(TAP)

$(TAP): $(BIN) $(ZXTAP)
	$(ZXTAP) -o $@ $(BIN)

$(BIN): $(SRC) $(AS) $(INCSRCS)
	$(AS) -o $@ $(SRC)

xm2lsengine: xm2lsengine.o
	$(CC) $(LDFLAGS) -o $@ xm2lsengine.o $(LIBS)

.PHONY: clean

clean:
	rm -f $(TAP) $(BIN) xm2lsengine xm2lsengine.o *~
