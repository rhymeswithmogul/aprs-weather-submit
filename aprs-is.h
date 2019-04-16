/*
 aprs-weather-submit version 1.2
 Copyright (c) 2019 Colin Cogle
 
 This file, aprs-is.h, is part of aprs-weather-submit.
 
 aprs-weather-submit is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 aprs-weather-submit is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with aprs-weather-submit. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef aprs_is_h
#define aprs_is_h

/**
 * sendPacket() -- sends a packet to an APRS-IS IGate server.
 *
 * @author			Colin Cogle
 * @param server	The (constant pointer to the constant) DNS hostname of the server.
 * @param port		The (constant) listening port on the server.
 * @param username	The (constant pointer to the constant) username with which to authenticate to the server.
 * @param password	The (constant pointer to the constant) password with which to authenticate to the server.
 * @param toSend	The (constant pointer to the constant) APRS-IS packet, as a string.
 * @since 0.3
 */
void sendPacket(const char* const server, const unsigned short port, const char* const username, const char* const password, const char* const toSend);

/* This should be defined by the operating system, but just in case... */
#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

/* This is not defined on Windows by default, it seems. */
#ifndef EAI_SYSTEM
#define EAI_SYSTEM -11
#endif


#endif /* aprs_is_h */
