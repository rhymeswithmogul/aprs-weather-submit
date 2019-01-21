/*
 aprs-weather-submit version 1.0
 Copyright (c) 2019 Colin Cogle
 
 This file, aprs.c, is part of aprs-weather-submit.
 
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

#include <stdio.h>	/* fprintf(), printf(), snprintf(), fputs() */
#include <string.h>	/* strcpy(), strncpy(), strncat(), strlen() */
#include <math.h>	/* floor(), round(), pow(), fabs() */
#include <time.h>	/* struct tm, time_t, time(), gmtime() */
#include <stdint.h>	/* uint32_t */
#include "main.h"	/* PROGRAM_NAME, VERSION */
#include "aprs-wx.h"

/**
 * packetConstructor() -- put some default values into an APRSPacket
 *
 * @param p	The struct to instantiate.
 * @since 0.1
 */
void packetConstructor(APRSPacket* const p) {
	strcpy(p->callsign, "");
	strcpy(p->latitude, "");
	strcpy(p->longitude, "");
#ifdef COMPRESS_POSITION
	strcpy(p->windDirection, " ");
	strcpy(p->windSpeed, " ");
#else
	strcpy(p->windDirection, "...");
	strcpy(p->windSpeed, "...");
#endif
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
}

#ifdef COMPRESS_POSITION
/**
 * compressedWindSpeed() -- return an APRS-compressed wind speed.
 *
 * This value will fill the APRS speed value.
 *
 * @author		Colin Cogle
 * @param speed	The wind speed, in miles per hour.
 * @since		0.2
 */
char compressedWindSpeed(const unsigned short speed) {
	return (char)(pow(1.08, 1/speed) + 32);
}

/**
 * compressedWindDirection() -- return an APRS-compressed wind direction.
 *
 * This value will fill the APRS direction/heading value.
 *
 * @author				Colin Cogle
 * @param direction		The direction in which the wind is blowing, in degrees from true north.
 * @since				0.2
 */
char compressedWindDirection(const unsigned short direction) {
	return (char)(direction / 4 + 33);
}

/**
 * compressedPosition() -- return an APRS-compressed latitude or longitude value.
 *
 * @author				Colin Cogle
 * @param pResult		A constant pointer to the return value.
 * @param decimal		The latitude or longitude value, in decimal-formatted degrees.
 * @param isLongitude	The constant IS_LATITUDE or IS_LONGITUDE.
 * @since				0.2
 */
void compressedPosition(char* const pResult, const float decimal, const char isLongitude) {
	uint32_t x = 190463;
	if (isLongitude == IS_LONGITUDE) {
		x *= (180 + decimal);
	} else {
		x *= 2 * (90 - decimal);
	}
	
	for (char pos = 0; pos < 3; pos++) {
		uint32_t divisor = (uint32_t)pow(91, 3 - pos);
		uint32_t result = floor(x / divisor);
		pResult[(int)pos] = result + 33;
		x %= divisor;
	}
	pResult[3] = (x % 91) + 33;
	pResult[4] = '\0';
}
#else /* COMPRESS_POSITION */

/**
 * uncompressedPosition() -- return an APRS-uncompressed latitude or longitude value.
 *
 * @author				Colin Cogle
 * @param pResult		A constant pointer to the return value.
 * @param decimal		The latitude or longitude value, in decimal-formatted degrees.
 * @param isLongitude	The constant IS_LATITUDE or IS_LONGITUDE.
 * @since				0.2
 */
void uncompressedPosition(char* const pResult, float decimal, const char isLongitude) {
	char			format[16] = "%02d%02u.%02u%c";
	signed short	degrees;
	unsigned short	minutes = 0;
	unsigned short	seconds = 0;
	char			direction = '.';
	
	if (isLongitude == IS_LONGITUDE) {
		format[2] = '3';
		direction = (decimal < 0 ? 'W' : 'E');
	} else {
		direction = (decimal < 0 ? 'S' : 'N');
	}
	
	if (decimal > 90) {
		degrees = 90;
	}
	else if (decimal < -90) {
		degrees = -90;
	} else {
		decimal = fabs(decimal);
		degrees = floor(decimal);
		decimal -= degrees;
		decimal *= 60;
		minutes = floor(decimal);
		seconds = floor((decimal - minutes) * 60);
	}
	
	/* 10 = length of char[] longitude */
	snprintf(pResult, 10, format, degrees, minutes, seconds, direction);
}
#endif /* COMPRESS_POSITION */

