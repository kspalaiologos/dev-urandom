
CFLAGS=-O2 -g3

PROGRAMS=nmsplit bmpextract histo bmpfunnel rgb24toycbcr rgb24toycocg

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -I. -o $@ $<

.PHONY: clean
clean:
	rm -f $(PROGRAMS)

