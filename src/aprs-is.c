/*
 aprs-weather-submit
 Copyright (c) 2019-2025 Colin Cogle <colin@colincogle.name>

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

#ifdef HAVE_APRSIS_SUPPORT

#include <stdio.h>      /* fprintf(), printf(), fputs() */
#include <stdlib.h>     /* malloc(), free(), exit() and constants */
#include <string.h>     /* strcat() */
#include "main.h"       /* PACKAGE, VERSION */
#include "aprs-is.h"
#include <errno.h>	/* errno */

#ifndef _WIN32
#include <sys/types.h>  /* size_t */
#include <sys/socket.h> /* socket(), connect(), shutdown(), recv(), send() */
#include <netinet/in.h> /* sockaddr, sockaddr_in, sockaddr_in6 */
#include <netinet/tcp.h>/* TCP_USER_TIMEOUT */
#include <arpa/inet.h>  /* inet_pton() */
#include <netdb.h>      /* getaddrinfo(), gai_strerror() */
#include <unistd.h>     /* EAI_SYSTEM */
#else  /* _WIN32 */
#include <winsock2.h>   /* all that socket stuff on Windows */
#include <ws2tcpip.h>   /* inet_pton(), only available on Windows Vista and up */
#include <stdint.h>     /* intmax_t */
#endif /* _WIN32 */

/**
 * sendPacket() -- sends a packet to an APRS-IS IGate server.
 *
 * @author          Colin Cogle
 * @param server    The DNS hostname of the server.
 * @param port      The listening port on the server.
 * @param timeout   How long to wait before ending the connection.
 * @param username  The username with which to authenticate to the server.
 * @param password  The password with which to authenticate to the server.
 * @param toSend    The APRS-IS packet, as a string.
 * @param debugFlag Set to !0 to enable debugging output.
 * @since 0.3
 */
void
sendPacket (const char* const restrict server,
            const unsigned short port,
            const time_t timeout,
            const char* const restrict username,
            const char* const restrict password,
            const char* const restrict toSend,
            const int debugFlag
)
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
	signed int       socket_desc = -1;
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
#ifndef _WIN32
		if (socket_desc < 0)
#else
		if (socket_desc == INVALID_SOCKET)
#endif
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
		char* timeoutText = malloc(BUFSIZE);
		if (timeout > 0)
		{
			snprintf(timeoutText, BUFSIZE - 1, " (timeout %jd seconds)", (intmax_t)timeout);
		}
		else
		{
			snprintf(timeoutText, 14, " (no timeout)");
		}

		switch (addressinfo->sa_family)
		{
			case AF_INET:
				inet_ntop(
					AF_INET,
					&((struct sockaddr_in*)addressinfo)->sin_addr,
					buffer,
					INET_ADDRSTRLEN);
#ifndef NO_DEBUGGING
				if (debugFlag != 0)
				{
					printf("Connecting to %s:%d%s...\n",
					       buffer,
					       ntohs(((struct sockaddr_in*)addressinfo)->sin_port),
					       timeoutText
					);
				}
#endif
				break;

			case AF_INET6:
				inet_ntop(
					AF_INET6,
					&((struct sockaddr_in6*)addressinfo)->sin6_addr,
					buffer,
					INET6_ADDRSTRLEN);
#ifndef NO_DEBUGGING
				if (debugFlag != 0)
				{
					printf("Connecting to [%s]:%d%s...\n",
					       buffer,
					       ntohs(((struct sockaddr_in6*)addressinfo)->sin6_port),
					       timeoutText
					);
				}
#endif
				break;
		}
		free(timeoutText);

		/* Code to handle timeouts, as long as the user doesn't
		   want to wait forever (like v1.7.2 and older). */
		if (timeout > 0)
		{
#ifndef _WIN32
			struct timeval socket_timeout;
			socket_timeout.tv_sec  = timeout;
			socket_timeout.tv_usec = 0;
			setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &socket_timeout, sizeof(socket_timeout));
			setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &socket_timeout, sizeof(socket_timeout));
#else
			DWORD socket_timeout = timeout * 1000;
			setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, (const char*)&socket_timeout, sizeof(socket_timeout));
			setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (const char*)&socket_timeout, sizeof(socket_timeout));
#endif
		}

		if (connect(socket_desc, addressinfo, (size_t)(result->ai_addrlen)) >= 0)
		{
			foundValidServerIP = 1;
			break; /* for loop */
		}
		else
		{
			/* The error message for a timeout is misleading.
			   It says the connection is in progress, but we
			   are going to kill it, so this next statement
			   replaces that error with something clearer. */
			if (errno == EINPROGRESS)
			{
				errno = ETIMEDOUT;
			}
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
	sprintf(buffer, "user %s pass %s vers %s %s\n",
	        username, password, PACKAGE, VERSION);
#ifndef NO_DEBUGGING
	if (debugFlag != 0)
	{
		printf("> %s", buffer);
	}
#endif
	send(socket_desc, buffer, (size_t)strlen(buffer), 0);

	strcpy(verificationMessage, username);
	strcat(verificationMessage, " verified");
	bytesRead = recv(socket_desc, buffer, BUFSIZE, 0);
	while (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
#ifndef NO_DEBUGGING
		if (debugFlag != 0)
		{
			printf("< %s", buffer);
		}
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
		fputs("Authentication failed!\n", stderr);
		exit(EXIT_FAILURE);
	}

	/* Send packet */
#ifndef NO_DEBUGGING
	if (debugFlag != 0)
	{
		printf("> %s", toSend);
	}
#endif
	send(socket_desc, toSend, (size_t)strlen(toSend), 0);

	/* Done! */
	shutdown(socket_desc, 2);
	return;
}

#endif /* HAVE_APRSIS_SUPPORT */

/* The following line does nothing. It is present so that ISO-compliant
   compilers will not complain about this file being empty (which they
   will if we are not compiling with APRS-IS support. */
typedef int make_iso_compilers_happy;