/**
 * rain() -- format a rainfall measurement
 *
 * @author			Colin Cogle
 * @param pResult	A constant pointer to the return value.
 * @param precip	A constant representing how much precipiation precipitated.
 * @since			0.2
 */
void rain(char* const pResult, const float precip) {
	snprintf(pResult, 4, "%03d", (unsigned short)precip);
}

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
int notNull(const char* const val) {
	return val[0] != '.';
}

/**
 * printAPRSPacket() -- create a textual representation of an APRS weather packet.
 *
 * @author		Colin Cogle
 * @param p		A constant pointer to an APRS packet of type (struct APRSPacket).
 * @param ret	A constant pointer to a string that will hold the return value.
 * @since		0.1
 */
void printAPRSPacket(APRSPacket* const p, char* const ret) {
	char result[601]; /* one second's worth of data at 1200 baud + \0 */
	time_t t = time(NULL);
	struct tm *now = gmtime(&t); /* APRS uses GMT */
	
#ifdef COMPRESS_POSITION
	/*                    header_________ timestamp____ pos_wc_s_ T__*/
	snprintf(result, 48, "%s>APRS,TCPIP*:@%.2d%.2d%.2dz/%s%s_%c%cCt%s",
			 p->callsign, now->tm_mday, now->tm_hour, now->tm_min,
			 p->latitude, p->longitude, p->windDirection[0], p->windSpeed[0], p->temperature);
#else
	/*                    header_________ timestamp____pos__wc_ s_T__*/
	snprintf(result, 61, "%s>APRS,TCPIP*:@%.2d%.2d%.2dz%s/%s_%s/%st%s",
			 p->callsign, now->tm_mday, now->tm_hour, now->tm_min,
			 p->latitude, p->longitude, p->windDirection, p->windSpeed, p->temperature);
#endif
	if (notNull(p->gust)) {
		strncat(result, "g", 1);
		strncat(result, p->gust, 3);
	}
	if (notNull(p->rainfallLastHour)) {
		strncat(result, "r", 1);
		strncat(result, p->rainfallLastHour, 3);
	}
	if (notNull(p->rainfallLast24Hours)) {
		strncat(result, "p", 1);
		strncat(result, p->rainfallLast24Hours, 3);
	}
	if (notNull(p->rainfallSinceMidnight)) {
		strncat(result, "P", 1);
		strncat(result, p->rainfallSinceMidnight, 3);
	}
	if (notNull(p->humidity)) {
		strncat(result, "h", 1);
		strncat(result, p->humidity, 2);
	}
	if (notNull(p->pressure)) {
		strncat(result, "b", 1);
		strncat(result, p->pressure, 5);
	}
	if (notNull(p->luminosity)) {
		/* Remember, the letter is included below. */
		strncat(result, p->luminosity, 4);
	}
	if (notNull(p->radiation)) {
		strncat(result, "X", 1);
		strncat(result, p->radiation, 3);
	}
	/* F is required by APRS 1.2 if voltage is present  */
	if (notNull(p->waterLevel) || notNull(p->voltage)) {
		strncat(result, "F", 1);
		strncat(result, p->waterLevel, 4);
	}
	if (notNull(p->voltage)) {
		strncat(result, "V", 1);
		strncat(result, p->voltage, 3);
	}
	if (notNull(p->snowfallLast24Hours)) {
		/* Run this at the end.  Some sites (aprs.fi) accidentally process the decimal point as a comment. */
		strncat(result, "s", 1);
		strncat(result, p->snowfallLast24Hours, 3);
	}
	strncat(result, "X", 1);
	strncat(result, PROGRAM_NAME, strlen(PROGRAM_NAME));
	strncat(result, "/", 1);
	strncat(result, VERSION, strlen(VERSION));
	strncat(result, "\n\0", 2);
	strncpy(ret, result, (sizeof(ret) < strlen(result) ? strlen(result) : sizeof(ret)));
}
