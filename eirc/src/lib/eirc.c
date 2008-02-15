#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "libirc_rfcnumeric.h"
#include "libircclient.h"
#include "Eirc.h"

#include <Ecore.h>

#define IF_FREE(ptr) if (ptr) free(ptr);
#define IF_STRDUP(dst, src) if (src) { dst = strdup(src); } else { dst = NULL; }
#define ALLOC(t) calloc(1, sizeof(t));

#define SESS_INT_RET \
irc_session_t *sess; \
if (!s) \
	return 0; \
sess = s->session;

#define SESS_VOID_RET \
irc_session_t *sess; \
if (!s) \
	return; \
sess = s->session;

#define EV_SESS_CTX_ALLOC_SET \
	ev->session = ALLOC(Eirc_Session); \
	ev->session->session = session;

/**
 * Takes in an origin (nickname) or the form nick!host\@ircserver
 * and writes the nickname alone (nick) into the preallocated
 * supplied buffer.
 */
void eirc_nick_strip(const char *origin, char *stripped_nick, int stripped_nick_size)
{
	if (!stripped_nick || !stripped_nick_size)
		return;
	irc_target_get_nick (origin, stripped_nick, stripped_nick_size);
}

/**
 * Takes in an origin (nickname) or the form nick!host\@ircserver
 * and writes the host alone into the preallocated
 * supplied buffer.
 */
void eirc_host_strip(const char *origin, char *stripped_host, int stripped_host_size)
{
	if (!stripped_host || !stripped_host_size)
		return;
	irc_target_get_host (origin, stripped_host, stripped_host_size);
}



void addlog (const char * fmt, ...)
{
	FILE * fp;
	char buf[1024];
	va_list va_alist;

	va_start (va_alist, fmt);
#if defined (WIN32)
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
#else
	vsnprintf (buf, sizeof(buf), fmt, va_alist);
#endif
	va_end (va_alist);

	printf ("%s\n", buf);

	if ( (fp = fopen ("irctest.log", "ab")) != 0 )
	{
		fprintf (fp, "%s\n", buf);
		fclose (fp);
	}
}

void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char buf[512];
	int cnt;

	buf[0] = '\0';

	for ( cnt = 0; cnt < count; cnt++ )
	{
		if ( cnt )
			strcat (buf, "|");

		strcat (buf, params[cnt]);
	}

	addlog ("Event \"%s\", origin: \"%s\", params: %d [%s]", event, origin ? origin : "NULL", cnt, buf);
}

static void
_eirc_event_connect_free(void *data, void *event)
{
	Eirc_Event_Connect *ev;

	ev = event;
	if (!ev)
		return;
	
	IF_FREE(ev->session);
	IF_FREE(ev);
}

