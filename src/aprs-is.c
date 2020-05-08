/*
 aprs-weather-submit version 1.4
 Copyright (c) 2019-2020 Colin Cogle <colin@colincogle.name>

 This file, aprs-is.c, is part of aprs-weather-submit.
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

#include <stdio.h>      /* fprintf(), printf(), fputs() */
#include <stdlib.h>     /* malloc(), free(), exit() and constants */
#include <string.h>     /* str?cat() */
#include "main.h"       /* PROGRAM_NAME, VERSION */
#include "aprs-is.h"

#ifndef _WIN32
#include <sys/types.h>  /* size_t */
#include <sys/socket.h> /* socket(), connect(), shutdown(), recv(), send() */
#include <netinet/in.h> /* sockaddr, sockaddr_in, sockaddr_in6 */
#include <arpa/inet.h>  /* inet_pton() */
#include <netdb.h>      /* getaddrinfo(), gai_strerror() */
#include <unistd.h>     /* EAI_SYSTEM */
#else  /* _WIN32 */
#include <WinSock2.h>   /* all that socket stuff on Windows */
#include <WS2tcpip.h>   /* inet_pton(), only available on Windows Vista and up */
#endif /* _WIN32 */

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
            const char* const restrict toSend)
{
	int              error = 0;
	int              bytesRead = 0;
	char             authenticated = 0;
	char             foundValidServerIP = 0;
	struct addrinfo* result = NULL;
	struct addrinfo* results;
	char             verificationMessage[BUFSIZE];
	char*            buffer = malloc(BUFSIZE);
#ifndef _WIN32
	int              socket_desc = -1;
#else
	SOCKET           socket_desc = INVALID_SOCKET;
	int              wsaResult;
	WSADATA          wsaData;

	wsaResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (wsaResult != 0)
	{
		fprintf(stderr,
		        "WinSock2: WSAStartup failed with error: %d\n", wsaResult);
		exit(EXIT_FAILURE);
	}
#endif

	error = getaddrinfo(server, NULL, NULL, &results);
	if (error != 0)
	{
		if (error == EAI_SYSTEM)
		{
			perror("getaddrinfo");
		}
		else
		{
			fprintf(stderr, "error in getaddrinfo: %s: %s\n", server,
			        gai_strerror(error));
		}
		exit(EXIT_FAILURE);
	}

	for (result = results; result != NULL; result = result->ai_next)
	{
		/* For readability later: */
		struct sockaddr* const addressinfo = result->ai_addr;

		socket_desc = socket(addressinfo->sa_family, SOCK_STREAM, IPPROTO_TCP);
		if (socket_desc < 0)
		{
			perror("error in socket()");
			continue; /* for loop */
		}

		/* Assign the port number. */
		switch (addressinfo->sa_family)
		{
			case AF_INET:
				((struct sockaddr_in*)addressinfo)->sin_port   = htons(port);
				break;
			case AF_INET6:
				((struct sockaddr_in6*)addressinfo)->sin6_port = htons(port);
				break;
		}

		/* Connect */
		switch (addressinfo->sa_family)
		{
			case AF_INET:
				inet_ntop(
					AF_INET,
					&((struct sockaddr_in*)addressinfo)->sin_addr,
					buffer,
					INET_ADDRSTRLEN);
#ifdef DEBUG
				printf("Connecting to %s:%d...\n", buffer,
				       ntohs(((struct sockaddr_in*)addressinfo)->sin_port));
#endif
				break;
			case AF_INET6:
				inet_ntop(
					AF_INET6,
					&((struct sockaddr_in6*)addressinfo)->sin6_addr,
					buffer,
					INET6_ADDRSTRLEN);
#ifdef DEBUG
				printf("Connecting to [%s]:%d...\n", buffer,
				       ntohs(((struct sockaddr_in6*)addressinfo)->sin6_port));
#endif
				break;
		}

		if (connect(socket_desc, addressinfo, (size_t)(result->ai_addrlen)) >= 0)
		{
			foundValidServerIP = 1;
			break; /* for loop */
		}
		else
		{
			perror("error in connect()");
			shutdown(socket_desc, 2);
		}
	}
	freeaddrinfo(results);
	if (foundValidServerIP == 0)
	{
		fputs("Could not connect to the server.\n", stderr);
		exit(EXIT_FAILURE);
	}

	/* Authenticate */
	sprintf(buffer, "user %s pass %s vers %s/%s\n",
	        username, password, PROGRAM_NAME, VERSION);
#ifdef DEBUG
	printf("> %s", buffer);
#endif
	send(socket_desc, buffer, (size_t)strlen(buffer), 0);

	strncpy(verificationMessage, username, (size_t)strlen(username)+1);
	strncat(verificationMessage, " verified", 9);
	bytesRead = recv(socket_desc, buffer, BUFSIZE, 0);
	while (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
#ifdef DEBUG
		printf("< %s", buffer);
#endif
		if (strstr(buffer, verificationMessage) != NULL)
		{
			authenticated = 1;
			break;
		}
		else
		{
			bytesRead = recv(socket_desc, buffer, BUFSIZE, 0);
		}
	}
	free(buffer);
	if (!authenticated)
	{
		fputs("Authentication failed!", stderr);
		exit(EXIT_FAILURE);
	}

	/* Send packet */
#ifdef DEBUG
	printf("> %s", toSend);
#endif
	send(socket_desc, toSend, (size_t)strlen(toSend), 0);
	
	/* Done! */
	shutdown(socket_desc, 2);
	return;
}
