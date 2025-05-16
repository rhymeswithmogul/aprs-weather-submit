# Installing `aprs-weather-submit`

## Via My <abbr title="Personal Package Archive">PPA</abbr>
I've created my very own PPA.  Ubuntu users (and most Debian users, I believe) can add my repository and install a binary package:

```console
sudo add-apt-repository ppa:signofzeta/aprs-weather-submit
sudo apt update
sudo apt install aprs-weather-submit
```

This is the recommended method, as it will automatically update along with the rest of your software.

## Compiling and Installing from Source
To use this app, you will need a standard C library.  Almost any Linux or macOS system will have this.

While this app is designed to run on any system, I've been able to test it on the following architectures:
*   32-bit Intel (`x86`, `i386`, `i486`, or `i686`)
*   64-bit Intel (`x86_64` or `amd64`)
*   32-bit PowerPC (`ppc`)
*   64-bit PowerPC (`ppc64` and `ppc64el`)
*   ARM/AArch64 (`armhf`, `arm64`, and Apple's `arm64e`)
*   IBM i/System 390 (`s390x`)
*   RISC-V (`riscv64`)

APRS-IS features require a TCP/IP network.  IPv4 and IPv6 are both supported.

### Build Dependencies
To build this app, you will need Autotools, GCC, and GNU make.  On Debian-based systems, you can install them with this:

```console
sudo apt install autoconf build-essential make
```

### Build Script
To build and install `aprs-weather-submit`, do this:

```console
./autogen.sh
./configure [--help to see all the options]
make
sudo make install
```
