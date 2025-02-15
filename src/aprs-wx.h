/*
 aprs-weather-submit
 Copyright (c) 2019-2025 Colin Cogle <colin@colincogle.name>
 
 This file, aprs-wx.h, is part of aprs-weather-submit.
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

#ifndef aprs_wx_h
#define aprs_wx_h

/* The maximum comment for some packet types is 43 characters,
   according to APRS 1.01.  */
#define MAX_COMMENT_LENGTH 43

typedef struct APRSPacket
{
	char callsign[10]; /* callsign (strlen <= 6), dash, SSID (strlen <= 2) */
	char latitude[9];
	char longitude[10];
	char altitude[6];
	char windDirection[4];
	char windSpeed[4];
	char gust[4];
	char temperature[4];
	char rainfallLastHour[4];
	char rainfallLast24Hours[4];
	char rainfallSinceMidnight[4];
	char snowfallLast24Hours[4];
	char humidity[3];
	char pressure[6];
	char luminosity[5]; /* We're keeping an extra digit for l/L conversion */
	char radiation[4];
	char waterLevel[5];
	char voltage[4];
	char deviceType[3];
	char comment[MAX_COMMENT_LENGTH + 1];
	char icon[3];
} APRSPacket;

#define IS_LATITUDE  0
#define IS_LONGITUDE 1

#define UNCOMPRESSED_PACKET 0
#define COMPRESSED_PACKET 1

/* As we begin our function definitions, you'll notice that "extern" is inside
   an #ifndef statement everywhere it occurs. This is because OpenWatcom's POSIX
   linker doesn't recognize these functions when they're marked as extern.  So,
   if this is a DOS build, the user is likely using OpenWatcom, so we'll ignore
   these externs and the inlines in aprs-wx.c.	*/

/**
 * packetConstructor() -- put some default values into an APRSPacket
 *
 * @author  Colin Cogle
 * @param p The struct to instantiate.
 * @since 0.1
 */
void
packetConstructor (APRSPacket* const p);

/**
 * rain() -- format a rainfall measurement
 *
 * @author        Colin Cogle
 * @param pResult A constant pointer to the return value.
 * @param precip  A constant representing how much precipitation precipitated.
 * @since         0.2
 */
#ifndef _DOS
extern
#endif
void
rain (char* const pResult, const double precip);

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
extern
#endif
int
notNull (const char* const val);

/**
 * printAPRSPacket() -- create a textual representation of an APRS weather packet.
 *
 * @author                  Colin Cogle
 * @param p                 A pointer to an APRS packet (struct APRSPacket).
 * @param ret               A pointer to a string that will hold the return value.
 * @param compressedPacket  The constant COMPRESSED_PACKET or UNCOMPRESSED_PACKET.
 * @param suppressUserAgent If !=0, don't put the app name/version in the comment
 *                          field.
 */
void
printAPRSPacket (
	APRSPacket* restrict const p,
	char* restrict const ret,
	char compressedPacket,
	const char suppressUserAgent
);

/**
 * compressedPosition() -- return an APRS-compressed latitude or longitude value.
 *
 * @author             Colin Cogle
 * @param pResult      A constant pointer to the return value.
 * @param decimal      The latitude or longitude, in decimal-formatted degrees.
 * @param isLongitude  The constant IS_LATITUDE or IS_LONGITUDE.
 * @since              0.2
 */
void
compressedPosition (char* const pResult, const double decimal,
                    const char isLongitude);

/**
 * compressedWindDirection() -- return an APRS-compressed wind direction.
 *
 * This value will fill the APRS direction/heading value.
 *
 * @author          Colin Cogle
 * @param direction The direction in which the wind is blowing, in degrees from
 *                  true north.
 * @return char     A single character representing the wind direction.
 * @since           0.2
 */
#ifndef _DOS
extern
#endif
char
compressedWindDirection (const unsigned short direction);

/**
 * compressedWindSpeed() -- return an APRS-compressed wind speed.
 *
 * This value will fill the APRS speed value.
 *
 * @author       Colin Cogle
 * @param speed  The wind speed, in miles per hour.
 * @return char  A single character representing the wind speed.
 * @since        0.2
 */
#ifndef _DOS
extern
#endif
char
compressedWindSpeed (const unsigned short speed);

/**
 * uncompressedPosition() -- return an APRS-uncompressed latitude or longitude
 * value.
 *
 * @author            Colin Cogle
 * @param pResult     A constant pointer to the return value.
 * @param decimal     The latitude or longitude, in decimal-formatted degrees.
 * @param isLongitude The constant IS_LATITUDE or IS_LONGITUDE.
 * @since             0.2
 */
void
uncompressedPosition (char* const pResult, double decimal,
                      const char isLongitude);

#endif /* aprs_wx_h */
