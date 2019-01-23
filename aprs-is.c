/*
 aprs-weather-submit version 1.1
 Copyright (c) 2019 Colin Cogle
 
 This file, aprs-is.c, is part of aprs-weather-submit.
 
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

#include <stdio.h>		/* fprintf(), printf(), fputs() */
#include <stdlib.h>		/* malloc(), free(), exit() and constants */
#include <string.h>		/* str?cat() */
#include <sys/socket.h>	/* socket(), connect(), shutdown() */
#include <unistd.h>		/* read() */
#include <netinet/in.h>	/* sockaddr, sockaddr_in, sockaddr_in6 */
#include <arpa/inet.h>	/* inet_pton() */
#include <netdb.h>		/* getaddrinfo() */
#include "main.h"		/* PROGRAM_NAME, VERSION */
#include "aprs-is.h"

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
void sendPacket(const char* const server, const unsigned short port, const char* const username, const char* const password, const char* const toSend) {
	int					socket_desc = -1;
	int					error = 0;
	int					bytesRead = 0;
	char				authenticated = 0;
	char				foundValidServerIP = 0;
	struct addrinfo*	result = NULL;
	struct addrinfo*	results;
	char				verificationMessage[BUFSIZE];
	char*				buffer = malloc(BUFSIZE);
	
	error = getaddrinfo(server, NULL, NULL, &results);
	if (error != 0) {
		if (error == EAI_SYSTEM) {
			perror("getaddrinfo");
		} else {
			fprintf(stderr, "error in getaddrinfo: %s: %s\n", server, gai_strerror(error));
		}
		exit(EXIT_FAILURE);
	}
	
	for (result = results; result != NULL; result = result->ai_next) {
		/* For readability: */
		struct sockaddr* const addressinfo = result->ai_addr;
		
		socket_desc = socket(addressinfo->sa_family, SOCK_STREAM, IPPROTO_TCP);
		if (socket_desc < 0) {
			perror("error in socket()");
			continue; /* for loop */
		}
		
		/* Assign the port number. */
		switch (addressinfo->sa_family) {
			case AF_INET:
				((struct sockaddr_in*)addressinfo)->sin_port = htons(port);
				break;
			case AF_INET6:
				((struct sockaddr_in6*)addressinfo)->sin6_port = htons(port);
				break;
		}
		
		/* Connect */
#ifdef DEBUG
		switch (addressinfo->sa_family) {
			case AF_INET:
				inet_ntop(AF_INET, &((struct sockaddr_in*)addressinfo)->sin_addr, buffer, INET_ADDRSTRLEN);
				printf("Connecting to %s:%d...\n", buffer, ntohs(((struct sockaddr_in*)addressinfo)->sin_port));
				break;
			case AF_INET6:
				inet_ntop(AF_INET6, &((struct sockaddr_in6*)addressinfo)->sin6_addr, buffer, INET6_ADDRSTRLEN);
				printf("Connecting to [%s]:%d...\n", buffer, ntohs(((struct sockaddr_in6*)addressinfo)->sin6_port));
				break;
		}
#endif
		if (connect(socket_desc, addressinfo, result->ai_addrlen) >= 0) {
			foundValidServerIP = 1;
			break; /* for loop */
		}
		else {
			perror("error in connect()");
			shutdown(socket_desc, 2);
		}
	}
	freeaddrinfo(results);
	if (foundValidServerIP == 0) {
		fputs("Could not connect to the server.\n", stderr);
		exit(EXIT_FAILURE);
	}
	
	/* Authenticate */
	sprintf(buffer, "user %s pass %s vers %s/%s\n", username, password, PROGRAM_NAME, VERSION);
#ifdef DEBUG
	printf("> %s", buffer);
#endif
	send(socket_desc, buffer, strlen(buffer), 0);
	
	strncpy(verificationMessage, username, strlen(username)+1);
	strncat(verificationMessage, " verified", 9);
	bytesRead = read(socket_desc, buffer, BUFSIZE);
	while (bytesRead > 0) {
		buffer[bytesRead] = '\0';
#ifdef DEBUG
		printf("< %s", buffer);
#endif
		if (strstr(buffer, verificationMessage) != NULL) {
			authenticated = 1;
			break;
		} else {
			bytesRead = read(socket_desc, buffer, BUFSIZE);
		}
	}
	free(buffer);
	if (!authenticated) {
		fputs("Authentication failed!", stderr);
		exit(EXIT_FAILURE);
	}
	
	/* Send packet */
#ifdef DEBUG
	printf("> %s\n", toSend);
#endif
	send(socket_desc, toSend, strlen(toSend), 0);
	shutdown(socket_desc, 2);
	return;
}
