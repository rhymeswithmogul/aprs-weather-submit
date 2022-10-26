/*
 aprs-weather-submit
 Copyright (c) 2019-2022 Colin Cogle <colin@colincogle.name>
 
 This file, help.h, is part of aprs-weather-submit.
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
