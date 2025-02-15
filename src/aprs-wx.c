/*
 aprs-weather-submit
 Copyright (c) 2019-2025 Colin Cogle <colin@colincogle.name>

 This file, aprs-wx.c, is part of aprs-weather-submit.
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

#include <stdio.h>    /* fprintf(), printf(), snprintf(), fputs() */
#include <string.h>   /* strcpy(), strcat(), strlen() */
#include <math.h>     /* floor(), round(), pow(), fabs() */
#include <time.h>     /* struct tm, time_t, time(), gmtime_r() */
#include <stdint.h>   /* uint32_t */

#include "main.h"     /* PACKAGE, VERSION, BUFSIZE, snprintf_verify() */
#include "aprs-wx.h"
#include "c99math.h"  /* logf(), log1p(), and round() as needed , for
                         incomplete C99 implementations. */

/* As we begin our function definitions, you'll notice that "inline" is inside
   an #ifndef statement everywhere it occurs. This is because OpenWatcom's POSIX
   linker doesn't recognize these functions when they're marked as inline.  So,
   if this is a DOS build, the user is likely using OpenWatcom, so we'll ignore
   these inlines and the externs in aprs-wx.h.	*/

/**
 * packetConstructor() -- put some default values into an APRSPacket
 *
 * @param p	The struct to instantiate.
 * @since 0.1
 */
void
packetConstructor (APRSPacket* const p)
{
	strcpy(p->callsign, "");
	strcpy(p->latitude, "");
	strcpy(p->longitude, "");
	strcpy(p->altitude, "");
	strcpy(p->windDirection, " ");
	strcpy(p->windSpeed, " ");
	strcpy(p->gust, "..");
	strcpy(p->temperature, "...");
	strcpy(p->rainfallLastHour, "...");
	strcpy(p->rainfallLast24Hours, "...");
	strcpy(p->rainfallSinceMidnight, "...");
	strcpy(p->humidity, "..");
	strcpy(p->pressure, ".....");
	strcpy(p->luminosity, "....");
	strcpy(p->radiation, "...");
	strcpy(p->waterLevel, "....");
	strcpy(p->voltage, "...");
	strcpy(p->snowfallLast24Hours, "...");
	strcpy(p->comment, "");
	strcpy(p->icon, "/_");	/* the default icon, (WX) */
	strcpy(p->deviceType, "");
	return;
}

/**
 * compressedWindSpeed() -- return an APRS-compressed wind speed.
 *
 * This value will fill the APRS speed value, calculated as below.
 * Let R be the return value, and S be speed.
 *
 * Given that:  1.08^R - 1 = S
 * Then:            1.08^R = S + 1
 *            log (1.08^R) = log (S + 1)
 *              R log 1.08 = log (S + 1)
 *                       R = log (S + 1) / log 1.08
 *
 * Then, as per the APRS spec, add 33 to convert it to ASCII.
 *
 * @author       Colin Cogle
 * @param speed  The wind speed, in miles per hour.
 * @since        0.2
 */
#ifndef _DOS
inline
#endif
char
compressedWindSpeed (const unsigned short speed)
{
    return (char)(round(log1p(speed) / logf(1.08)) + 33);
}

/**
 * compressedWindDirection() -- return an APRS-compressed wind direction.
 *
 * This value will fill the APRS direction/heading value.
 *
 * @author          Colin Cogle
 * @param direction The direction in which the wind is blowing, in degrees from
 *                  true north.
 * @since           0.2
 */
#ifndef _DOS
inline
#endif
char
compressedWindDirection (const unsigned short direction)
{
	return (char)(round(direction / 4) + 33);
}

/**
 * compressedPosition() -- return an APRS-compressed latitude or longitude value.
 *
 * @author             Colin Cogle
 * @param pResult      A constant pointer to the return value.
 * @param decimal      The latitude or longitude value, in decimal-formatted
 *                     degrees.
 * @param isLongitude  The constant IS_LATITUDE or IS_LONGITUDE.
 * @since              0.2
 */
