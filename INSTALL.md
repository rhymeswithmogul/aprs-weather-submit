# Building and Installing `aprs-weather-submit`
To build and install `aprs-weather-submit`, do this:

```console
./autogen.sh
./configure [--disable-aprs-is] [--enable-debug]
make
sudo make install
```
## Build Dependencies
To build this app, you will need Autotools, GCC, and GNU make.  On Debian-based systems, you can install them with this:

```console
sudo apt install autoconf build-essential make
```