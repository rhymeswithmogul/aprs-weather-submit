# aprs-weather-submit news

This file details all of the user-facing changes that are in version 1.7 of aprs-weather-submit. For more details, please consult the CHANGELOG file or this project's GitHub page.

## CUSTOM ICONS

The new `--icon` parameter lets you specify a character from the two symbol tables.  If not specified, you will get the classic weather icon.

Note that APRS.fi does not support parsing weather data from packets that have custom icons.  If this is important to you, do not use this new feature.

## ARMHF FIXES

Version 1.7 did not compile on the armhf platforms.  Version 1.7.1 is fixed.
