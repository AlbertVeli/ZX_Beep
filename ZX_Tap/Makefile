# (k) ALL RIGHTS REVERSED - Reprint what you like
#
# Sweetmorn, the 55th day of Confusion in the YOLD 3179
#
# Albert Veli

PROGGY = zxtap

OBJS = basic.o parse_cmdline.o zxtap.o

CFLAGS = -g -O0 -W -Wall

all: $(PROGGY)

$(PROGGY): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(PROGGY) $(OBJS) *~
