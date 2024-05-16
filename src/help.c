#include <stdio.h>
#include "help.h"
#include "main.h" /* for PACKAGE and VERSION constants */

/* As we begin our function definitions, you'll notice that "extern" is inside
   an #ifndef statement everywhere it occurs. This is because OpenWatcom's POSIX
   linker doesn't recognize these functions when they're marked as extern.  So,
   if this is a DOS build, the user is likely using OpenWatcom, so we'll ignore
   these externs and the inlines in aprs-wx.c.	*/


/**
 * usage() -- show version information.
 *
 * @author Colin Cogle
 * @since  0.1
 */
#ifndef _DOS
inline
#endif
void
version (void)
{
	printf("%s, version %s", PACKAGE, VERSION);
#ifdef DEBUG
	fputs(", compiled with debugging output", stdout);
#endif
#if defined(_DOS)
	fputs(", compiled for DOS without APRS-IS support", stdout);
#elif !defined(HAVE_APRSIS_SUPPORT)
	fputs(", compiled without APRS-IS support", stdout);
#endif

#ifdef _DOS
	/* Per the FreeDOS standards, we should emit one line with license
	 * information. */
	puts(".\n\
Copyright (c) 2019-2024 Colin Cogle.\n\
This program is free software (GNU Affero General Public License v3).");
#else
	puts(".\n\
Copyright (c) 2019-2024 Colin Cogle.\n\
This program comes with ABSOLUTELY NO WARRANTY. This is free software, and you\n\
are welcome to redistribute it under certain conditions.  See the GNU Affero\n\
General Public License (version 3.0) for more details.");
#endif
	return;
}

/**
 * usage() -- show some help.
 *
 * @author Colin Cogle
 * @since  0.1
 */
#ifndef _DOS
inline
#endif
void
usage(void)
{
#ifdef _DOS
	printf("Usage: %s /k CALL /e LAT /n LONG [OTHER PARAMETERS]\n", PACKAGE);
#else
	printf("Usage: %s --callsign [CALLSIGN[-SSID]] --latitude [LATITUDE] --longitude [LONGITUDE] [OTHER PARAMETERS]\n", PACKAGE);
#endif
	return;
}

/**
 * help() -- show even more help.
 *
 * @author Colin Cogle
 * @since  0.1
 */
#ifndef _DOS
inline
#endif
void
help (void)
{
	version();
	puts("");
	usage();
#ifndef _DOS
	puts("\n\
Special parameters:\n\
	-H, --help                 Show this help and exit.\n\
	-v, --version              Show version and licensing information, and exit.\n\
	-C, --compressed-position  Create a packet with the compressed position format.\n\
	-Q, --no-comment           Don't include this app's name and version in the comment field.\n\
\n\
Required parameters:\n\
	-k, --callsign      Your callsign, with SSID if desired.\n\
	-e, --longitude     The longitude of your weather station, in degrees east of the Prime Meridian.\n\
	-n, --latitude      The latitude of your weather station, in degrees north of the equator.\n");
#ifdef HAVE_APRSIS_SUPPORT
	puts("APRS-IS IGate parameters:\n\
	-I, --server        Name of the APRS-IS IGate server to submit the packet to.\n\
	-o, --port          Port that the APRS-IS IGate service is listening on.\n\
	-u, --username      Authenticate to the server with this username.\n\
	-d, --password      Authenticate to the server with this password.\n");
#endif /* HAVE_APRSIS_SUPPORT */
	puts("Optional parameters:\n\
	-A, --altitude                 The altitude of your weather station (in feet above mean sea level).\n\
	-b, --pressure                 Barometric pressure (millibars or hectopascals).\n\
	-c, --wind-direction           Direction that the wind is blowing (degrees).\n\
	-F, --water-level-above-stage  Water level above flood stage or mean tide (feet).\n\
	-g, --gust                     Peak wind speed in the last five minutes (miles per hour).\n\
	-h, --humidity                 Relative humidity (percentage).\n\
	-i, --icon                     Override the default symbol table code and identifier (/_).\n\
	-L, --luminosity               Luminosity (watts per square meter).\n\
	-M, --comment                  Add a custom comment to the packet.\n\
	-p, --rainfall-last-24-hours   Total rainfall in the past 24 hours (inches).\n\
	-P, --rainfall-since-midnight  Total rainfall since midnight local time (inches).\n\
	-r, --rainfall-last-hour       Total rainfall in the past hour (inches).\n\
	-s, --snowfall-last-24-hours   Total snowfall in the past 24 hours (inches).\n\
	-S, --wind-speed               Sustained wind speed in the past minute (miles per hour).\n\
	-t, --temperature              Temperature (degrees Fahrenheit).\n\
	-T, --temperature-celsius      Temperature (degrees Celsius).\n\
	-V, --voltage                  Battery voltage of your weather station.\n\
	-X, --radiation                Radiation levels (nanosieverts per hour).\n\n\
Find this project online at https://github.com/rhymeswithmogul/aprs-weather-submit\n");
#else /* DOS build -- son't forget to wrap lines at 80 characters */
	puts("\n\
Special parameters:\n\
	/?\tShow this help and exit.\n\
	/v\tShow version and licensing information, and exit.\n\
	/C\tCreate a packet with the compressed position format.\n\
	/Q\tDon't include this app's name and version in the comment field.\n\
\n\
Required parameters (each requires an argument):\n\
	/k\tYour callsign, with SSID if desired.\n\
	/e\tLongitude, in degrees east of the Prime Meridian.\n\
	/n\tLatitude, in degrees north of the equator.\n\
\n\
Optional parameters (each requires an argument):\n\
	/A\tAltitude (in feet above mean sea level).\n\
	/b\tBarometric pressure (millibars or hectopascals).\n\
	/c\tDirection that the wind is blowing (degrees).\n\
	/F\tWater level above flood stage or mean tide (feet).\n\
	/g\tPeak wind speed in the last five minutes (miles per hour).\n\
	/h\tRelative humidity (percentage).\n\
	/i\tOverride the default symbol table code and identifier (/_).\n\
	/L\tLuminosity (watts per square meter).\n\
	/M\tAdd a custom comment to the packet.\n\
	/p\tTotal rainfall in the past 24 hours (inches).\n\
	/P\tTotal rainfall since midnight local time (inches).\n\
	/r\tTotal rainfall in the past hour (inches).\n\
	/s\tTotal snowfall in the past 24 hours (inches).\n\
	/S\tSustained wind speed in the past minute (miles per hour).\n\
	/t\tTemperature (degrees Fahrenheit).\n\
	/T\tTemperature (degrees Celsius).\n\
	/V\tBattery voltage of your weather station.\n\
	/X\tRadiation levels (nanosieverts per hour).\n\n\
Find this online at https://github.com/rhymeswithmogul/aprs-weather-submit\n");
#endif
	return;
}