static void
event_connect (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Connect *ev;

	ev = ALLOC(Eirc_Event_Connect);
	EV_SESS_CTX_ALLOC_SET;
	ecore_event_add(EIRC_EVENT_CONNECT, ev, _eirc_event_connect_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void
_eirc_event_join_free(void *data, void *event)
{
	Eirc_Event_Join *ev;

	ev = event;
	if (!ev)
		return;
	
	IF_FREE(ev->session);
	IF_FREE(ev->origin);
	IF_FREE(ev->channel);
	IF_FREE(ev);
}

static void
event_join (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Join *ev;

	if (count != 1)
		return;

	ev = ALLOC(Eirc_Event_Join);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->channel, params[0]);
	ecore_event_add(EIRC_EVENT_JOIN, ev, _eirc_event_join_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void
_eirc_event_nick_free(void *data, void *event)
{
	Eirc_Event_Nick *ev;

	ev = event;
	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->new_nick);
	IF_FREE(ev);
}

static void
event_nick (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Nick *ev;

	if (count != 1)
		return;

	ev = ALLOC(Eirc_Event_Nick);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->new_nick, params[0]);
	ecore_event_add(EIRC_EVENT_NICK, ev, _eirc_event_nick_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void
_eirc_event_quit_free(void *data, void *event)
{
	Eirc_Event_Quit *ev;

	ev = event;
	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->reason);
	IF_FREE(ev);
}

static void
event_quit (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Quit *ev;

	if (count != 1)
		return;

	ev = ALLOC(Eirc_Event_Quit);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->reason ,params[0]);
	ecore_event_add(EIRC_EVENT_QUIT, ev, _eirc_event_quit_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void
_eirc_event_part_free(void *data, void *event)
{
	Eirc_Event_Part *ev;

	ev = event;
	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->channel);
	IF_FREE(ev->reason);
	IF_FREE(ev);
}

static void
event_part (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Part *ev;

	if (count != 2)
		return;

	ev = ALLOC(Eirc_Event_Part);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->channel, params[0]);
	IF_STRDUP(ev->reason, params[1]);
	ecore_event_add(EIRC_EVENT_PART, ev, _eirc_event_part_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void
_eirc_event_mode_free(void *data, void *event)
{
	Eirc_Event_Mode *ev;

	ev = event;
	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->channel);
	IF_FREE(ev->changed_mode);
	IF_FREE(ev->mode_arg);
	IF_FREE(ev);
}

static void
event_mode (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Mode *ev;

	if (count != 3)
		return;

	ev = ALLOC(Eirc_Event_Mode);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->channel, params[0]);
	IF_STRDUP(ev->changed_mode, params[1]);
	IF_STRDUP(ev->mode_arg, params[2]);
	ecore_event_add(EIRC_EVENT_MODE, ev, _eirc_event_mode_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void
_eirc_event_topic_free(void *data, void *event)
{
	Eirc_Event_Topic *ev;

	ev = event;
	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->channel);
	IF_FREE(ev->topic);
	IF_FREE(ev);
}

static void
event_topic (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Topic *ev;

	if (count != 2)
		return;

	ev = ALLOC(Eirc_Event_Topic);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->channel, params[0]);
	IF_STRDUP(ev->topic, params[1]);
	ecore_event_add(EIRC_EVENT_TOPIC, ev, _eirc_event_topic_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void
_eirc_event_kick_free(void *data, void *event)
{
	Eirc_Event_Kick *ev;

	ev = event;
	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->channel);
	IF_FREE(ev->nick_kicked);
	IF_FREE(ev->kick_reason);
	IF_FREE(ev);
}

static void
event_kick (irc_session_t * session, 
						const char * event, 
						const char * origin, 
						const char ** params, 
						unsigned int count)
{
	Eirc_Event_Kick *ev;

	if (count != 3)
		return;

	ev = ALLOC(Eirc_Event_Kick);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->channel, params[0]);
	IF_STRDUP(ev->nick_kicked, params[1]);
	IF_STRDUP(ev->kick_reason, params[2]);
	ecore_event_add(EIRC_EVENT_KICK, ev, _eirc_event_kick_free, NULL);

	dump_event (session, event, origin, params, count);
}

static void 
_eirc_event_channel_free(void *data, void *event)
{
	Eirc_Event_Channel *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->channel);
	IF_FREE(ev->message);
	IF_FREE(ev);
}

static void 
event_channel (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Channel *ev;

	if ( count != 2 )
		return;
	
	ev = ALLOC(Eirc_Event_Channel);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->channel, params[0]);
	IF_STRDUP(ev->message, params[1]);
	ecore_event_add(EIRC_EVENT_CHANNEL, ev, _eirc_event_channel_free, NULL);
}

static void 
_eirc_event_privmsg_free(void *data, void *event)
{
	Eirc_Event_Privmsg *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->nick);
	IF_FREE(ev->message);
	IF_FREE(ev);
}

static void 
event_privmsg (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Privmsg *ev;

	if ( count != 2 )
		return;
	
	ev = ALLOC(Eirc_Event_Privmsg);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->nick, params[0]);
	IF_STRDUP(ev->message, params[1]);
	ecore_event_add(EIRC_EVENT_PRIVMSG, ev, _eirc_event_privmsg_free, NULL);
}

static void 
_eirc_event_notice_free(void *data, void *event)
{
	Eirc_Event_Notice *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->channel);
	IF_FREE(ev->message);
	IF_FREE(ev);
}

static void 
event_notice (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Notice *ev;

	if ( count != 2 )
		return;
	
	ev = ALLOC(Eirc_Event_Notice);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->channel, params[0]);
	IF_STRDUP(ev->message, params[1]);
	ecore_event_add(EIRC_EVENT_NOTICE, ev, _eirc_event_notice_free, NULL);
}

