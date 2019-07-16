## Version 1.2.1
In progress
* Added &lt;stdint.h&gt; to prevent compiler errors.  (Thank you to UR3QJW for reporting this.)
* Switched from ANSI C to the C99 standard, in order to keep using <tt>snprintf()</tt>.  Some compilers are stricter than mine and refuse to compile K&R C with it.  (Another thank you to UR3QJW.)
* Fixed a pair of bugs where uncompressed packets would fail to encode the location correctly.
* Code cleanup.

## Version 1.2
<time datetime="2019-04-16T03:10:07-04:00">April 16, 2019</time>
* Added preliminary support for compiling on Windows.
* Added the option of using APRS uncompressed positions at runtime with the  <tt>--uncompressed-position</tt> switch, rather than a compile-time choice.
* Removed a dependency on <code>stdint.h</code>.
* Fixed a bug where impractically large APRS-IS packets would be truncated.  This was unlikely to affect real-world use.

## Version 1.1
<time datetime="2019-01-23T11:18:00-05:00">January 23, 2019</time> -- codename QLF
* Added support for reporting temperatures in degrees Celsius.
* Added a Makefile for POSIX systems.
* Added a man page.
* Fixed a bug where negative temperatures may cause the packet to fail to encode.
* Cleaned up some documentation.

## Version 1.0
<time datetime="2019-01-21T00:55:00-05:00">January 21, 2019</time> -- codename QRV
* Added significant documentation.

## Version 0.3
<time datetime="2019-01-20T23:04:02-05:00">January 20, 2019</time> -- codename QST
* Wrote help.
* Added support for actually sending the packet to an APRS-IS IGate.
* Changed the position and wind data to compressed from uncompressed.  For now, this can only be undone at compile-time.

## Version 0.2
<time datetime="2019-01-18">January 18, 2019</time> -- codename "Strings are hard"
* Added support for snowfall, as clarified in APRS 1.1's errata.
* Added support for the APRS 1.2 parameters: radiation, water level, and voltage.

## Version 0.1
<time datetime="2019-01-14T22:15:00-05:00">January 14, 2019</time> -- codename Q
* Initial private release.