void
compressedPosition (char* const pResult, const double decimal,
                    const char isLongitude)
{
	char     pos = 0;       /* position; an iterator */
	uint32_t x   = 190463;  /* magic number for longitude (APRS 1.0 spec, p.38) */

	if (isLongitude == IS_LONGITUDE)
	{
		x = (uint32_t)(x * (180 + decimal));
	}
	else
	{
		/* The magic number for latitude is exactly twice that of longitude,
		 * so that's why we're doubling it here (also on p.38 of the APRS spec).
		 */
		x = (uint32_t)(x * 2 * (90 - decimal));
	}

	for (; pos < 3; pos++)
	{
		uint32_t divisor = (uint32_t)pow(91, 3 - pos);
		uint32_t result  = (uint32_t)floor(x / divisor);
		pResult[(uint32_t)pos] = (char)(result + 33);
		x %= divisor;
	}
	pResult[3] = (char)((x % 91) + 33);
	pResult[4] = '\0';

	return;
}


/**
 * uncompressedPosition() -- return an APRS-uncompressed latitude or longitude
 * value.
 *
 * @author            Colin Cogle
 * @param pResult     A constant pointer to the return value.
 * @param decimal     The latitude or longitude value, in decimal-formatted
 *                    degrees.
 * @param isLongitude The constant IS_LATITUDE or IS_LONGITUDE.
 * @since             0.2
 */
void
uncompressedPosition (char* const pResult, const double decimal,
                      const char isLongitude)
{
	short degrees;
	float minutes = 0;

	if (!isLongitude && (decimal > 90 || decimal < -90))
	{
		degrees = 90;
	}
	else if (isLongitude && (decimal < -180 || decimal > 180))
	{
		degrees = 180;
	}
	else
	{
		float x = fabs(decimal);
		degrees = (short)floor(x);
		minutes = (x - degrees) * 60;
	}

	if (isLongitude == IS_LATITUDE)
	{
		snprintf_verify(snprintf(
			pResult, 9, "%02hi%05.2f%c",
			degrees, minutes, (decimal < 0 ? 'S' : 'N')
		));
	}
	else
	{
		snprintf_verify(snprintf(
			pResult, 10, "%03hi%05.2f%c",
			degrees, minutes, (decimal < 0 ? 'W' : 'E')
		));
	}
	return;
}

/**
 * rain() -- format a rainfall measurement
 *
 * @author        Colin Cogle
 * @param pResult A constant pointer to the return value.
 * @param precip  A constant representing how much precipitation precipitated.
 * @since         0.2
 */
#ifndef _DOS
inline
#endif
void
rain (char* const pResult, const double precip)
{
	snprintf_verify( snprintf(pResult, 4, "%03d", (unsigned short)precip) );
	return;
}

/**
 * notNull() -- return !0 if the user specified a meaningful value.
 *
 * Return !0 if there is a numerical value for this parameter.  If the user
 * hasn't specified a value, then packetConstructor() would have filled this
 * with dots, so return 0.
 *
 * @author    Colin Cogle
 * @param val A constant pointer to the raw value, which will remain constant.
 * @return    0 if this value is unspecified/not meaningful; !0 otherwise.
 * @since     0.2
 */
#ifndef _DOS
inline
#endif
int
notNull (const char* const val)
{
	return val[0] != '.';
}

/**
 * printAPRSPacket() -- create a textual representation of an APRS weather packet.
 *
 * @author                  Colin Cogle
 * @param p                 A constant pointer to an APRS packet of type
 *                          (struct APRSPacket).
 * @param ret               A constant pointer to a string that will hold the
 *                          return value.
 * @param compressedPacket  The constant COMPRESSED_PACKET or UNCOMPRESSED_PACKET.
 * @param suppressUserAgent If !=0, don't put the Linux flag ('X') nor the app
 *                          name and version in the comment field.
 * @since 0.1
 */
