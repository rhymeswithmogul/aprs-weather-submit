/*
 aprs-weather-submit
 Copyright (c) 2019-2025 Colin Cogle <colin@colincogle.name>
 
 This file, aprs-is.h, is part of aprs-weather-submit.
 <https://github.com/rhymeswithmogul/aprs-weather-submit>

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along
with this program.  If not, see <https://www.gnu.org/licenses/agpl-3.0.html>.
 */

#ifdef HAVE_APRSIS_SUPPORT

#ifndef aprs_is_h
#define aprs_is_h

/* We need to include a definition for socklen_t. */
#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#endif

/**
 * sendPacket() -- sends a packet to an APRS-IS IGate server.
 *
 * @author          Colin Cogle
 * @param server    The DNS hostname of the server.
 * @param port      The listening port on the server.
 * @param timeout   How long to wait before timing out.
 * @param username  The username with which to authenticate to the server.
 * @param password  The password with which to authenticate to the server.
 * @param toSend    The APRS-IS packet, as a string.
 * @param debugFlag Set to !0 to enable debugging output.
 * @since 0.3
 */
void
sendPacket (const char* const restrict server,
            const socklen_t port,
            const time_t timeout,
            const char* const restrict username,
            const char* const restrict password,
            const char* const restrict toSend,
            const char debugFlag);

/* How long to wait before timing out when the user doesn't specify.
   Fifteen seconds seems reasonable. */
#ifndef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 15
#endif

/* This should be defined by the operating system, but just in case... */
#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

/* This is not defined on Windows by default, it seems. */
#ifndef EAI_SYSTEM
#define EAI_SYSTEM -11
#endif

/* When compiling for Windows, we need to target Vista and later
   because WinSock2 contains the inet_ntop() function. */
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #undef  _WIN32_WINNT
  #define _WIN32_WINNT 0x0600
#endif /* _WIN32 */

#endif /* aprs_is_h */

#endif /* HAVE_APRSIS_SUPPORT */
