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
 * $Id: libirc_dcc.h 18 2004-09-14 19:50:10Z gyunaev $
 */

#ifndef _EIRC_DCC_H
#define _EIRC_DCC_H

#include <stdio.h>
#include <stdlib.h>

#include "libirc_params.h"
#include "libirc_sockets.h"
#include "libirc_portable.h"

/*
 * This structure keeps the state of a single DCC connection.
 */
struct irc_dcc_session_s
{
	irc_dcc_session_t	*	next;

	irc_dcc_t		id;
	void		*	ctx;
	socket_t		sock;		/*!< DCC socket */
	int				dccmode;	/*!< Boolean value to differ chat vs send 
	                             requests. Changes the cb behavior - when
	                             it is chat, data is sent by lines with 
	                             stripped CRLFs. In file mode, the data
	                             is sent as-is */
	int				state;
	time_t			timeout;

	FILE		*	dccsend_file_fp;
	unsigned int	received_file_size;
	unsigned int	file_confirm_offset;

	struct sockaddr_in	remote_addr;

	char 			incoming_buf[LIBIRC_DCC_BUFFER_SIZE];
	unsigned int	incoming_offset;

	char 			outgoing_buf[LIBIRC_DCC_BUFFER_SIZE];
	unsigned int	outgoing_offset;
	port_mutex_t	mutex_outbuf;

	irc_dcc_callback_t		cb;
};

irc_dcc_session_t * libirc_find_dcc_session (irc_session_t * session, irc_dcc_t dccid, int lock_list);
void libirc_dcc_destroy_nolock (irc_session_t * session, irc_dcc_t dccid);
void libirc_remove_dcc_session (irc_session_t * session, irc_dcc_session_t * dcc, int lock_list);
void libirc_dcc_add_descriptors (irc_session_t * ircsession, fd_set *in_set, fd_set *out_set, int * maxfd);
void libirc_dcc_process_descriptors (irc_session_t * ircsession, fd_set *in_set, fd_set *out_set);
int libirc_new_dcc_session (irc_session_t * session, unsigned long ip, unsigned short port, int dccmode, void * ctx, irc_dcc_session_t ** pdcc);
int irc_dcc_destroy (irc_session_t * session, irc_dcc_t dccid);
int irc_dcc_chat (irc_session_t * session, void * ctx, const char * nick, irc_dcc_callback_t callback, irc_dcc_t * dccid);
int irc_dcc_msg (irc_session_t * session, irc_dcc_t dccid, const char * text);
void libirc_dcc_request (irc_session_t * session, const char * nick, const char * req);
int irc_dcc_accept (irc_session_t * session, irc_dcc_t dccid, void * ctx, irc_dcc_callback_t callback);
int irc_dcc_decline (irc_session_t * session, irc_dcc_t dccid);
int irc_dcc_sendfile (irc_session_t * session, void * ctx, const char * nick, const char * filename, irc_dcc_callback_t callback, irc_dcc_t * dccid);


#endif 
