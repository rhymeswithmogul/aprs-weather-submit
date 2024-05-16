# aprs-weather-submit
# Copyright (c) 2019-2023 Colin Cogle <colin@colincogle.name>
#
# This file, Makefile.am, is part of aprs-weather-submit.
# <https://github.com/rhymeswithmogul/aprs-weather-submit>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
# details.
#
# You should have received a copy of the GNU Affero General Public License along
# with this program.  If not, see <https://www.gnu.org/licenses/agpl-3.0.html>.

AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = aprs-weather-submit
aprs_weather_submit_SOURCES = src/main.c src/aprs-wx.c src/aprs-is.c src/help.c src/c99math.c
man1_MANS = man/aprs-weather-submit.man

# When compiling for Windows on MinGW, we also need to link with -lws2_32.
# I'm not sure how to do that conditionally just yet.
LDADD = -lm
