#!/bin/sh
cd /usr/local/src
cp -r /keybase/private/colincogle/aprs-weather-submit aprs-weather-submit-$1

# Create TAR
cd aprs-weather-submit
rm -rf .git .gitignore src/.dirstamp src/.deps
cd ..
tar cf aprs-weather-submit_$1.orig.tar --exclude=debian aprs-weather-submit-$1
xz -9e aprs-weather-submit_$1.orig.tar

# Build package
cd aprs-weather-submit-$1
dh_make -s -c gpl3 -y
dpkg-buildpackage -rfakeroot -kB9D51810CEFEEDFC
dput ppa:signofzeta/aprs-weather-submit *.changes
