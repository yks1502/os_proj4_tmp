CROSS_COMPILE = arm-linux-gnueabi-
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
CFLAGS := -Wall -Werror -g
INCLUDE = -I. -I../linux-3.10-artik/usr/include -I../linux-3.10-artik/include -I../linux-3.10-artik/arch/arm/include
LDFLAGS := -static $(INCLUDE)

all: file_loc gpsupdate

file_loc: file_loc.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

gpsupdate: gpsupdate.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f file_loc.o file_loc gpsupdate.o gpsupdate

.PHONY: clean