void
printAPRSPacket (APRSPacket* restrict const p, char* restrict const ret,
                 char compressPacket, const char suppressUserAgent)
{
	char       result[BUFSIZE] = "\0";
	time_t     t               = time(NULL);
	struct tm  now;
	gmtime_r(&t, &now); /* APRS uses GMT */

	if (compressPacket == COMPRESSED_PACKET)
	{
		/* Compression type byte ("T"):
		 * (GPS fix: current) | (NMEA source: other) | (Origin: software) = 34
		 * Add 33 as per the spec, and you get 67, the ASCII code for 'C'.
		 */
		snprintf_verify(snprintf(
			result, 48,
		/*	 header_________ timestamp____siLNLEscWDWST*/
			"%s>APRS,TCPIP*:@%.2d%.2d%.2dz%c%s%s%c%c%cC",
			p->callsign, now.tm_mday, now.tm_hour, now.tm_min, p->icon[0],
			p->latitude, p->longitude, p->icon[1], p->windDirection[0], p->windSpeed[0]
		));
	}
	else {
		snprintf_verify(snprintf(
			result, 61,
		/*	 header_________ timestamp____LNsiLEscWD/WS*/
			"%s>APRS,TCPIP*:@%.2d%.2d%.2dz%s%c%s%c%s/%s",
			p->callsign, now.tm_mday, now.tm_hour, now.tm_min, p->latitude,
			p->icon[0], p->longitude, p->icon[1], p->windDirection, p->windSpeed
		));
	}

	if (notNull(p->gust))
	{
		strcat(result, "g");
		strcat(result, p->gust);
	}

	/* The temperature field is mandatory. */
	strcat(result, "t");
	strcat(result, p->temperature);

	if (notNull(p->rainfallLastHour))
	{
		strcat(result, "r");
		strcat(result, p->rainfallLastHour);
	}

	if (notNull(p->rainfallLast24Hours))
	{
		strcat(result, "p");
		strcat(result, p->rainfallLast24Hours);
	}

	if (notNull(p->rainfallSinceMidnight))
	{
		strcat(result, "P");
		strcat(result, p->rainfallSinceMidnight);
	}

	if (notNull(p->humidity))
	{
		strcat(result, "h");
		strcat(result, p->humidity);
	}

	if (notNull(p->pressure))
	{
		strcat(result, "b");
		strcat(result, p->pressure);
	}

	if (notNull(p->luminosity))
	{
		/* Remember, the letter is included below. */
		strcat(result, p->luminosity);
	}

	if (notNull(p->radiation))
	{
		strcat(result, "X");
		strcat(result, p->radiation);
	}

	/* F is required by APRS 1.2.1 if voltage or device type are present  */
	if (notNull(p->waterLevel) || notNull(p->voltage) || notNull(p->deviceType))
	{
		strcat(result, "F");
		strcat(result, p->waterLevel);
	}

	if (notNull(p->voltage))
	{
		strcat(result, "V");
		strcat(result, p->voltage);
	}

	if (notNull(p->deviceType))
	{
		strcat(result, "Z");
		strcat(result, p->deviceType);
	}

	if (notNull(p->snowfallLast24Hours))
	{
		/* Add snowfall last, as some sites (aprs.fi) accidentally parse a
		 * period as the start of a comment, and will ignore the weather data
		 * after it.
		 */
		strcat(result, "s");
		strcat(result, p->snowfallLast24Hours);
	}

	if (strlen(p->altitude))
	{
		strcat(result, "/A=");
		strcat(result, p->altitude);
	}

	/* If the user provided a comment, we will use that.  Otherwise, we
	 * will check and see if --no-comment was specified, and _not_ emit
	 * the user agent then.
	 */
	if (strlen(p->comment))
	{
		strcat(result, p->comment);
	}
	else if (suppressUserAgent != 1)
	{
		strcat(result, PACKAGE);
		strcat(result, "/");
		strcat(result, VERSION);
	}

	strcat(result, "\n\0");
	strcpy(ret, result);
	return;
}
