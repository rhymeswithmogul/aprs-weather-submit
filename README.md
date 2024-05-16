# aprs-weather-submit

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a5e5337dd57b486089391aabd2f5429b)](https://app.codacy.com/gh/rhymeswithmogul/aprs-weather-submit?utm_source=github.com\&utm_medium=referral\&utm_content=rhymeswithmogul/aprs-weather-submit\&utm_campaign=Badge_Grade_Settings)

Not everyone has a fancy weather station with APRS connectivity built in.  Maybe you're like me, and have an old-school thermometer and CoCoRaHS-approved rain gauge.  This command-line app, written in C99, can compile on most Linux toolchains (Windows support is in the works) and will manually submit APRS 1.2.1-compliant weather information to the APRS-IS network.

## Help

Anyone can use this app to create [an APRS packet](http://www.aprs.org/doc/APRS101.PDF).  However, to send it to the APRS-IS network, you must have an account on an APRS-IS IGate server, as well as an amateur radio license or CWOP identifier (more on that below).

## Examples

At the bare minimum, you can submit your weather station's position with a command line like this:

```console
$ ./aprs-weather-submit --callsign W1AW-13 --latitude 41.714692 --longitude -72.728514 --altitude 240 --server example-igate-server.foo --port 12345 --username hiram --password percymaxim
```

If you'd like to report a temperature of 68°F, you can use a command like this:

```console
$ ./aprs-weather-submit -k W1AW-13 -n 41.714692 -e -72.728514 -I example-igate-server.foo -o 12345 -u hiram -d percymaxim -t 68
```

Or, if you just want the raw packet for your own use, don't specify server information:

```console
$ ./aprs-weather-submit -k W1AW-13 -n 41.714692 -e -72.728514 -t 68
W1AW-13>APRS,TCPIP*:@090247z4142.88N/07243.71W_.../...t068aprs-weather-submit/1.5.2
```

This app supports all of the weather data parameters defined in APRS versions up to and including version 1.2.1:

*   Altitude (`-A`, `--altitude`)
*   Barometric pressure (`-b`, `--pressure`) in mbar/hPa
*   Luminosity (`-L`, `--luminosity`)
*   Radiation (`-X`, `--radiation`)
*   Rainfall in the past 24 hours (`-p`, `--rainfall-last-24-hours`) in inches
*   Rainfall since midnight (`-P`, `--rainfall-since-midnight`) in inches
*   Rainfall in the past hour (`-r`, `--rainfall-last-hour`) in inches
*   Relative humidity (`-h`,`  --humidity `)
*   Snowfall in the past 24 hours (`-s`, `--snowfall-last-24-hours`)
*   Temperature (°F) (`-t`, `--temperature`)
*   Temperature (°C) (`-T`, `--temperature-celsius`)
*   Water level above flood stage or mean tide (`-F`, `--water-level-above-stage`)
*   Weather station battery voltage (`-V`, `--voltage`)
*   Wind direction (`-c`, `--wind-direction`)
*   Wind speed, peak in the last five minutes (`-g`, `--gust`) in mph
*   Wind speed, sustained over the last minute (`-S`, `--wind-speed`) in mph

## Installing

### Ubuntu Linux and Debian-based distributions
If your distribution supports <abbr title="Personal Package Archive">PPA</abbr>s, [I just learned how to make a PPA!](https://launchpad.net/~signofzeta/+archive/ubuntu/aprs-weather-submit)  You can install this with APT:

```bash
sudo add-apt-repository ppa:signofzeta/aprs-weather-submit
sudo apt update
sudo apt install aprs-weather-submit
```

If not, follow the instructions in `INSTALL.md` to configure it normally:
```bash
./autogen.sh
./configure
make
```

## Legal Notices

To use this app, you *must* be either:

1.  a licensed amateur radio operator, or
2.  a member of the [Citizen Weather Observer Program](http://wxqa.com/) in good standing.

[Getting your ham radio license is easy](https://hamstudy.org/), and [joining CWOP is even easier](http://wxqa.com/SIGN-UP.html).

Like it says in the license:  this app is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the [GNU Affero General Public License 3.0](https://www.gnu.org/licenses/agpl-3.0.html) for more details.  As such, you and you alone are solely responsible for using this app to submit complete and correct weather and/or location data.  Please do not use this app for evil.  Don't make me regret writing this app.

QTH. 73, W1DNS (formerly KC1HBK)
