## Version 1.5.2
<time datetime="2022-08-04T00:45:00-04:00">August 4, 2022</time>
*   Fix a bug where the `--no-comment` flag accidentally did the opposite of what it was supposed to do.
*   Fix a bug where the short option `-Q` was not recognized.
*   Fix a bug where altitude reports (`/A=`) were always included in packets, even when no altitude was specified.
*   Small code change to allow this to compile on PowerPC (ppc64/ppc64le) and other non-x86 architectures.

## Version 1.5.1
<time datetime="2022-06-08T08:31:00-04:00">June 8, 2022</time>

*   Completely rewrite the build script to have a standard `autogen`, `configure`, and `make`.
*   Eliminate compiler errors when using `-Wextra`.
*   Fix a bug where [APRS.fi](https://APRS.fi) would not accept wind gust speeds if it was placed before the temperature.  This is a bug on their end, but it's a painless change over here.  (Thank you, [Menno](https://github.com/pd9mwo)!)

## Version 1.5
<time datetime="2020-09-26T13:19:22-05:00">September 26, 2020</time>

*   Replace the Makefile with an Autoconf script.
*   Replace the `-DNO_APRSIS` make option with the `--disable-aprs-is` configure switch.
*   Add support for disabling assertions with the `--disable-assert` configure switch (though I recommend not using this).
*   Conform with GNU standards by adding a ChangeLog file (plain-text copy of GitHub's CHANGELOG.md), COPYING (a copy of LICENSE), NEWS, and README (a plaintext copy of GitHub's README.md).
*   Add the `--no-comment` option to suppress this app's user agent in the comment section of the packet.
*   Add the `--altitude` (`-A`) option to allow for reporting the location's altitude, in feet above mean sea level.

## Version 1.4

<time datetime="2020-03-08T22:59:19-04:00">March 8, 2020</time>

*   Fixed a bug where latitudes and longitudes were not being converted into <abbr title="Degrees, minutes, seconds">DMS</abbr> *exactly* as they should be, which had been leading to minor position inaccuracies.
*   Made uncompressed positions the default again, because [APRS.fi](https://APRS.fi) does not support weather packets with compressed positions.  [This is a long-standing bug on their web site.](https://groups.google.com/forum/#!searchin/aprsfi/compressed%7Csort:date/aprsfi/uzjdm026NZQ/FqES7V2iAwAJ)  The `-0` and `--uncompressed-positions` options are still accepted for backwards compatibility, but no longer do anything.
*   Better documentation for the compression type byte.
*   Changed the copyright date for the new year.

## Version 1.3.1

<time datetime="2019-09-14T11:46:52-04:00">September 14, 2019</time>

*   Fixed a bug where uncompressed latitudes greater than 90° were truncated.  This was not the default behavior. My apologies to anyone in Asia, Australia, Oceania, and most of the Americas.

## Version 1.3

<time datetime="2019-07-31T01:54:29-04:00">July 31, 2019</time>

*   Added the compile-time option `NO_APRSIS` to strip all Internet-related functionality, just in case you're <em>really</em> low on space.

## Version 1.2.1

<time datetime="2019-07-31T01:13:29-04:00">July 31, 2019</time>

*   Added \<stdint.h> to prevent compiler errors.  (Thank you to UR3QJW for reporting this.)

*   Switched from ANSI C to the C99 standard, in order to keep using `snprintf()`.  Some compilers are stricter than mine and refuse to compile K\&R C with it.  (Another thank you to UR3QJW.)

*   Since we switched to C99, I added some tiny performance enhancements with `restrict` and `inline`.

*   Fixed a pair of bugs where uncompressed packets would fail to encode the location correctly.

*   Fixed a bug where wind speeds would not be reported properly in compressed packets, because math is hard.

*   Added linker options to the Makefile to fix linking errors with GCC 7.

*   Code cleanup.
    \*<strong>Known issue:</strong> when compiling with GCC 7 and newer, there will be many warnings about formatting.  These can be ignored.  Adding `-Wno-format-detection` to the Makefile will quiet it down.  This will be patched in a future release.

## Version 1.2

<time datetime="2019-04-16T03:10:07-04:00">April 16, 2019</time>

*   Added preliminary support for compiling on Windows.
*   Added the option of using APRS uncompressed positions at runtime with the  `--uncompressed-position` switch, rather than a compile-time choice.
*   Removed a dependency on <code>stdint.h</code>.
*   Fixed a bug where impractically large APRS-IS packets would be truncated.  This was unlikely to affect real-world use.

## Version 1.1

<time datetime="2019-01-23T11:18:00-05:00">January 23, 2019</time> -- codename QLF

*   Added support for reporting temperatures in degrees Celsius.
*   Added a Makefile for POSIX systems.
*   Added a man page.
*   Fixed a bug where negative temperatures may cause the packet to fail to encode.
*   Cleaned up some documentation.

## Version 1.0

<time datetime="2019-01-21T00:55:00-05:00">January 21, 2019</time> -- codename QRV

*   Added significant documentation.

## Version 0.3

<time datetime="2019-01-20T23:04:02-05:00">January 20, 2019</time> -- codename QST

*   Wrote help.
*   Added support for actually sending the packet to an APRS-IS IGate.
*   Changed the position and wind data to compressed from uncompressed.  For now, this can only be undone at compile-time.

## Version 0.2

<time datetime="2019-01-18">January 18, 2019</time> -- codename "Strings are hard"

*   Added support for snowfall, as clarified in APRS 1.1's errata.
*   Added support for the APRS 1.2 parameters: radiation, water level, and voltage.

## Version 0.1

<time datetime="2019-01-14T22:15:00-05:00">January 14, 2019</time> -- codename Q

*   Initial private release.
