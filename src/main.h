/*
 aprs-weather-submit version 1.4
 Copyright (c) 2019-2020 Colin Cogle <colin@colincogle.name>
 
 This file, main.h, is part of aprs-weather-submit.
 <https://github.com/rhymeswithmogul/aprs-weather-submit>

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along
with this program.  If not, see <https://www.gnu.org/licenses/agpl-3.0.html>.
*/

#ifndef main_h
#define main_h

/* Program constants. */
#define PROGRAM_NAME "aprs-weather-submit"
#define VERSION      "1.4"

/**
 * usage() -- show some help.
 *
 * @author Colin Cogle
 * @since  0.1
 */
extern void
usage (void);

/**
 * help() -- show even more help.
 *
 * @author Colin Cogle
 * @since  0.1
 */
extern void
help (void);

/**
 * version() -- show the version.
 *
 * @author Colin Cogle
 * @since  0.1
 */
extern void
version (void);

/* A decent buffer size (plus one). */
#ifndef	BUFSIZE
#define BUFSIZE 1025
#endif

#endif /* main_h */
