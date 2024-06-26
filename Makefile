
CC=gcc
CXX=g++
CFLAGS=-O3 -march=native -mtune=native -g3

PROGRAMS=nmsplit bmpextract histo bmpfunnel rgb24toycbcr rgb24toycocg \
	     ycocgtorgb24 ycbcrtorgb24 fconstmod wavextract wavfunnel unipatch \
		 aberth mhost pcm16-ulaw pcm16-alaw bef93 rezip ntpget box-muller

EXTRAPROGS=alpha64/alpha64-x86_64 asm3zip asm3zip-bin-x86_64 fastent gdbnow \
		   gen-ptab ls2mode mkfatfloppy mtx86 npack pcmbits pfilter rfilter rmap \
		   uniqc xor256x86 mkasm

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -I. -o $@ $< -lm -larchive

%: %.cpp
	$(CXX) $(CFLAGS) -I. -o $@ $<

.PHONY: clean install uninstall all
clean:
	rm -f $(PROGRAMS)

install:
	install -d $(DESTDIR)/usr/bin
	install -m 755 $(PROGRAMS) $(DESTDIR)/usr/bin
	install -m 755 $(EXTRAPROGS) $(DESTDIR)/usr/bin

uninstall:
	rm -f $(DESTDIR)/usr/bin/$(PROGRAMS)
	rm -f $(DESTDIR)/usr/bin/$(EXTRAPROGS)


