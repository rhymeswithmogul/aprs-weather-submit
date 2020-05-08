/*
 aprs-weather-submit version 1.4
 Copyright (c) 2019-2020 Colin Cogle <colin@colincogle.name>
 
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

#ifndef NO_APRSIS

#ifndef aprs_is_h
#define aprs_is_h

/**
 * sendPacket() -- sends a packet to an APRS-IS IGate server.
 *
 * @author         Colin Cogle
 * @param server   The DNS hostname of the server.
 * @param port     The listening port on the server.
 * @param username The username with which to authenticate to the server.
 * @param password The password with which to authenticate to the server.
 * @param toSend   The APRS-IS packet, as a string.
 * @since 0.3
 */
void
sendPacket (const char* const restrict server, const unsigned short port,
            const char* const restrict username,
            const char* const restrict password,
            const char* const restrict toSend);

/* This should be defined by the operating system, but just in case... */
#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

/* This is not defined on Windows by default, it seems. */
#ifndef EAI_SYSTEM
#define EAI_SYSTEM -11
#endif


#endif /* aprs_is_h */

#endif // NO_APRSIS
