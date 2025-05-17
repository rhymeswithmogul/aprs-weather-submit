# aprs-weather-submit news

This file details all of the user-facing changes that are in versions 2.0 of aprs-weather-submit. For more details, please consult the CHANGELOG file or this project's GitHub page.


## We have our own TOCALL now!

`aprs-weather-submit` will identify itself using the `APWXS` TOCALL destination.  Most APRS hardware and software has its own TOCALL for statistical, tracking, and filtering purposes, and now I do as well.  (I thought this was big enough news to warrant bumping the major version number.)

If you are a developer forking this project, you are welcome to use your own TOCALL or pick from the reserved experimental range.  Directions for changing it are located in `main.h`.


## Debugging output is now a runtime option.

There is a new command-line option, `--debug`, that will enable debugging output.  In previous versions, this was a compile-time option.  Thanks to DL9SEC for the idea!

As a result, I've removed the `--enable-debug` compile-time option that set the `DEBUG` preprocessor macro.

However, if you *really* need the smallest-possible binary for whatever reason, there is a `--disable-debugging` compile-time option that will strip all debugging code from the executable.

## This app supports changing the icon, but you probably shouldn't.

Upon careful reading of the APRS specification, SQ9RHX and I found out that you're not allowed to change the APRS icon if you're including any weather data *except for* wind speed and direction.  Now, the app's documentation will suggest you _not_ use the `--icon` switch, as it will cause other weather data (such as temperature) to be interpreted as a comment.


## Device type identifier.

The `Z` device type identifier is now supported by using the `-Z`/`--device-type` parameter.  The device type is any two characters.  Note that not all APRS-IS sites can parse this information, and may display it as a comment.


## Tested on Apple Silicon

This code is fully compatible with Apple Silicon's `arm64` and `arm64e` architectures.  Translation: it just works!
