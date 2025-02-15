# aprs-weather-submit news

This file details all of the user-facing changes that are in versions 1.9 of aprs-weather-submit. For more details, please consult the CHANGELOG file or this project's GitHub page.

## Debugging output is now a runtime option.
There is a new command-line option, `--debug`, that will enable debugging output.  In previous versions, this was a compile-time option.  Thanks to DL9SEC for the idea!

As a result, I've removed the `--enable-debug` compile-time option that set the `DEBUG` preprocessor macro.

However, if you *really* need the smallest-possible binary for whatever reason, there is a `--disable-debugging` compile-time option that will strip all debugging code from the executable.

## Device type identifier.
The `Z` device type identifier is now supported by using the `-Z`/`--device-type` parameter.  The device type is any two characters.

