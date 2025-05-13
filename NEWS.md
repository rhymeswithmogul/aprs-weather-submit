# aprs-weather-submit news

This file details all of the user-facing changes that are in versions 1.9 of aprs-weather-submit. For more details, please consult the CHANGELOG file or this project's GitHub page.

## We have our own TOCALL now!
`aprs-weather-submit` will identify itself using the `APWXS` TOCALL destination.  Most APRS hardware and software has its own TOCALL for statistical, tracking, and filtering purposes, and now I do as well.

If you are a developer forking this project, you are welcome to use your own TOCALL or pick from the reserved experimental range.  Directions for changing it are located in `main.h`.

## Device type identifier.
The `Z` device type identifier is now supported by using the `-Z`/`--device-type` parameter.  The device type is any two characters.

