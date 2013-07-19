# All Rights Reversed - No Rights Reserved

OBJS = bin2tap.o
#CFLAGS = -O2 -W -Wall -DPRINT_RETCODE
CFLAGS = -O2 -W -Wall

bin2tap: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean

clean:
	rm -f bin2tap $(OBJS) *~
