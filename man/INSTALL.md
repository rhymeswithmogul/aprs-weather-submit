# Building and Installing `aprs-weather-submit`
To use this app, you will need a standard C library.  Almost any Linux or macOS system will have this.

While this app is designed to run on any system, I've been able to test it on the following architectures:
* 32-bit Intel ("x86" or "i486")
* 64-bit Intel ("x86_64" or "amd64")
* 32-bit PowerPC ("ppc")
* 64-bit PowerPC ("ppc64" or "ppc64el")
* IBM i/System 390 ("s390x")

APRS-IS features require a TCP/IPv4 or TCP/IPv6 network.

## Build Dependencies
To build this app, you will need Autotools, GCC, and GNU make.  On Debian-based systems, you can install them with this:

```console
sudo apt install autoconf build-essential make
```

## Build Script
To build and install `aprs-weather-submit`, do this:

```console
./autogen.sh
./configure [--disable-aprs-is] [--enable-debug]
make
sudo make install
```
