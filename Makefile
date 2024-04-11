
CC=gcc
CXX=g++
CFLAGS=-O3 -march=native -mtune=native -g3

PROGRAMS=nmsplit bmpextract histo bmpfunnel rgb24toycbcr rgb24toycocg \
	     ycocgtorgb24 ycbcrtorgb24 fconstmod wavextract wavfunnel unipatch \
		 aberth mhost pcm16-ulaw pcm16-alaw bef93 rezip ntpget

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -I. -o $@ $< -lm -larchive

%: %.cpp
	$(CXX) $(CFLAGS) -I. -o $@ $<

.PHONY: clean
clean:
	rm -f $(PROGRAMS)

