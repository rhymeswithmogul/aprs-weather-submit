/*
 aprs-weather-submit version 1.1
 Copyright (c) 2019 Colin Cogle
 
 This file, aprs.h, is part of aprs-weather-submit.
 
 aprs-weather-submit is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 aprs-weather-submit is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with aprs-weather-submit. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef aprs_wx_h
#define aprs_wx_h

typedef struct APRSPacket {
	char  callsign[10]; /* max. six for callsign, max. three for SSID */
#ifdef COMPRESS_POSITION
	char  latitude[5];
	char  longitude[5];
	char  windDirection[2];
	char  windSpeed[2];
#else
	char  latitude[9];
	char  longitude[10];
	char  windDirection[4];
	char  windSpeed[4];
#endif
	char  gust[4];
	char  temperature[4];
	char  rainfallLastHour[4];
	char  rainfallLast24Hours[4];
	char  rainfallSinceMidnight[4];
	char  snowfallLast24Hours[4];
	char  humidity[3];
	char  pressure[6];
	char  luminosity[5]; /* We're keeping an extra digit for l/L conversion */
	char  radiation[4];
	char  waterLevel[5];
	char  voltage[4];
} APRSPacket;

#define IS_LATITUDE 0
#define IS_LONGITUDE 1

/**
 * packetConstructor() -- put some default values into an APRSPacket
 *
 * @author	Colin Cogle
 * @param p	The struct to instantiate.
 * @since 0.1
 */
void packetConstructor(APRSPacket* const p);

/**
 * rain() -- format a rainfall measurement
 *
 * @author			Colin Cogle
 * @param pResult	A constant pointer to the return value.
 * @param precip	A constant representing how much precipiation precipitated.
 * @since			0.2
 */
void rain(char* const pResult, const float precip);

/**
 * notNull() -- return !0 if the user specified a meaningful value.
 *
 * Return !0 if there is a numerical value for this parameter.  If the user hasn't specified
 * a value, then packetConstructor() would have filled this with dots, so return 0.
 *
 * @author		Colin Cogle
 * @param val	A constant pointer to the raw value, which will remain constant.
 * @return		0 if this value is unspecified/not meaningful; !0 otherwise.
 * @since		0.2
 */
int notNull(const char* const val);

/**
 * printAPRSPacket() -- create a textual representation of an APRS weather packet.
 *
 * @author		Colin Cogle
 * @param p		A constant pointer to an APRS packet of type (struct APRSPacket).
 * @param ret	A constant pointer to a string that will hold the return value.
 * @since		0.1
 */
void printAPRSPacket(APRSPacket* const p, char* const ret);

#ifdef COMPRESS_POSITION
/**
 * compressedPosition() -- return an APRS-compressed latitude or longitude value.
 *
 * @author				Colin Cogle
 * @param pResult		A constant pointer to the return value.
 * @param decimal		The latitude or longitude value, in decimal-formatted degrees.
 * @param isLongitude	The constant IS_LATITUDE or IS_LONGITUDE.
 * @since				0.2
 */
void compressedPosition(char* const pResult, const float decimal, const char isLongitude);

/**
 * compressedWindDirection() -- return an APRS-compressed wind direction.
 *
 * This value will fill the APRS direction/heading value.
 *
 * @author				Colin Cogle
 * @param direction		The direction in which the wind is blowing, in degrees from true north.
 * @since				0.2
 */
char compressedWindDirection(const unsigned short direction);

/**
 * compressedWindSpeed() -- return an APRS-compressed wind speed.
 *
 * This value will fill the APRS speed value.
 *
 * @author		Colin Cogle
 * @param speed	The wind speed, in miles per hour.
 * @since		0.2
 */
char compressedWindSpeed(const unsigned short speed);

#else /* COMPRESS_POSITION; that is, if the compiler wants uncompressed positions. */

/**
 * uncompressedPosition() -- return an APRS-uncompressed latitude or longitude value.
 *
 * @author				Colin Cogle
 * @param pResult		A constant pointer to the return value.
 * @param decimal		The latitude or longitude value, in decimal-formatted degrees.
 * @param isLongitude	The constant IS_LATITUDE or IS_LONGITUDE.
 * @since				0.2
 */
void uncompressedPosition(char* const pResult, float decimal, const char isLongitude);

#endif /* COMPRESS_POSITION */
#endif /* aprs_wx_h */
