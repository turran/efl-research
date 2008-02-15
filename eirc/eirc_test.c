#include <stdio.h>
#include <string.h>

#include <Ecore.h>
#include <Eirc.h>

int _event_connect_cb(void *data, int type, void *event)
{	
	Eirc_Event_Connect *ev;

	ev = event;
	if (!ev)
		return 1;

	printf("connected!\n");

	eirc_channel_join(ev->session, "#eirc", NULL);
	return 1;
}

int _event_notice_cb(void *data, int type, void *event)
{	
	Eirc_Event_Notice *ev;

	ev = event;
	if (!ev)
		return 1;

	printf("notice: %s %s\n", ev->origin, ev->message);

	return 1;
}

int _event_join_cb(void *data, int type, void *event)
{	
	Eirc_Event_Join *ev;

	ev = event;
	if (!ev)
		return 1;

	eirc_command_message(ev->session, ev->channel, "pants on!");

	return 1;
}

int _event_rpl_welcome_cb(void *data, int type, void *event)
{
	Eirc_Event_Rpl_Welcome *ev;

	ev = event;
	if (!ev)
		return 1;

	printf("Got welcome reply from server: %s\n", ev->message);
	return 1;
}

int main(int argc, char **argv)
{
	Eirc_Session *s;

	ecore_init();
	s = eirc_session_new();
	
	ecore_event_handler_add(EIRC_EVENT_CONNECT, _event_connect_cb, NULL);
	ecore_event_handler_add(EIRC_EVENT_NOTICE, _event_notice_cb, NULL);
	ecore_event_handler_add(EIRC_EVENT_JOIN, _event_join_cb, NULL);
	ecore_event_handler_add(EIRC_EVENT_RPL_WELCOME, _event_rpl_welcome_cb, NULL);

	eirc_connect(s, "irc.freenode.net", 6667, NULL, "Code[eirc]", NULL, "Hisham");
	eirc_main(s);
	return 1;
}
