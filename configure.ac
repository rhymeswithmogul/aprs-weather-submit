dnl  aprs-weather-submit
dnl  Copyright (c) 2019-2024 Colin Cogle <colin@colincogle.name>
dnl
dnl  This file, configure.ac, is part of aprs-weather-submit.
dnl  <https://github.com/rhymeswithmogul/aprs-weather-submit>
dnl
dnl  This program is free software: you can redistribute it and/or modify it
dnl  under the terms of the GNU Affero General Public License as published by
dnl  the Free Software Foundation, either version 3 of the License, or (at your
dnl  option) any later version.
dnl
dnl  This program is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public
dnl  License for more details.
dnl
dnl  You should have received a copy of the GNU Affero General Public License
dnl  along with this program. If not, see <http://gnu.org/licenses/agpl-3.0.html>.

AC_INIT([aprs-weather-submit], [1.8], [https://github.com/rhymeswithmogul/aprs-weather-submit/])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
AC_PREREQ
AC_PROG_INSTALL

dnl  Check for a C99 compiler.
AC_LANG([C])
AC_PROG_CC
AC_PROG_CC_C_O
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT

dnl  Check for install.
AC_PROG_INSTALL

dnl  Define types
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T

dnl  C compiler flags:
dnl    -O2              Optimize (unless we're debugging)
dnl    -std=gnu99       Compile C99, with GNU extensions (for getaddrinfo,
dnl                     gai_strerror, freeaddrinfo, struct addrinfo)
dnl    -Wall            Complain heavily.
dnl    -Wextra          Complain very heavily.
dnl    -Werror          Make all warnings errors.
dnl    -pedantic-errors Make sure we're writing compliant C code.
AM_CFLAGS="-O2 -std=gnu99 -Wall -Wextra -Werror -pedantic-errors"
AM_LDFLAGS="-lm"
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])


dnl  Check for the standard I/O functions, plus C99's snprintf().
AC_CHECK_HEADER(stdio.h,  [], AC_MSG_ERROR([stdio.h is required to build this package.]))
AC_CHECK_FUNC([fprintf],  [], AC_MSG_ERROR([fprintf() is required to build this package but was not found in stdio.h.]))
AC_CHECK_FUNC([fputs],    [], AC_MSG_ERROR([fputs() is required to build this package but was not found in stdio.h.]))
AC_CHECK_FUNC([printf],   [], AC_MSG_ERROR([printf() is required to build this package but was not found in stdio.h.]))
AC_CHECK_FUNC([puts],     [], AC_MSG_ERROR([puts() is required to build this package but was not found in stdio.h.]))
AC_CHECK_FUNC([snprintf], [], AC_MSG_ERROR([snprintf() is required to build this package but was not found in stdio.h.]))

dnl  Check for string functions.
AC_CHECK_HEADER(string.h, [], AC_MSG_ERROR([string.h is required to build this package.]))
AC_CHECK_FUNC([strcpy],   [], AC_MSG_ERROR([strcpy() is required to build this package but was not found in string.h.]))
AC_CHECK_FUNC([strlen],   [], AC_MSG_ERROR([strlen() is required to build this package but was not found in string.h.]))
AC_CHECK_FUNC([strncmp],  [], AC_MSG_ERROR([strncmp() is required to build this package but was not found in string.h.]))
AC_CHECK_FUNC([strstr],   [], AC_MSG_ERROR([strstr() is required to build this package but was not found in string.h.]))

dnl  Check for malloc() support.
AC_FUNC_MALLOC

dnl  Check for getopt.
AC_CHECK_HEADER(getopt.h,    [], AC_MSG_ERROR([getopt.h is required to build this package.]))
AC_CHECK_FUNC([getopt_long], [], AC_MSG_ERROR([getopt_long() is required to build this package but was not found in getopt.h.]))

dnl  Check for Windows support.
AC_ARG_ENABLE([windows],
    [AS_HELP_STRING([--enable-windows], [build for Windows (Vista and newer)])],
    [enable_windows="yes"],
    [enable_windows="no"]
)
AS_IF([test "x$enable_windows" != "xno"], [
    AC_MSG_RESULT([checking for --enable-windows flag... yes])
    AC_DEFINE([_WIN32], 1, [Build for Microsoft Windows])
])

dnl  Check for our math functions.
AC_CHECK_HEADERS(math.h, [], AC_MSG_ERROR([math.h is required to build this package.]))
dnl For some reason, the following checks are failing.
dnl AC_CHECK_FUNC([fabs],    [], AC_MSG_ERROR([fabs() is required to build this package but was not found in math.h.]))
dnl AC_CHECK_FUNC([floor],   [], AC_MSG_ERROR([floor() is required to build this package but was not found in math.h.]))
dnl AC_CHECK_FUNC([log],     [], AC_MSG_ERROR([log() is required to build this package but was not found in math.h.]))
dnl AC_CHECK_FUNC([logf],    [], AC_MSG_ERROR([logf() is required to build this package but was not found in math.h.]))
dnl AC_CHECK_FUNC([pow],     [], AC_MSG_ERROR([pow() is required to build this package but was not found in math.h.]))
dnl AC_CHECK_FUNC([round],   [], AC_MSG_ERROR([round() is required to build this package but was not found in math.h.]))
AM_LDFLAGS="-lm"
LDADD=-lm

dnl  Check for stdlib.h
AC_CHECK_HEADER(stdlib.h, [], AC_MSG_ERROR([stdlib.h is required to build this package.]))
AC_CHECK_FUNC([atof],     [], AC_MSG_ERROR([atof() is required to build this package but was not found in stdlib.h.]))
AC_CHECK_FUNC([atoi],     [], AC_MSG_ERROR([atoi() is required to build this package but was not found in stdlib.h.]))
AC_CHECK_FUNC([exit],     [], AC_MSG_ERROR([exit() is required to build this package but was not found in stdlib.h.]))

dnl  Check for time.h
AC_CHECK_HEADER(time.h, [], AC_MSG_ERROR([time.h is required to build this package.]))
AC_CHECK_FUNC([gmtime], [], AC_MSG_ERROR([gmtime() is required to build this package but was not found in time.h.]))
AC_CHECK_FUNC([time],   [], AC_MSG_ERROR([time() is required to build this package but was not found in time.h.]))

dnl  Check to see if we can compile this with APRS-IS support, unless the user
dnl  asked us not to.  If --disable-aprs-is was *not* specified and we cannot
dnl  compile with APRS-IS support for whatever reason, fail.
AC_ARG_ENABLE([aprs-is], 
    [AS_HELP_STRING([--disable-aprs-is], [disable APRS-IS support])],
    [enable_aprs_is="no"],
    [enable_aprs_is="yes"]
)
AS_IF([test "x$enable_aprs_is" != "xno"], [
dnl need: [gai_strerror getaddrinfo inet_ntop ntohs perror recv send shutdown socket]
    AC_CHECK_HEADERS(
        [errno.h],
        [],
        AC_MSG_ERROR([Cannot compile with APRS-IS support because <errno.h> is missing.])
    )
    AC_CHECK_HEADERS(
        [sys/types.h],
        [],
        AC_MSG_ERROR([Cannot compile with APRS-IS support because <sys/types.h> is missing.])
    )
    AC_CHECK_HEADERS(
        [unistd.h],
        [],
        AC_MSG_ERROR([Cannot compile with APRS-IS support because <unistd.h> is missing.])
    )
    AC_CHECK_FUNC(
        [atol],
        [],
        AC_MSG_ERROR([atol() is required to build this package but was not found in stdlib.h.])
    )
    AS_IF([test "x$enable_windows" != "xyes"], [
        AC_CHECK_HEADERS(
            [arpa/inet.h],
            [],
            AC_MSG_ERROR([Cannot compile with APRS-IS support because <arpa/inet.h> is missing.])
        )
        AC_CHECK_HEADERS(
            [netdb.h],
            [],
            AC_MSG_ERROR([Cannot compile with APRS-IS support because <netdb.h> is missing.])
        )
        AC_CHECK_HEADERS(
            [netinet/in.h],
            [],
            AC_MSG_ERROR([Cannot compile with APRS-IS support because <netinet/in.h> is missing.])
        )
        AC_CHECK_HEADERS(
            [sys/socket.h],
            [],
            AC_MSG_ERROR([Cannot compile with APRS-IS support because <sys/socket.h> is missing.])
        )
    ], [
        AC_CHECK_HEADERS(
            [winsock2.h],
            [],
            AC_MSG_ERROR([Cannot compile for Windows with APRS-IS support because <winsock2.h> is missing.])
        )
        AC_CHECK_HEADERS(
            [ws2tcpip.h],
            [],
            AC_MSG_ERROR([Cannot compile for Windows with APRS-IS support because <wstcpip2.h> is missing.])
        )
        AM_LDFLAGS="-lm -lws2_32"
        LDADD="-lm -lws2_32"
    ])
    AC_DEFINE([HAVE_APRSIS_SUPPORT], [1], [Enable APRS-IS])
])

AC_ARG_ENABLE([debug], 
    [AS_HELP_STRING([--enable-debug], [build with added debugging output])],
    [enable_debug="yes"],
    [enable_debug="no"]
)
AS_IF([test "x$enable_debug" = "xyes"], [
    AC_DEFINE([DEBUG], 1, [Build with symbols and debugging output.])
    CFLAGS="$CFLAGS -O0 -g3"
])

dnl  Done!
AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
AC_MSG_RESULT([
############################
Run make to build this app.

OPTIONS:
  DOS support:      no (use MAKE.BAT instead)
  Windows support:  $enable_windows
  APRS-IS support:  $enable_aprs_is
  Debugging output: $enable_debug
###########################
])
