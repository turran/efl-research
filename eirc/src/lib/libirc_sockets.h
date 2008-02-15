#ifndef _LIBIRC_SOCKET_H
#define _LIBIRC_SOCKET_H

/*
 * The sockets interface was moved out to simplify going OpenSSL integration.
 */
#if !defined (WIN32)
	#include <sys/socket.h>
	#include <netdb.h>
	#include <arpa/inet.h>	
	#include <netinet/in.h>
	#include <fcntl.h>

	#define IS_SOCKET_ERROR(a)	((a)<0)
	typedef int				socket_t;

#else
	#include <windows.h>
	#include <winsock.h>

	#define IS_SOCKET_ERROR(a)	((a)==SOCKET_ERROR)

	#define EWOULDBLOCK		WSAEWOULDBLOCK
	#define EINPROGRESS		WSAEINPROGRESS
	#define EINTR			WSAEINTR

	typedef unsigned int	socklen_t;
	typedef SOCKET			socket_t;

#endif

#ifndef INADDR_NONE
	#define INADDR_NONE 	0xFFFFFFFF
#endif

int socket_error();
int socket_create (int domain, int type, socket_t * sock);
int socket_make_nonblocking (socket_t * sock);
int socket_close (socket_t * sock);
int socket_connect (socket_t * sock, const struct sockaddr *saddr, socklen_t len);
int socket_accept (socket_t * sock, socket_t * newsock, struct sockaddr *saddr, socklen_t * len);
int socket_recv (socket_t * sock, void * buf, size_t len);
int socket_send (socket_t * sock, const void *buf, size_t len);

#endif

