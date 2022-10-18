# aprs-weather-submit news

This file details all of the user-facing changes that are in versions 1.6 of
aprs-weather-submit. For more details, please consult the CHANGELOG file or
this project's GitHub page.

## DOS SUPPORT?

I have preliminary support for compiling this app on FreeDOS, MS-DOS, and other DOS-compatible systems.  Use the `make-dos.bat` file to build the app and create a file, `APRSWX.EXE`.  Note that APRS-IS support will be unavailable.

This does not currently work.  OpenWatcom has an implementation of `getopt()` that is required, but due to licensing differences, I cannot distribute it with this app.  I'm working on finding a replacemnet.

## CUSTOM COMMENTS

Custom comments can now be included with the `--comment` option.

## GOT ME A RAYGUN, GOT ME AN ALTITUDE

The short version of the `--altitude` parameter (`-A`) was not being detected.  This bug has been corrected.

-----
For information about older versions, consult the CHANGELOG.md files.
