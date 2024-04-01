
CFLAGS=-O2 -g3

PROGRAMS=nmsplit bmpextract histo bmpfunnel rgb24toycbcr rgb24toycocg \
	     ycocgtorgb24 ycbcrtorgb24 fconstmod

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -I. -o $@ $<

.PHONY: clean
clean:
	rm -f $(PROGRAMS)

