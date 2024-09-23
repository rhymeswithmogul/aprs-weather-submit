# aprs-weather-submit news

This file details all of the user-facing changes that are in versions 1.7 of aprs-weather-submit. For more details, please consult the CHANGELOG file or this project's GitHub page.

## Custom Icons

The new `--icon` parameter lets you specify a character from the two symbol tables.  If not specified, you will get the classic weather icon.

Note that APRS.fi does not support parsing weather data from packets that have custom icons.  If this is important to you, do not use this new feature.

## `armhf` Fixes

Version 1.7 did not compile on the armhf platforms.  Version 1.7.1 is fixed.

## It's not the heat, it's the humidity!

Before version 1.7.2, one could not create an APRS packet with 100% relative humidity due to a bug in the code that only allowed values from 1% to 99%.  This was corrected in version 1.7.2 thanks to a report from Jeff, N9CQS.

## Can this app be in Debian, please?

I've also been making behind-the-scenes improvements to how this app is packaged and installed.  Users of Debian-based Linux distributions can enjoy nearly-effortless installation, upgrades, and removals.
