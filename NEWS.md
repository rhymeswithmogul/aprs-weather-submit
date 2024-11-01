# aprs-weather-submit news

This file details all of the user-facing changes that are in versions 1.8 of aprs-weather-submit. For more details, please consult the CHANGELOG file or this project's GitHub page.

## Graceful failure

In version 1.7.2 and older, the app would hang almost indefinitely if the remote APRS-IS server did not respond.  This version fixes that by converting to non-blocking sockets, so that if your APRS-IS server does not respond in a timely manner (for whatever reason), the connection will fail gracefully.

Likewise, there is a new parameter, `--timeout`, that will let you specify a timeout value, if you don't like my default of fifteen seconds.

If you want the old behavior, specify `--timeout 0`.

Thanks to [DL9SEC](https://www.dl9sec.de) for pointing out this behavior, and I suppose thanks are due to the APRS Tier 2 network for having some servers down long enough for this bug to get pointed out.


## Building without APRS-IS support

I'd always given users the option to build a custom version without APRS-IS support by specifying `./configure --disable-aprs-is`.  However, that broke at some point.  This option now works as intended.

