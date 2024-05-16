/*
 aprs-weather-submit
 Copyright (c) 2019-2024 Colin Cogle <colin@colincogle.name>
 
 This file, main.c, is part of aprs-weather-submit.
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

#include <stdio.h>          /* *printf(), *puts(), and friends */
#include <stdlib.h>         /* atof(), EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>         /* str*cpy() and friends */
#include <math.h>           /* round(), floor() */
#include <stdint.h>         /* uint16_t */

#ifdef _DOS
#include "getopt.h"         /* getopt(), in ./contrib/freegetopt-0.11/ */
#else /* POSIX */
#include <getopt.h>         /* getopt_long() */
#endif

#include "main.h"
#include "help.h"
#include "aprs-wx.h"

#ifdef HAVE_APRSIS_SUPPORT
#include "aprs-is.h"
#endif

#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (a) : (b))
#endif


int
main (const int argc, const char** argv)
{
	char         packetToSend[BUFSIZE] = "";
	char         packetFormat = UNCOMPRESSED_PACKET;
	char         suppressUserAgent = 0;
	signed char  c = '\0';          /* for getopt */

#ifndef _DOS
	int          option_index = 0;  /* for getopt_long() */
#endif
	int          i = 0;
	APRSPacket   packet;
#ifdef HAVE_APRSIS_SUPPORT
	char         username[BUFSIZE] = "";
	char         password[BUFSIZE] = "";
	char         server[NI_MAXHOST] = "";
	uint16_t     port = 0;
#endif

#ifndef _DOS
	static const struct option long_options[] = {
		{"compressed-position",     no_argument,       0, 'C'},
		{"uncompressed-position",   no_argument,       0, '0'},	/* ignored as of v1.4 */
		{"no-comment",              no_argument,       0, 'Q'},
		{"comment",	                required_argument, 0, 'M'},
		{"help",                    no_argument,       0, 'H'},
		{"version",                 no_argument,       0, 'v'},
#ifdef HAVE_APRSIS_SUPPORT
		{"server",                  required_argument, 0, 'I'},
		{"port",                    required_argument, 0, 'o'},
		{"username",                required_argument, 0, 'u'},
		{"password",                required_argument, 0, 'd'},
#endif
		{"callsign",                required_argument, 0, 'k'},
		{"latitude",                required_argument, 0, 'n'},
		{"longitude",               required_argument, 0, 'e'},
		{"altitude",                required_argument, 0, 'A'},
		/*
			The following options are using APRS-standard short options,
			for clarity.  The exception is wind speed, because that's
			overloaded with snowfall ('s').
		 */
		{"wind-direction",          required_argument, 0, 'c'},
		{"wind-speed",              required_argument, 0, 'S'},
		{"gust",                    required_argument, 0, 'g'},
		{"temperature",             required_argument, 0, 't'},
		{"temperature-celsius",     required_argument, 0, 'T'},
		{"rainfall-last-hour",      required_argument, 0, 'r'},
		{"rainfall-last-24-hours",  required_argument, 0, 'p'},
		{"rainfall-since-midnight", required_argument, 0, 'P'},
		{"snowfall-last-24-hours",  required_argument, 0, 's'},
		{"humidity",                required_argument, 0, 'h'},
		{"pressure",                required_argument, 0, 'b'},
		{"luminosity",              required_argument, 0, 'L'},
		{"radiation",               required_argument, 0, 'X'},
		{"water-level-above-stage", required_argument, 0, 'F'}, /* APRS 1.2 */
		{"voltage",                 required_argument, 0, 'V'}, /* APRS 1.2 */
		{"icon",                    required_argument, 0, 'i'},
		{0, 0, 0, 0}
	};
#endif

#ifdef DEBUG
	puts("Compiled with debugging output.\n");
#endif
	packetConstructor(&packet);

	/* Check for --compressed-position early. */
	for (i = 0; i < argc; i++)
	{
		if (strncmp(argv[i], "-C", MAX(2, strlen(argv[i]))) == 0
		    || strncmp(argv[i], "--compressed-position", MAX(21, strlen(argv[i]))) == 0)
		{
			packetFormat = COMPRESSED_PACKET;
			break;
		}
	}

	if (packetFormat == UNCOMPRESSED_PACKET)
	{
		strcpy(packet.windDirection, "...");
		strcpy(packet.windSpeed, "...");
	}

#ifdef _DOS
	while ((c = (char) getopt(argc, (char**)argv, "CH?vI:o:u:d:k:n:e:A:c:S:g:t:T:r:P:p:s:h:b:L:X:F:V:QM:i:")) != -1)
#else
	while ((c = (char) getopt_long(argc, (char* const*)argv, "CHvI:o:u:d:k:n:e:A:c:S:g:t:T:r:P:p:s:h:b:L:X:F:V:QM:i:", long_options, &option_index)) != -1)
#endif
	{
		double x = 0.0;	 /* scratch space */

		switch (c)
		{
			/* Use compressed position (-C | --compressed-position). */
			case 'C':
				/* We handled this before the while loop. */
				break;

			/* Use uncompressed position (-0 | --uncompressed-position). */
			case '0':
				/* This is now the default option as of version 1.4.  This
				 * switch does nothing now, and is only defined for backwards
				 * compatibility.
				 */
				break;

			/* Complete help (-H | --help) */
			case 'H':
			case '?':
				help();
				return EXIT_SUCCESS;

			/* Version information (-v | --version) */
			case 'v':
				version();
				return EXIT_SUCCESS;

#ifdef HAVE_APRSIS_SUPPORT
			/* IGate server name (-I | --server) */
			case 'I':
				snprintf_verify(
					snprintf(server, strlen(optarg)+1, "%s", optarg)
				);
				break;

			/* IGate server port (-o | --port) */
			case 'o':
				x = (double)atoi(optarg);
				if (x <= 0 || x > 65535)
				{
					fprintf(stderr, "%s: argument for option '-%c' was invalid.  Valid port numbers are 1 through 65535.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				port = (unsigned short)x;
				break;

			/* IGate server username (-u | --username) */
			case 'u':
				snprintf_verify(
					snprintf(username, strlen(optarg)+1, "%s", optarg)
				);
				break;

			/* IGate server password (-d | --password) */
			case 'd':
				snprintf_verify(
					snprintf(password, strlen(optarg)+1, "%s", optarg)
				);
				break;
#endif /* HAVE_APRSIS_SUPPORT */

			/* Callsign, with SSID if desired (-k | --callsign) */
			case 'k':
				snprintf_verify(
					snprintf(packet.callsign, 10, "%s", optarg)
				);
				break;

			/* Your latitude, in degrees north (-n | --latitude) */
			case 'n':
				x = atof(optarg);
				if (x < -90 || x > 90)
				{
					fprintf(stderr, "%s: option `-%c' must be between -90 and 90 degrees north.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					if (packetFormat == COMPRESSED_PACKET)
					{
						compressedPosition(packet.latitude, x, IS_LATITUDE);
					}
					else
					{
						uncompressedPosition(packet.latitude, x, IS_LATITUDE);
					}
				}
				break;

			/* Your longitude, in degrees east (-e | --longitude) */
			case 'e':
				x = atof(optarg);
				if (x < -180 || x > 180)
				{
					fprintf(stderr, "%s: option `-%c' must be between -180 and 180 degrees east.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					if (packetFormat == COMPRESSED_PACKET)
					{
						compressedPosition(packet.longitude, x, IS_LONGITUDE);
					}
					else
					{
						uncompressedPosition(packet.longitude, x, IS_LONGITUDE);
					}
				}
				break;

			/* Your altitude, in feet about mean sea level (-A | --altitude) */
			case 'A':
				x = atoi(optarg);
				if (x > 999999)
				{
					fprintf(stderr, "%s: option `-%c' must be less than 999,999 feet.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					snprintf_verify(
						snprintf(packet.altitude, 6, "%05d", (int)x)
					);
				}
				break;

			/* Wind direction, in degrees from true north (-c | --wind-direction) */
			case 'c':
				x = atof(optarg);
				if (x < 0 || x > 360)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 360 degrees.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					if (packetFormat == COMPRESSED_PACKET)
					{
						snprintf_verify(
							snprintf(
								packet.windDirection, 2, "%c",
								compressedWindDirection((int)x % 360)
							)
						);
					}
					else
					{
						snprintf_verify(
							snprintf(
								packet.windDirection, 4, "%03d",
								(int)(round(x)) % 360
							)
						);
					}
				}
				break;

			/* Wind speed in miles per hour (-S | --wind-speed) */
			case 'S':
				x = atof(optarg);
				if (x < 0 || x > 999)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 999 miles per hour.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					if (packetFormat == COMPRESSED_PACKET)
					{
						snprintf_verify(
							snprintf(
								packet.windSpeed, 2, "%c",
						    	compressedWindSpeed((const uint16_t)x)
							)
						);
					}
					else {
						snprintf_verify(
							snprintf(
								packet.windSpeed, 4, "%03d",
								(int)(round(x))
							)
						);
					}
				}
				break;

			/* Wind gust, in miles per hour (-g | --gust) */
			case 'g':
				x = atof(optarg);
				if (x < 0 || x > 999)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 999 miles per hour.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					snprintf_verify(
						snprintf(packet.gust, 4, "%03d", (int)(round(x)))
					);
				}
				break;

			/* Temperature in degrees Fahrenheit (-t | --temperature) */
			case 't':
				x = atof(optarg);
				if (x < -99 || x > 999)
				{
					fprintf(stderr, "%s: option `-%c' must be between -99 and 999 degrees Fahrenheit.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					snprintf_verify(
						snprintf(packet.temperature, 4, "%03d", (int)(round(x)))
					);
				}
				break;

			/* Temperature in degrees Celsius (-T | --temperature-celsius) */
			case 'T':
				x = atof(optarg);
				x = x * 1.8 + 32;
				if (x < -99 || x > 999)
				{
					fprintf(stderr, "%s: option `-%c' must be between -72 and 537 degrees Celsius.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					snprintf_verify(
						snprintf(packet.temperature, 4, "%03d", (int)(round(x)) )
					);
				}
				break;

			/* Rainfall in the past hour, in inches (-r | --rainfall-last-hour) */
			case 'r':
				x = atof(optarg);
				if (x < 0 || x > 9.99)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 9.99 inches.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					rain(packet.rainfallLastHour, x * 100);
				}
				break;

			/* Rainfall in the past 24 hours, in inches (-p | --rainfall-last-day) */
			case 'p':
				x = atof(optarg);
				if (x < 0 || x > 9.99)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 9.99 inches.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					rain(packet.rainfallLast24Hours, x * 100);
				}
				break;
				
			/* Rainfall since midnight, in inches (-P | --rainfall-since-midnight) */
			case 'P':
				x = atof(optarg);
				if (x < 0 || x > 9.99)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 9.99 inches.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					rain(packet.rainfallSinceMidnight, x * 100);
				}
				break;

			/* (APRS 1.1) Snowfall in the last 24 hours, in inches (-s | --snowfall) */
			case 's':
				x = atof(optarg);
				if (x < 0 || x > 999)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 999 inches.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					/* According to the APRS 1.1 errata, if we have more than
					 * ten inches, remove the decimal part. APRS doesn't give us
					 * enough resolution to report it.
					 * http://www.aprs.org/aprs11/spec-wx.txt
					 */
					if (x > 10)
					{
						snprintf_verify(
							snprintf(
								packet.snowfallLast24Hours, 4, "%03d",
								(unsigned short)floor(x)
							)
						);
					}
					else if (x >= 9.95)
					{
						/* This avoids the previous case from printing "10.",
						   while keeping the accuracy of the other two cases. */
						snprintf_verify(
							snprintf(packet.snowfallLast24Hours, 4, "%03d", 10)
						);
					}
					else
					{
						snprintf_verify(
							snprintf(packet.snowfallLast24Hours, 4, "%1.1f", x)
						);
					}
				}
				break;

			/* Humidity, in the range from 1 to 100% (-h | --humidity) */
			case 'h':
				x = atoi(optarg);
				if (x < 0 || x > 99)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0%% and 100%%.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					unsigned short int h = (unsigned short)(round(x));
					/* APRS only supports values 1-100. Round 0% up to 1%. */
					if (h == 0) {
						h = 1;
					}
					/* APRS requires us to encode 100% as "00". */
					else if (h == 100) {
						h = 0;
					}

					snprintf_verify( snprintf(packet.humidity, 3, "%.2d", h) );
				}
				break;

			/* Barometric pressure, in millibars or hectopascals (-b | --pressure) */
			case 'b':
				x = atof(optarg);
				if (x < 0 || x > 9999.9)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 9999.9 millibars.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					snprintf_verify(
						snprintf(
							packet.pressure, 6, "%.5d", (int)(round(x * 10))
						)
					);
				}
				break;

			/* Luminosity, in watts per square meter (-L | --luminosity) */
			case 'L':
				x = atof(optarg);
				if (x < 0 || x > 1999) {
					fprintf(stderr, "%s: option `-%c' must be between 0 and 1999 watts per square meter.\n", argv[0], optopt);
					return EXIT_FAILURE;
				} else {
					/* This is where it gets weird.  APRS supports readings of
					 * up to 2,000 W/m^2, but only gives us three digits' 
					 * resolution to use.  Values under 1000 are encoded as
					 * "L000" and values over 1000 are encoded as "l000".
					 */
					snprintf_verify(
						snprintf(packet.luminosity, 5, "L%.3d", (int)(x) % 1000)
					);

					if (x > 999) {
						packet.luminosity[0] = 'l';
					}
				}
				break;

			/* (APRS 1.2) Radiation in nanosieverts (-X | --radiation) */
			case 'X':
				x = atof(optarg);
				if (x < 0 || x > 99000000) {
					fprintf(stderr, "%s: option `-%c' must be between 0 and 99 billion nSv.\n", argv[0], optopt);
					return EXIT_FAILURE;
				} else {
					unsigned short magnitude = 0;
					for (; x > 100; magnitude++) {
						x /= 10;
					}
					snprintf_verify(
						snprintf(
							packet.radiation, 4, "%.2d%d", (unsigned short)x,
							magnitude
						)
					);
				}
				break;

			/* (APRS 1.2) Flooding (-F | --water-level-above-stage) */
			case 'F':
				x = atof(optarg);
				if (x < -99.9 || x > 99.9) {
					fprintf(stderr, "%s: option `-%c' must be between -99.9 and 99.9 feet.\n", argv[0], optopt);
					return EXIT_FAILURE;
				} else {
					snprintf_verify(
						snprintf(packet.waterLevel, 4, "%.3d", (short)x * 10)
					);
				}
				break;

			/* (APRS 1.2) Weather station battery voltage (-V | --voltage) */
			case 'V':
				x = atof(optarg);
				if (x < 0 || x > 99.9)
				{
					fprintf(stderr, "%s: option `-%c' must be between 0 and 99.9 volts.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				else
				{
					snprintf_verify(
						snprintf(packet.voltage, 4, "%.3d", (short)x * 10)
					);
				}
				break;

			/* -Q | --no-comment: Suppress our user agent, if desired. */
			case 'Q':
				suppressUserAgent = 1;
				break;
			
			/* -M | --comment: Add comment to packet.
				The macro MAX_COMMENT_LENGTH is defined in "aprs-wx.h".
			*/
			case 'M':
				snprintf_verify(
					snprintf(packet.comment, (size_t)MAX_COMMENT_LENGTH, "%s", optarg)
				);

				/* This comparison might not be Unicode-safe.  However, I don't
				   believe I've seen any APRS-IS packets with emoji, so we might
				   be able to get away with this. */
				if (strlen(optarg) > MAX_COMMENT_LENGTH)
				{
					fprintf(stderr, "Your comment was %zu characters long, but APRS allows %zu characters.  Your comment was truncated.\n", strlen(optarg), (size_t)MAX_COMMENT_LENGTH);
				}
				break;
			
			/* -i | --icon: the icon to use on the map.
				This value must map to something on the APRS symbol table.
			*/
			case 'i':
				if (strlen(optarg) != 2)
				{
					fprintf(stderr, "%s: option '-%c' is not a valid icon.\n", argv[0], optopt);
					return EXIT_FAILURE;
				}
				if (optarg[0] != '/' && optarg[0] != '\\')
				{
					fprintf(stderr, "%s: the symbol table identifier '%c' is not valid.\n", argv[0], optarg[0]);
					return EXIT_FAILURE;
				}
				if (optarg[1] < 33 || optarg[1] > 126)
				{
					fprintf(stderr, "%s: the symbol code '%c' is not in the APRS symbol table.\n", argv[0], optarg[1]);
					return EXIT_FAILURE;
				}

				/*snprintf_verify(
					snprintf(packet.icon, 2, "%s", optarg)
				);*/
				packet.icon[0] = optarg[0];
				packet.icon[1] = optarg[1];
				break;
				
			/* Unknown argument handler (quick help). */
			default:
				usage();
				return EXIT_FAILURE;
		}
	}

	/*
	 * Check for mandatory parameters.
	 * If any are missing, show usage() and quit.
	 */
	if (strlen(packet.callsign) == 0
	    || strlen(packet.latitude) == 0
	    || strlen(packet.longitude) == 0
	) {
		usage();
		return EXIT_FAILURE;
	}

	/* Create the APRS packet. */
	printAPRSPacket(&packet, packetToSend, packetFormat, suppressUserAgent);

#ifdef HAVE_APRSIS_SUPPORT
	/*
	 * If we specified all of the server information, send the packet.
	 * Otherwise, print the packet to stdout and let the user deal with it.
	 */
	if (strlen(server) && strlen(username) && strlen(password) && port != 0)
	{
		sendPacket(server, port, username, password, packetToSend);
	}
	else
	{
#endif
		fputs(packetToSend, stdout);
#ifdef HAVE_APRSIS_SUPPORT
	}
#endif
	
	return EXIT_SUCCESS;
}

/**
 * snprintf_verify(x) -- verify that a call to snprintf() worked.
 *     If x >= 0, then that means snprintf() was able to put all of its data
 *     into the string. If x <= BUFSIZE, that means our buffer was large enough
 *     to hold all our data.  If either check fails, we just overflowed a buffer
 *     and should terminate execution.
 * 
 * @author Colin Cogle
 * @brief  Verify that the call to snprintf() returned a value between 0 and BUFSIZE.
 * @param  x  Return value of snprintf().
 * @since  1.6
 */
#ifndef _DOS
inline
#endif
void
snprintf_verify (const int x)
{
	if (x < 0 || x > BUFSIZE)
	{
		exit(EXIT_FAILURE);
	}
	return;
}
