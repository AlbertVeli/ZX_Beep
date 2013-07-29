# (k) ALL RIGHTS REVERSED - Reprint what you like
#
# Prickle-Prickle, the 53rd day of Confusion in the YOLD 3179
#
# Albert Veli

PROGGY = furelise

# Main source file
SRC = $(PROGGY).asm

# Sources included from main source file
INCSRCS = beep_song.asm octaves_c.asm

# Probably no need to change anything below this line

TAP = $(PROGGY).tap
BIN = $(PROGGY).bin
ZXTAP = ZX_Tap/zxtap
# zxtap options
REMS = \
-r "   __" \
-r "  |--|         _" \
-r "  |  |        |~" \
-r " () ()        |" \
-r "        |\   ()" \
-r "        | \\" \
-r "       () |" \
-r "          |" \
-r "         ()"
SCREEN = ZX_Tap/examples/beethoven.scr
ZXTOPTS = $(REMS) -s $(SCREEN) -b 0 -p 7 -i 0

ASDIR = z80asm
AS = $(ASDIR)/z80asm

# Flags (for freqs.c)
CFLAGS = -O2 -W -Wall

all: $(TAP)

$(TAP): $(BIN) $(ZXTAP)
	$(ZXTAP) -o $@ $(ZXTOPTS) $(BIN)

$(BIN): $(SRC) $(AS) $(INCSRCS)
	$(AS) -o $@ $(SRC)

$(ZXTAP):
	make -C ZX_Tap

$(AS): $(AS).c
	make -C $(ASDIR)

$(AS).c:
	@echo ""
	@echo "It looks like the z80asm submodule is missing!"
	@echo "I will try to fetch it for you."
	@echo "If this fails, run the following commands manually:"
	@echo ""
	@echo "  git submodule init"
	@echo "  git submodule update"
	@echo ""
	git submodule init
	git submodule update

freqs: freqs.o
	$(CC) $(LDFLAGS) -o $@ freqs.o $(LIBS)

.PHONY: clean distclean

clean:
	rm -f $(TAP) $(BIN) freqs freqs.o *~

distclean: clean
	make -C ZX_Tap clean
	make -C $(ASDIR) clean
