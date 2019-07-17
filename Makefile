# Application name
APPNAME=aprs-weather-submit

# The compiler.
# This means to just use the system default, like GCC or Clang.
CC=cc

#
# Compiler flags:
#  -I.		look in the current folder for headers.
#  -std=c99	compile C99, old enough to be portable, new enough for snprintf()
#  -Wall	complain heavily
#  -Os		optimize for size
#
CFLAGS=-I. -std=c99 -Wall -Os

# Some variables.
SRCS = main.c aprs-wx.c aprs-is.c
DEPS = $(SRCS:.c=.h)
OBJS = $(SRCS:.c=.o)
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# Default target is "all"
default: all

all: $(SRCS) $(APPNAME)

$(APPNAME): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $(APPNAME)

main.o:    main.c main.h aprs-is.h aprs-wx.h
	$(CC) $(CFLAGS) -c main.c

aprs-is.o: aprs-is.c aprs-is.h main.h
	$(CC) $(CFLAGS) -c aprs-is.c

aprs-wx.o: aprs-wx.c aprs-wx.h main.h
	$(CC) $(CFLAGS) -c aprs-wx.c

.PHONY: clean

clean:
	rm -f $(OBJS) *~ $(APPNAME)

install:
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(APPNAME) $(DESTDIR)$(PREFIX)/bin/$(APPNAME)
	install -d $(DESTDIR)$(PREFIX)/share/man/man1
	install -m 644 $(APPNAME).1.man $(DESTDIR)$(PREFIX)/share/man/man1/$(APPNAME).1

