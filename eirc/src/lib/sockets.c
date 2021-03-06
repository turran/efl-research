/* 
 * Copyright (C) 2004 Georgy Yunaev tim@krasnogorsk.ru
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public 
 * License for more details.
 *
 * $Id: sockets.c 53 2004-10-17 11:16:03Z gyunaev $
 */

#include "libircclient.h"

int socket_error()
{
#if !defined (WIN32)
	return errno;
#else
	return WSAGetLastError();
#endif
}


int socket_create (int domain, int type, socket_t * sock)
{
	*sock = socket (PF_INET, SOCK_STREAM, 0);
	return IS_SOCKET_ERROR(*sock) ? 1 : 0;
}


int socket_make_nonblocking (socket_t * sock)
{
#if !defined (WIN32)
	return fcntl (*sock, F_SETFL, fcntl (*sock, F_GETFL,0 ) | O_NONBLOCK) != 0;
#else
	unsigned long mode = 0;
	return ioctlsocket (*sock, FIONBIO, &mode) == SOCKET_ERROR;
#endif
}


int socket_close (socket_t * sock)
{
#if !defined (WIN32)
	close (*sock);
#else
	closesocket (*sock);
#endif

	*sock = -1;
	return 0;
}


int socket_connect (socket_t * sock, const struct sockaddr *saddr, socklen_t len)
{
	while ( 1 )
	{
	    if ( connect (*sock, saddr, len) < 0 )
	    {
	    	if ( socket_error() == EINTR )
	    		continue;

			if ( socket_error() != EINPROGRESS && socket_error() != EWOULDBLOCK )
				return 1;
		}

		return 0;
	}
}


int socket_accept (socket_t * sock, socket_t * newsock, struct sockaddr *saddr, socklen_t * len)
{
	while ( IS_SOCKET_ERROR(*newsock = accept (*sock, saddr, len)) )
	{
    	if ( socket_error() == EINTR )
    		continue;

		return 1;
	}

	return 0;
}


int socket_recv (socket_t * sock, void * buf, size_t len)
{
	int length;

	while ( (length = recv (*sock, buf, len, 0)) < 0
	&& socket_error() == EINTR )
		continue;

	return length;
}


int socket_send (socket_t * sock, const void *buf, size_t len)
{
	int length;

	while ( (length = send (*sock, buf, len, 0)) < 0
	&& socket_error() == EINTR )
		continue;

	return length;
}
