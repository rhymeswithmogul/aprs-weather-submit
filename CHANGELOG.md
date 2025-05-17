# Change Log for `aprs-weather-submit`

## Version 2.0.1
Not yet released
- A security change in version 2.0 would not compile on DOS because `gmtime_r()` isn't part of the C99 standard.  Now, for DOS platforms, [we revert to using `gmtime()`](https://github.com/rhymeswithmogul/aprs-weather-submit/issues/36).

## Version 2.0
<time datetime="2025-05-15">May 15, 2025</time>
- [We have our own TOCALL destination!](https://github.com/aprsorg/aprs-deviceid/issues/192)  This app can now identify itself using the `APWXS?` destination, so that the various APRS tracking sites can identify this app without the user agent.  I felt that was worthy of a major version bump!
- In addition to the above, anyone who forks this project can now easily pick their own TOCALL at compile-time by editing the `TOCALL` macro in `main.h` or supplying `-DTOCALL=APZxxx` in `$CFLAGS`.
- The command-line option `--debug` has been added to toggle debugging output at runtime.  [Thanks for the suggestion, DL9SEC!](https://github.com/rhymeswithmogul/aprs-weather-submit/issues/18)
  - As a result, the `--enable-debug` compile-time option (that sets `-DDEBUG`) is removed.
  - However, if you *really* need the smallest-possible binary, there is a new `--disable-debugging` that will remove all debugging capability.
- Added the `Z` device type field, as noted in [**APRS version 1.2.1 WEATHER UPDATES TO THE SPEC**](https://www.aprs.org/aprs12/weather-new.txt).
- Fixed a low-severity [security bug involving the use of `gmtime()`](https://github.com/rhymeswithmogul/aprs-weather-submit/issues/25).  This has the limited potential to be unsafe, so I replaced it with a call to `gmtime_r()`.

## Version 1.8.2
<time datetime="2024-11-13">November 13, 2024</time>
- Fixed a bug where v1.8.1 would not handle timeouts correctly using the Win32 API.  macOS and Linux users are not affected.
- Fixed incorrect behavior in APRS server negotiation.  The `vers` field expects the app name and version, separated by a space;  however, I was separating it with a forward slash instead.  I don't know if this matters or not, but it should still be fixed.

## Version 1.8.1
<time datetime="2024-11-03">November 3, 2024</time>
Fixed a bug where compilation would fail on `armhf` platforms due to an improper cast when `printf()`-ing a `time_t`.  It's a `long long int` on that platform, while `printf()` was expecting a `long int` instead.

## Version 1.8
<time datetime="2024-10-31T22:35:00-04:00">October 31, 2024</time>
*   Converted the APRS-IS socket from blocking to non-blocking, with a default timeout of 15 seconds before the connection is aborted.  Thanks to [DL9SEC](https://www.dl9sec.de) for pointing this out.
*   Added a new parameter, `--timeout`, to set how long the app will wait for the remote server before giving up.
*   Added support for ISO-compliant compilers (e.g., `gcc -pedantic`).
*   Fixed a bug where `./configure --disable-aprs-is` was not honored.
*   Minor code cleanup.

## Version 1.7.2
<time datetime="2024-09-23T09:09:09-04:00">September 23, 2024</time>
*   Fixed a bug that would prevent users from creating APRS packets with 100% relative humidity.  Thanks to Jeff (N9CQS) for the bug report!
*   Minor packaging improvements for Debian repositories.

## Version 1.7.1
<time datetime="2023-11-30T00:54:00-05:00">November 30, 2023</time>
This patch fixes a compile-time bug on armhf platforms.  The format identifer `%zu` is used to get around an error about the size of `size_t`.

## Version 1.7
<time datetime="2023-11-29T23:02:00-05:00">November 29, 2023</time>
*   The icon can now be changed from the default (WX) icon using the new `--icon` option.
*   Output of `snprintf()` is now better checked to make sure we haven't overflowed our buffer.
*   Fixed a compile-time error on armhf platforms.

## Version 1.6
<time datetime="2022-10-26T22:53:17-04:00">October 26, 2022</time>
*   This app now runs on DOS systems!  Added support for compiling with OpenWatcom, to target 16-bit targets and retro machines, all the way back to MS-DOS and the 8086 (without APRS-IS support).  I've only tested it on FreeDOS 1.3 and an emulated 486 thus far.
*   Custom comments can be added to packets!  Use `--comment` to add your own notes to a packet.
*   When not using custom comments, the "X" in the comment field is removed.  APRS packets rarely, if ever, use the operating system identifier these days, so there is no need to prefix the user agent or comment with it.
*   Fixed a bug where [minute values less than ten would result in invalid uncompressed positions](https://github.com/rhymeswithmogul/aprs-weather-submit/issues/9).  Thank you, [ploeffler](https://github.com/ploeffler)!
*   Fixed a bug where the `--altitude` parameter was not being detected in its short form (`-A`).
*   Worked around a bug with [APRS.fi](https://APRS.fi), where [their site would reject packets with the temperature listed before the wind gust speeds](https://github.com/rhymeswithmogul/aprs-weather-submit/issues/6).  This was not a bug in this app.  Thank you, [Menno](https://github.com/pd9mwo).
*   [Replace calls to `strncat()` with calls to `strcat()`.](https://github.com/rhymeswithmogul/aprs-weather-submit/pull/4)  This eliminates on-by-default warnings in GCC 8.1., [KR4DIO](https://github.com/KR4DIO)!
*   Removed dependency on `<assert.h>`.

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
