/*
 aprs-weather-submit
 Copyright (c) 2019-2024 Colin Cogle <colin@colincogle.name>
 
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

/* Program constants that should be set by ./configure. */
#ifndef PACKAGE
#define PACKAGE "aprs-weather-submit"
#endif

#ifndef VERSION
#define VERSION "1.7.2"
#endif

/* We don't support networking on DOS at this time.
   On the off-chance the Makefile didn't unset this, this will.
   (If you know how to get TCP/IP working on DOS, feel free to submit a
   patch request!)
*/
#ifdef _DOS
#undef HAVE_APRSIS_SUPPORT
#endif

/* As we begin our function definitions, you'll notice that "extern" is inside
   an #ifndef statement everywhere it occurs. This is because OpenWatcom's POSIX
   linker doesn't recognize these functions when they're marked as extern.  So,
   if this is a DOS build, the user is likely using OpenWatcom, so we'll ignore
   these externs and the inlines in aprs-wx.c.	*/

/**
 * usage() -- show some help.
 *
 * @author Colin Cogle
 * @since  0.1
 */
#ifndef _DOS
extern
#endif
void
usage (void);

/**
 * help() -- show even more help.
 *
 * @author Colin Cogle
 * @since  0.1
 */
#ifndef _DOS
extern
#endif
void
help (void);

/**
 * version() -- show the version.
 *
 * @author Colin Cogle
 * @since  0.1
 */
#ifndef _DOS
extern
#endif
void
version (void);

/**
 * snprintfVerify(x) -- verify that a call to snprintf() worked.
 *     If x >= 0, then that means snprintf() was able to put all of its
 *     data into the string.  Otherwise, we just overflowed a buffer and
 *     should terminate execution.
 * 
 * @author Colin Cogle
 * @brief  Verify that the call to snprintf() returned a positive value.
 * @param  x  Return value of snprintf().
 * @since  1.6
 */
#ifndef _DOS
extern
#endif
void
snprintf_verify(const int x);

/*
	A decent buffer size (plus one) for use in various places around this app.
	When compiling without APRS-IS support, we will be using the buffer for user
	and intermediate inputs;  as such, we can make this much smaller.  This is
	required to prevent a stack overflow on FreeDOS.
*/
#ifndef	BUFSIZE
	#ifdef HAVE_APRSIS_SUPPORT
		#define BUFSIZE 1025
	#else
		#define BUFSIZE 257
	#endif
#endif

#endif /* main_h */