static void 
_eirc_event_invite_free(void *data, void *event)
{
	Eirc_Event_Invite *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->nick);
	IF_FREE(ev->channel);
	IF_FREE(ev);
}

static void 
event_invite (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Invite *ev;

	if ( count != 2 )
		return;
	
	ev = ALLOC(Eirc_Event_Invite);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->nick, params[0]);
	IF_STRDUP(ev->channel, params[1]);
	ecore_event_add(EIRC_EVENT_INVITE, ev, _eirc_event_invite_free, NULL);
}

static void 
_eirc_event_umode_free(void *data, void *event)
{
	Eirc_Event_Umode *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->changed_mode);
}

static void 
event_umode (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Umode *ev;

	if ( count != 1 )
		return;
	
	ev = ALLOC(Eirc_Event_Umode);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->changed_mode, params[0]);
	ecore_event_add(EIRC_EVENT_UMODE, ev, _eirc_event_umode_free, NULL);
}

static void 
_eirc_event_ctcp_rep_free(void *data, void *event)
{
	Eirc_Event_Ctcp_Rep *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->message);
	IF_FREE(ev);
}

static void 
event_ctcp_rep (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Ctcp_Rep *ev;

	if ( count != 1 )
		return;
	
	ev = ALLOC(Eirc_Event_Ctcp_Rep);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->message, params[0]);
	ecore_event_add(EIRC_EVENT_CTCP_REP, ev, _eirc_event_ctcp_rep_free, NULL);
}

static void 
_eirc_event_ctcp_req_free(void *data, void *event)
{
	Eirc_Event_Ctcp_Req *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->message);
	IF_FREE(ev);
}

static void 
event_ctcp_req (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Ctcp_Rep *ev;

	if ( count != 1 )
		return;
	
	ev = ALLOC(Eirc_Event_Ctcp_Req);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->message, params[0]);
	ecore_event_add(EIRC_EVENT_CTCP_REQ, ev, _eirc_event_ctcp_req_free, NULL);
}

static void 
_eirc_event_ctcp_action_free(void *data, void *event)
{
	Eirc_Event_Ctcp_Action *ev = event;

	if (!ev)
		return;

	IF_FREE(ev->origin);
	IF_FREE(ev->message);
	IF_FREE(ev);
}

static void 
event_ctcp_action (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Ctcp_Action *ev;

	if ( count != 1 )
		return;
	
	ev = ALLOC(Eirc_Event_Ctcp_Action);
	EV_SESS_CTX_ALLOC_SET;
	IF_STRDUP(ev->origin, origin);
	IF_STRDUP(ev->message, params[0]);
	ecore_event_add(EIRC_EVENT_CTCP_ACTION, ev, _eirc_event_ctcp_action_free, NULL);
}

static void 
_eirc_event_unknown_free(void *data, void *event)
{
	Eirc_Event_Unknown *ev = event;

	if (!ev)
		return;

	IF_FREE(ev);
}

static void 
event_unknown (irc_session_t * session, 
							const char * event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	Eirc_Event_Unknown *ev;

	if ( count != 0 )
		return;
	
	ev = ALLOC(Eirc_Event_Unknown);
	EV_SESS_CTX_ALLOC_SET;
	ecore_event_add(EIRC_EVENT_UNKNOWN, ev, _eirc_event_unknown_free, NULL);
}

static void 
_eirc_event_numeric_free(void *data, void *event)
{
	Eirc_Event_Numeric *ev = event;

	if (!ev)
		return;

	IF_FREE(ev);
}

static void
_eirc_event_welcome_free(void *data, void *event)
{
	Eirc_Event_Rpl_Welcome *ev = event;

	if (!event)
		return;

	IF_FREE(ev->nick);
	IF_FREE(ev->message);
	IF_FREE(ev);
}

static void 
event_numeric (irc_session_t * session, 
							unsigned int event, 
							const char * origin, 
							const char ** params, 
							unsigned int count)
{
	switch(event)
	{
		case LIBIRC_RFC_RPL_WELCOME:
		{
			Eirc_Event_Rpl_Welcome *ev;
			ev = ALLOC(Eirc_Event_Rpl_Welcome);
			IF_STRDUP(ev->nick, params[0]);
			IF_STRDUP(ev->message, params[1]);
			ecore_event_add(EIRC_EVENT_RPL_WELCOME, ev, _eirc_event_welcome_free, NULL);
		}
		break;

		default:
		{
			Eirc_Event_Numeric *ev;
			ev = ALLOC(Eirc_Event_Numeric);
			EV_SESS_CTX_ALLOC_SET;
			ev->event_number = event;
			ecore_event_add(EIRC_EVENT_NUMERIC, ev, _eirc_event_numeric_free, NULL);
		}
		break;	
	}
}

