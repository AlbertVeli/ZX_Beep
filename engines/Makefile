# (k) ALL RIGHTS REVERSED - Reprint what you like
#
# Boomtime, the 66th day of Confusion in the YOLD 3179
#
# Albert Veli



ASDIR = ../z80asm
AS = $(ASDIR)/z80asm
ZXTAP = ../ZX_Tap/zxtap

all: lsengine/lstest.tap poww/poww.tap

lsengine/lstest.tap: $(AS) $(ZXTAP)
	make -C lsengine

poww/poww.tap: $(AS) $(ZXTAP)
	make -C poww

$(AS): $(AS).c
	make -C $(ASDIR)

$(ZXTAP):
	make -C ../ZX_Tap

.PHONY: clean

clean:
	rm -f *~
	make -C lsengine clean
	make -C poww clean
