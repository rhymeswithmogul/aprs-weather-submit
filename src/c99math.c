/*
 aprs-weather-submit
 Copyright (c) 2019-2024 Colin Cogle <colin@colincogle.name>

 This file, c99math.h, is part of aprs-weather-submit.
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

/*
	This file contains definitions for some C99 <math.h> functions that
	may not be defined by all implementations.  Namely, the OpenWatcom
	C library does not include these.  We are using #ifndef directives
	to make sure that we only re-define missing functions.
*/

#ifndef _APRSWX_C99MATH_H
#define _APRSWX_C99MATH_H

#ifdef  _DOS
#include <math.h>	/* log() */

#ifndef logf
/**
 * @brief	An implementation of C99's logf().
 * @param x	A number.
 * @return	float
 * @since	1.6
 */
float
logf (const float x)
{
	return ((float) log((double) x));
}
#endif


#ifndef log1p
/**
 * @brief	An implementation of C99's log1p().
 * @param x	A number.
 * @return	double
 * @since	1.6
 */
double
log1p (const double x)
{
	return log(1+x)-(((1+x)-1)-x)/(1+x);
}
#endif


#ifndef round
/**
 * @brief	An implementation of C99's round().
 * @param x	A number to round up or down.
 * @return	int
 * @since	1.6
 */
int
round (const double x)
{
	if (x < 0.0)
	{
		return (int)(x - 0.5);
	}
	else
	{
		return (int)(x + 0.5);
	}
}
#endif /* round */

#endif /* _DOS */

/* The following line does nothing. It is present so that ISO-compliant
   compilers will not complain about this file being empty (which they
   will if _DOS is not defined at compile time. */
typedef int make_iso_compilers_happy;

#endif /* _APRSWX_C99MATH_H */
