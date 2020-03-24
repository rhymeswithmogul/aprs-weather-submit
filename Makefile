# aprs-weather-submit version 1.4
# Copyright (c) 2019-2020 Colin Cogle <colin@colincogle.name>
#
# This file, Makefile, is part of aprs-weather-submit.
# <https://github.com/rhymeswithmogul/aprs-weather-submit>
#
# aprs-weather-submit is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# aprs-weather-submit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with aprs-weather-submit. If not, see <https://www.gnu.org/licenses/>.

# Application name
APPNAME=aprs-weather-submit

# The compiler.
# This means to just use the system default, like GCC or Clang.
CC=cc

#
# Compiler flags:
#  -I.                      Look in the current folder for headers.
#  -std=gnu99               Compile C99 with GNU extensions.
#  -Wall                    Complain heavily.
#  -Wno-format-truncation   ...but limit warnings -- add this on GCC 7+.
#  -Os                      Optimize for size.
#  -s                       Strip the binary (Linux).
#  -DNO_APRSIS              Compile without APRS-IS support -- add this if desired.
#
CFLAGS=-I. -std=gnu99 -Wall -Os

#
# Linker flags:
#  -lm    Include <math.h>
#
LDFLAGS=-lm

# Some variables.
SRCS = src/main.c src/aprs-wx.c src/aprs-is.c
DEPS = $(SRCS:.c=.h)
OBJS = $(SRCS:.c=.o)
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# Default target is "all"
default: all

all: $(SRCS) $(APPNAME)

$(APPNAME): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $(APPNAME) $(LDFLAGS)

main.o:    src/main.c src/main.h src/aprs-is.h src/aprs-wx.h
	$(CC) $(CFLAGS) -c src/main.c

aprs-is.o: src/aprs-is.c src/aprs-is.h src/main.h
	$(CC) $(CFLAGS) -c src/aprs-is.c

aprs-wx.o: src/aprs-wx.c src/aprs-wx.h src/main.h
	$(CC) $(CFLAGS) -c src/aprs-wx.c

.PHONY: clean

clean:
	rm -f $(OBJS) *~ $(APPNAME)

install:
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(APPNAME) $(DESTDIR)$(PREFIX)/bin/$(APPNAME)
	install -d $(DESTDIR)$(PREFIX)/share/man/man1
	install -m 644 man/$(APPNAME).1.man $(DESTDIR)$(PREFIX)/share/man/man1/$(APPNAME).1