void dcc_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	static int count = 1;
	char buf[12];

	switch (status)
	{
	case LIBIRC_ERR_CLOSED:
		printf ("DCC %d: chat closed\n", id);
		break;

	case 0:
		if ( !data )
		{
			printf ("DCC %d: chat connected\n", id);
			irc_dcc_msg	(session, id, "Hehe");
		}
		else 
		{
			printf ("DCC %d: %s\n", id, data);
			sprintf (buf, "DCC [%d]: %d", id, count++);
			irc_dcc_msg	(session, id, buf);
		}
		break;

	default:
		printf ("DCC %d: error %s\n", id, irc_strerror(status));
		break;
	}
}


void dcc_file_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	if ( status == 0 && length == 0 )
	{
		printf ("File sent successfully\n");

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else if ( status )
	{
		printf ("File sent error: %d\n", status);

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else
	{
		if ( ctx )
			fwrite (data, 1, length, (FILE*) ctx);
		printf ("File sent progress: %d\n", length);
	}
}


void irc_event_dcc_chat (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid)
{
	printf ("DCC chat [%d] requested from '%s' (%s)\n", dccid, nick, addr);

	irc_dcc_accept (session, dccid, 0, dcc_recv_callback);
}


void irc_event_dcc_send (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
	FILE * fp;
	printf ("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)\n", dccid, nick, addr, filename, size);

	if ( (fp = fopen ("file", "wb")) == 0 )
		abort();

	irc_dcc_accept (session, dccid, fp, dcc_file_recv_callback);
}

/**
 * Create a new session handler
 */
Eirc_Session *
eirc_session_new(void)
{
	Eirc_Session *s;
	irc_callbacks_t	callbacks;
	irc_session_t *sess;

	EIRC_EVENT_CONNECT = ecore_event_type_new();
	EIRC_EVENT_JOIN = ecore_event_type_new();
	EIRC_EVENT_NICK = ecore_event_type_new();
	EIRC_EVENT_QUIT = ecore_event_type_new();
	EIRC_EVENT_PART = ecore_event_type_new();
	EIRC_EVENT_MODE = ecore_event_type_new();
	EIRC_EVENT_TOPIC = ecore_event_type_new();
	EIRC_EVENT_KICK = ecore_event_type_new();
	EIRC_EVENT_CHANNEL = ecore_event_type_new();
	EIRC_EVENT_PRIVMSG = ecore_event_type_new();
	EIRC_EVENT_NOTICE = ecore_event_type_new();
	EIRC_EVENT_INVITE = ecore_event_type_new();
	EIRC_EVENT_UMODE = ecore_event_type_new();
	EIRC_EVENT_CTCP_REP = ecore_event_type_new();
	EIRC_EVENT_CTCP_REQ = ecore_event_type_new();
	EIRC_EVENT_CTCP_ACTION = ecore_event_type_new();
	EIRC_EVENT_UNKNOWN = ecore_event_type_new();
	EIRC_EVENT_NUMERIC = ecore_event_type_new();
	EIRC_EVENT_DCC_CHAT_REQ = ecore_event_type_new();
	EIRC_EVENT_DCC_SEND_REQ = ecore_event_type_new();

	/* Numeric events */

	EIRC_EVENT_RPL_WELCOME = ecore_event_type_new();
	EIRC_EVENT_RPL_YOURHOST = ecore_event_type_new();
	EIRC_EVENT_RPL_CREATED = ecore_event_type_new();
	EIRC_EVENT_RPL_MYINFO = ecore_event_type_new();
	EIRC_EVENT_RPL_BOUNCE = ecore_event_type_new();
	EIRC_EVENT_RPL_USERHOST = ecore_event_type_new();
	EIRC_EVENT_RPL_ISON = ecore_event_type_new();
	EIRC_EVENT_RPL_AWAY = ecore_event_type_new();
	EIRC_EVENT_RPL_UNAWAY = ecore_event_type_new();
	EIRC_EVENT_RPL_NOWAWAY = ecore_event_type_new();
	EIRC_EVENT_RPL_WHOISUSER = ecore_event_type_new();
	EIRC_EVENT_RPL_WHOISSERVER = ecore_event_type_new();
	EIRC_EVENT_RPL_WHOISOPERATOR = ecore_event_type_new();
	EIRC_EVENT_RPL_WHOISIDLE = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFWHOIS = ecore_event_type_new();
	EIRC_EVENT_RPL_WHOISCHANNELS = ecore_event_type_new();
	EIRC_EVENT_RPL_WHOWASUSER = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFWHOWAS = ecore_event_type_new();
	EIRC_EVENT_RPL_LIST = ecore_event_type_new();
	EIRC_EVENT_RPL_LISTEND = ecore_event_type_new();
	EIRC_EVENT_RPL_UNIQOPIS = ecore_event_type_new();
	EIRC_EVENT_RPL_CHANNELMODEIS = ecore_event_type_new();
	EIRC_EVENT_RPL_NOTOPIC = ecore_event_type_new();
	EIRC_EVENT_RPL_TOPIC = ecore_event_type_new();
	EIRC_EVENT_RPL_INVITING = ecore_event_type_new();
	EIRC_EVENT_RPL_SUMMONING = ecore_event_type_new();
	EIRC_EVENT_RPL_INVITELIST = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFINVITELIST = ecore_event_type_new();
	EIRC_EVENT_RPL_EXCEPTLIST = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFEXCEPTLIST = ecore_event_type_new();
	EIRC_EVENT_RPL_VERSION = ecore_event_type_new();
	EIRC_EVENT_RPL_WHOREPLY = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFWHO = ecore_event_type_new();
	EIRC_EVENT_RPL_NAMREPLY = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFNAMES = ecore_event_type_new();
	EIRC_EVENT_RPL_LINKS = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFLINKS = ecore_event_type_new();
	EIRC_EVENT_RPL_BANLIST = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFBANLIST = ecore_event_type_new();
	EIRC_EVENT_RPL_INFO = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFINFO = ecore_event_type_new();
	EIRC_EVENT_RPL_MOTDSTART = ecore_event_type_new();
	EIRC_EVENT_RPL_MOTD = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFMOTD = ecore_event_type_new();
	EIRC_EVENT_RPL_YOUREOPER = ecore_event_type_new();
	EIRC_EVENT_RPL_REHASHING = ecore_event_type_new();
	EIRC_EVENT_RPL_YOURESERVICE = ecore_event_type_new();
	EIRC_EVENT_RPL_TIME = ecore_event_type_new();
	EIRC_EVENT_RPL_USERSSTART = ecore_event_type_new();
	EIRC_EVENT_RPL_USERS = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFUSERS = ecore_event_type_new();
	EIRC_EVENT_RPL_NOUSERS = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACELINK = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACECONNECTING = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACEHANDSHAKE = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACEUNKNOWN = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACEOPERATOR = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACEUSER = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACESERVER = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACESERVICE = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACENEWTYPE = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACECLASS = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACELOG = ecore_event_type_new();
	EIRC_EVENT_RPL_TRACEEND = ecore_event_type_new();
	EIRC_EVENT_RPL_STATSLINKINFO = ecore_event_type_new();
	EIRC_EVENT_RPL_STATSCOMMANDS = ecore_event_type_new();
	EIRC_EVENT_RPL_ENDOFSTATS = ecore_event_type_new();
	EIRC_EVENT_RPL_STATSUPTIME = ecore_event_type_new();
	EIRC_EVENT_RPL_STATSOLINE = ecore_event_type_new();
	EIRC_EVENT_RPL_UMODEIS = ecore_event_type_new();
	EIRC_EVENT_RPL_SERVLIST = ecore_event_type_new();
	EIRC_EVENT_RPL_SERVLISTEND = ecore_event_type_new();
	EIRC_EVENT_RPL_LUSERCLIENT = ecore_event_type_new();
	EIRC_EVENT_RPL_LUSEROP = ecore_event_type_new();
	EIRC_EVENT_RPL_LUSERUNKNOWN = ecore_event_type_new();
	EIRC_EVENT_RPL_LUSERCHANNELS = ecore_event_type_new();
	EIRC_EVENT_RPL_LUSERME = ecore_event_type_new();
	EIRC_EVENT_RPL_ADMINME = ecore_event_type_new();
	EIRC_EVENT_RPL_ADMINLOC1 = ecore_event_type_new();
	EIRC_EVENT_RPL_ADMINLOC2 = ecore_event_type_new();
	EIRC_EVENT_RPL_ADMINEMAIL = ecore_event_type_new();
	EIRC_EVENT_RPL_TRYAGAIN = ecore_event_type_new();
	EIRC_EVENT_ERR_NOSUCHNICK = ecore_event_type_new();
	EIRC_EVENT_ERR_NOSUCHSERVER = ecore_event_type_new();
	EIRC_EVENT_ERR_NOSUCHCHANNEL = ecore_event_type_new();
	EIRC_EVENT_ERR_CANNOTSENDTOCHAN = ecore_event_type_new();
	EIRC_EVENT_ERR_TOOMANYCHANNELS = ecore_event_type_new();
	EIRC_EVENT_ERR_WASNOSUCHNICK = ecore_event_type_new();
	EIRC_EVENT_ERR_TOOMANYTARGETS = ecore_event_type_new();
	EIRC_EVENT_ERR_NOSUCHSERVICE = ecore_event_type_new();
	EIRC_EVENT_ERR_NOORIGIN = ecore_event_type_new();
	EIRC_EVENT_ERR_NORECIPIENT = ecore_event_type_new();
	EIRC_EVENT_ERR_NOTEXTTOSEND = ecore_event_type_new();
	EIRC_EVENT_ERR_NOTOPLEVEL = ecore_event_type_new();
	EIRC_EVENT_ERR_WILDTOPLEVEL = ecore_event_type_new();
	EIRC_EVENT_ERR_BADMASK = ecore_event_type_new();
	EIRC_EVENT_ERR_UNKNOWNCOMMAND = ecore_event_type_new();
	EIRC_EVENT_ERR_NOMOTD = ecore_event_type_new();
	EIRC_EVENT_ERR_NOADMININFO = ecore_event_type_new();
	EIRC_EVENT_ERR_FILEERROR = ecore_event_type_new();
	EIRC_EVENT_ERR_NONICKNAMEGIVEN = ecore_event_type_new();
	EIRC_EVENT_ERR_ERRONEUSNICKNAME = ecore_event_type_new();
	EIRC_EVENT_ERR_NICKNAMEINUSE = ecore_event_type_new();
	EIRC_EVENT_ERR_NICKCOLLISION = ecore_event_type_new();
	EIRC_EVENT_ERR_UNAVAILRESOURCE = ecore_event_type_new();
	EIRC_EVENT_ERR_USERNOTINCHANNEL = ecore_event_type_new();
	EIRC_EVENT_ERR_NOTONCHANNEL = ecore_event_type_new();
	EIRC_EVENT_ERR_USERONCHANNEL = ecore_event_type_new();
	EIRC_EVENT_ERR_NOLOGIN = ecore_event_type_new();
	EIRC_EVENT_ERR_SUMMONDISABLED = ecore_event_type_new();
	EIRC_EVENT_ERR_USERSDISABLED = ecore_event_type_new();
	EIRC_EVENT_ERR_NOTREGISTERED = ecore_event_type_new();
	EIRC_EVENT_ERR_NEEDMOREPARAMS = ecore_event_type_new();
	EIRC_EVENT_ERR_ALREADYREGISTRED = ecore_event_type_new();
	EIRC_EVENT_ERR_NOPERMFORHOST = ecore_event_type_new();
	EIRC_EVENT_ERR_PASSWDMISMATCH = ecore_event_type_new();
	EIRC_EVENT_ERR_YOUREBANNEDCREEP = ecore_event_type_new();
	EIRC_EVENT_ERR_YOUWILLBEBANNED = ecore_event_type_new();
	EIRC_EVENT_ERR_KEYSET = ecore_event_type_new();
	EIRC_EVENT_ERR_CHANNELISFULL = ecore_event_type_new();
	EIRC_EVENT_ERR_UNKNOWNMODE = ecore_event_type_new();
	EIRC_EVENT_ERR_INVITEONLYCHAN = ecore_event_type_new();
	EIRC_EVENT_ERR_BANNEDFROMCHAN = ecore_event_type_new();
	EIRC_EVENT_ERR_BADCHANNELKEY = ecore_event_type_new();
	EIRC_EVENT_ERR_BADCHANMASK = ecore_event_type_new();
	EIRC_EVENT_ERR_NOCHANMODES = ecore_event_type_new();
	EIRC_EVENT_ERR_BANLISTFULL = ecore_event_type_new();
	EIRC_EVENT_ERR_NOPRIVILEGES = ecore_event_type_new();
	EIRC_EVENT_ERR_CHANOPRIVSNEEDED = ecore_event_type_new();
	EIRC_EVENT_ERR_CANTKILLSERVER = ecore_event_type_new();
	EIRC_EVENT_ERR_RESTRICTED = ecore_event_type_new();
	EIRC_EVENT_ERR_UNIQOPPRIVSNEEDED = ecore_event_type_new();
	EIRC_EVENT_ERR_NOOPERHOST = ecore_event_type_new();
	EIRC_EVENT_ERR_UMODEUNKNOWNFLAG = ecore_event_type_new();
	EIRC_EVENT_ERR_USERSDONTMATCH = ecore_event_type_new();

	memset(&callbacks, 0, sizeof(callbacks));

	callbacks.event_connect = event_connect;
	callbacks.event_join = event_join;
	callbacks.event_nick = event_nick;
	callbacks.event_quit = event_quit;
	callbacks.event_part = event_part;
	callbacks.event_mode = event_mode;
	callbacks.event_topic = event_topic;
	callbacks.event_kick = event_kick;
	callbacks.event_channel = event_channel;
	callbacks.event_privmsg = event_privmsg;
	callbacks.event_notice = event_notice;
	callbacks.event_invite = event_invite;
	callbacks.event_umode = event_umode;
	callbacks.event_ctcp_rep = event_ctcp_rep;
	callbacks.event_ctcp_req = event_ctcp_req;
	callbacks.event_ctcp_action = event_ctcp_action;
	callbacks.event_unknown = event_unknown;
	callbacks.event_numeric = event_numeric;

	callbacks.event_dcc_chat_req = irc_event_dcc_chat;
	callbacks.event_dcc_send_req = irc_event_dcc_send;

	sess = irc_create_session(&callbacks);

	if (!sess)
	{
		fprintf(stderr, "FATAL: Could not create session!\n");
		return NULL;
	}

	s = ALLOC(Eirc_Session);
	s->session = sess;
	return s;
}

/**
 * Destroy a session handler
 */
void
eirc_session_destroy(Eirc_Session *s)
{
	SESS_VOID_RET;

	irc_destroy_session(sess);

	IF_FREE(s);
}

/**
 * Connect to an irc server
 */
int 
eirc_connect(Eirc_Session *s, 
						const char *server, 
						int port, 
						const char *server_password, 
						const char *nick, 
						const char *username, 
						const char *realname)
{
	SESS_INT_RET;

	if (irc_connect (sess, server, port, server_password, nick, username, realname))
	{
		fprintf(stderr, "Could not connect: %s\n", irc_strerror (irc_errno(sess)));
		return 0;
	}

	return 1;
}

/**
 * Disconnects from irc server.
 */
void
eirc_disconnect(Eirc_Session *s)
{
	SESS_VOID_RET;
	irc_disconnect(sess);
}

/**
 * Figure out if the session is connected
 */
int
eirc_is_connected(Eirc_Session *s)
{
	SESS_INT_RET;
	
	return irc_is_connected(sess);
}

/**
 * Sends raw data to the server
 */
int
eirc_raw_data_send(Eirc_Session *s, const char *format, ...)
{
	SESS_INT_RET;

	/* TODO: implement this */
	//return irc_send_raw(sess, format, ...);
	return 0;
}

/**
 * Send the quit command to the server.
 */
int
eirc_command_quit(Eirc_Session *s, const char *reason)
{
	SESS_INT_RET;

	return irc_cmd_quit(sess, reason);
}

/**
 * Send the part command to a channel
 */
int
eirc_command_part(Eirc_Session *s, const char *channel)
{
	SESS_INT_RET;

	return irc_cmd_part(sess, channel);
}

/**
 * Invite a nick to a channel
 */
int
eirc_command_invite(Eirc_Session *s, const char *nick, const char *channel)
{
	SESS_INT_RET;

	return irc_cmd_invite(sess, nick, channel);
}

/**
 * Get list of users in a certain channel. Names are send as a numeric reply
 * with RPL_NAMREPLY.
 */
int
eirc_command_names(Eirc_Session *s, const char *channel)
{
	SESS_INT_RET;

	return irc_cmd_names(sess, channel);
}

/**
 * Ask server for active (existing) channel list. List is returned using
 * LIBIRC_RFC_RPL_LISTSTART - LIBIRC_RFC_RPL_LIST - LIBIRC_RFC_RPL_LISTEND
 * sequence. If channel is NULL, lists all channels.
 */
int 
eirc_command_channels_list(Eirc_Session *s, const char *channel)
{
	SESS_INT_RET;

	return irc_cmd_list(sess, channel);
}

/**
 * View or set  the topic of a channel. If topic is NULL, it will not be
 * set.
 */
int
eirc_command_topic(Eirc_Session *s, const char *channel, const char *topic)
{
	SESS_INT_RET;

	return irc_cmd_topic(sess, channel, topic);
}

/**
 * Used to view or set the channel mode
 */
int
eirc_command_channel_mode(Eirc_Session *s, const char *channel, const char *mode)
{
	SESS_INT_RET;

	return irc_cmd_channel_mode(sess, channel, mode);
}

/**
 * Used to view or set the user mode
 */
int
eirc_command_user_mode(Eirc_Session *s, const char *mode)
{
	SESS_INT_RET;

	return irc_cmd_user_mode(sess, mode);
}

/**
 * Used to change the user's nickname
 */
int
eirc_command_nick(Eirc_Session *s, const char *newnick)
{
	SESS_INT_RET;

	return irc_cmd_nick(sess, newnick);
}

/**
 * Queries information about a nick (/whois)
 */
int
eirc_command_whois(Eirc_Session *s, const char *nick)
{
	SESS_INT_RET;

	return irc_cmd_whois(sess, nick);
}

/**
 * Send a channel or a private message
 */
int
eirc_command_message(Eirc_Session *s, const char *nick_or_channel, const char *message)
{
	SESS_INT_RET;

	return irc_cmd_msg(sess, nick_or_channel, message);
}

/**
 * Send an action (/me) to a channel or person
 */
int
eirc_command_me(Eirc_Session *s, const char *nick_or_channel, const char *text)
{
	SESS_INT_RET;

	return irc_cmd_me(sess, nick_or_channel, text);
}

/**
 * Send a notice to a channel or person
 */
int
eirc_command_notice(Eirc_Session *s, const char *nick_or_channel, const char *text)
{
	SESS_INT_RET;

	return irc_cmd_notice(sess, nick_or_channel, text);
}

/**
 * Kick a person from a channel
 */
int
eirc_command_kick(Eirc_Session *s, const char *nick, const char *channel, const char *reason)
{
	SESS_INT_RET;

	return irc_cmd_kick(sess, nick, channel, reason);
}

/**
 * Send a ctcp request (VERSION, FINGER, PING, TIME)
 */
int
eirc_command_ctcp_request(Eirc_Session *s, const char *nick, const char *request)
{
	SESS_INT_RET;

	return irc_cmd_ctcp_request(sess, nick, request);
}

/**
 * Send a ctcp reply (VERSION, FINGER, PING, TIME)
 */
int
eirc_command_ctcp_reply(Eirc_Session *s, const char *nick, const char *reply)
{
	SESS_INT_RET;

	return irc_cmd_ctcp_request(sess, nick, reply);
}



/**
 * Join a channel with a certain (optiona) key
 */
int
eirc_channel_join(Eirc_Session *s, 
									const char *channel, 
									const char *key)
{
	SESS_INT_RET;
	
	printf("tyring to join %s\n", channel);
	return irc_cmd_join(sess, channel, key);
}
int 
eirc_main(Eirc_Session *s)
{
	SESS_INT_RET;

	irc_run(sess);
	ecore_shutdown();

	return 1;
}

