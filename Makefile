
CFLAGS=-O2 -g3

PROGRAMS=nmsplit bmpextract histo bmpfunnel

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -I. -o $@ $<

.PHONY: clean
clean:
	rm -f $(PROGRAMS)

