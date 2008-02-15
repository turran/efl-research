#ifndef EIRC_H
#define EIRC_H

typedef struct _Eirc_Session Eirc_Session;

typedef struct _Eirc_Event_Connect Eirc_Event_Connect;
typedef struct _Eirc_Event_Join Eirc_Event_Join;
typedef struct _Eirc_Event_Nick Eirc_Event_Nick;
typedef struct _Eirc_Event_Quit Eirc_Event_Quit;
typedef struct _Eirc_Event_Part Eirc_Event_Part;
typedef struct _Eirc_Event_Mode Eirc_Event_Mode;
typedef struct _Eirc_Event_Topic Eirc_Event_Topic;
typedef struct _Eirc_Event_Kick Eirc_Event_Kick;
typedef struct _Eirc_Event_Channel Eirc_Event_Channel;
typedef struct _Eirc_Event_Privmsg Eirc_Event_Privmsg;
typedef struct _Eirc_Event_Notice Eirc_Event_Notice;
typedef struct _Eirc_Event_Invite Eirc_Event_Invite;
typedef struct _Eirc_Event_Umode Eirc_Event_Umode;
typedef struct _Eirc_Event_Ctcp_Rep Eirc_Event_Ctcp_Rep;
typedef struct _Eirc_Event_Ctcp_Req Eirc_Event_Ctcp_Req;
typedef struct _Eirc_Event_Ctcp_Action Eirc_Event_Ctcp_Action;
typedef struct _Eirc_Event_Unknown Eirc_Event_Unknown;
typedef struct _Eirc_Event_Numeric Eirc_Event_Numeric;
typedef struct _Eirc_Event_Dcc_Chat_Req Eirc_Event_Dcc_Chat_Req;
typedef struct _Eirc_Event_Dcc_Send_Req Eirc_Event_Dcc_Send_Req;

/* Numeric events */
typedef struct _Eirc_Event_Rpl_Welcome Eirc_Event_Rpl_Welcome;

int EIRC_EVENT_CONNECT;
int EIRC_EVENT_JOIN;
int EIRC_EVENT_NICK;
int EIRC_EVENT_QUIT;
int EIRC_EVENT_PART;
int EIRC_EVENT_MODE;
int EIRC_EVENT_TOPIC;
int EIRC_EVENT_KICK;
int EIRC_EVENT_CHANNEL;
int EIRC_EVENT_PRIVMSG;
int EIRC_EVENT_NOTICE;
int EIRC_EVENT_INVITE;
int EIRC_EVENT_UMODE;
int EIRC_EVENT_CTCP_REP;
int EIRC_EVENT_CTCP_REQ;
int EIRC_EVENT_CTCP_ACTION;
int EIRC_EVENT_UNKNOWN;
int EIRC_EVENT_NUMERIC;
int EIRC_EVENT_DCC_CHAT_REQ;
int EIRC_EVENT_DCC_SEND_REQ;

/* Numeric events */
int EIRC_EVENT_RPL_WELCOME;
int EIRC_EVENT_RPL_YOURHOST;
int EIRC_EVENT_RPL_CREATED;
int EIRC_EVENT_RPL_MYINFO;
int EIRC_EVENT_RPL_BOUNCE;
int EIRC_EVENT_RPL_USERHOST;
int EIRC_EVENT_RPL_ISON;
int EIRC_EVENT_RPL_AWAY;
int EIRC_EVENT_RPL_UNAWAY;
int EIRC_EVENT_RPL_NOWAWAY;
int EIRC_EVENT_RPL_WHOISUSER;
int EIRC_EVENT_RPL_WHOISSERVER;
int EIRC_EVENT_RPL_WHOISOPERATOR;
int EIRC_EVENT_RPL_WHOISIDLE;
int EIRC_EVENT_RPL_ENDOFWHOIS;
int EIRC_EVENT_RPL_WHOISCHANNELS;
int EIRC_EVENT_RPL_WHOWASUSER;
int EIRC_EVENT_RPL_ENDOFWHOWAS;
int EIRC_EVENT_RPL_LIST;
int EIRC_EVENT_RPL_LISTEND;
int EIRC_EVENT_RPL_UNIQOPIS;
int EIRC_EVENT_RPL_CHANNELMODEIS;
int EIRC_EVENT_RPL_NOTOPIC;
int EIRC_EVENT_RPL_TOPIC;
int EIRC_EVENT_RPL_INVITING;
int EIRC_EVENT_RPL_SUMMONING;
int EIRC_EVENT_RPL_INVITELIST;
int EIRC_EVENT_RPL_ENDOFINVITELIST;
int EIRC_EVENT_RPL_EXCEPTLIST;
int EIRC_EVENT_RPL_ENDOFEXCEPTLIST;
int EIRC_EVENT_RPL_VERSION;
int EIRC_EVENT_RPL_WHOREPLY;
int EIRC_EVENT_RPL_ENDOFWHO;
int EIRC_EVENT_RPL_NAMREPLY;
int EIRC_EVENT_RPL_ENDOFNAMES;
int EIRC_EVENT_RPL_LINKS;
int EIRC_EVENT_RPL_ENDOFLINKS;
int EIRC_EVENT_RPL_BANLIST;
int EIRC_EVENT_RPL_ENDOFBANLIST;
int EIRC_EVENT_RPL_INFO;
int EIRC_EVENT_RPL_ENDOFINFO;
int EIRC_EVENT_RPL_MOTDSTART;
int EIRC_EVENT_RPL_MOTD;
int EIRC_EVENT_RPL_ENDOFMOTD;
int EIRC_EVENT_RPL_YOUREOPER;
int EIRC_EVENT_RPL_REHASHING;
int EIRC_EVENT_RPL_YOURESERVICE;
int EIRC_EVENT_RPL_TIME;
int EIRC_EVENT_RPL_USERSSTART;
int EIRC_EVENT_RPL_USERS;
int EIRC_EVENT_RPL_ENDOFUSERS;
int EIRC_EVENT_RPL_NOUSERS;
int EIRC_EVENT_RPL_TRACELINK;
int EIRC_EVENT_RPL_TRACECONNECTING;
int EIRC_EVENT_RPL_TRACEHANDSHAKE;
int EIRC_EVENT_RPL_TRACEUNKNOWN;
int EIRC_EVENT_RPL_TRACEOPERATOR;
int EIRC_EVENT_RPL_TRACEUSER;
int EIRC_EVENT_RPL_TRACESERVER;
int EIRC_EVENT_RPL_TRACESERVICE;
int EIRC_EVENT_RPL_TRACENEWTYPE;
int EIRC_EVENT_RPL_TRACECLASS;
int EIRC_EVENT_RPL_TRACELOG;
int EIRC_EVENT_RPL_TRACEEND;
int EIRC_EVENT_RPL_STATSLINKINFO;
int EIRC_EVENT_RPL_STATSCOMMANDS;
int EIRC_EVENT_RPL_ENDOFSTATS;
int EIRC_EVENT_RPL_STATSUPTIME;
int EIRC_EVENT_RPL_STATSOLINE;
int EIRC_EVENT_RPL_UMODEIS;
int EIRC_EVENT_RPL_SERVLIST;
int EIRC_EVENT_RPL_SERVLISTEND;
int EIRC_EVENT_RPL_LUSERCLIENT;
int EIRC_EVENT_RPL_LUSEROP;
int EIRC_EVENT_RPL_LUSERUNKNOWN;
int EIRC_EVENT_RPL_LUSERCHANNELS;
int EIRC_EVENT_RPL_LUSERME;
int EIRC_EVENT_RPL_ADMINME;
int EIRC_EVENT_RPL_ADMINLOC1;
int EIRC_EVENT_RPL_ADMINLOC2;
int EIRC_EVENT_RPL_ADMINEMAIL;
int EIRC_EVENT_RPL_TRYAGAIN;
int EIRC_EVENT_ERR_NOSUCHNICK;
int EIRC_EVENT_ERR_NOSUCHSERVER;
int EIRC_EVENT_ERR_NOSUCHCHANNEL;
int EIRC_EVENT_ERR_CANNOTSENDTOCHAN;
int EIRC_EVENT_ERR_TOOMANYCHANNELS;
int EIRC_EVENT_ERR_WASNOSUCHNICK;
int EIRC_EVENT_ERR_TOOMANYTARGETS;
int EIRC_EVENT_ERR_NOSUCHSERVICE;
int EIRC_EVENT_ERR_NOORIGIN;
int EIRC_EVENT_ERR_NORECIPIENT;
int EIRC_EVENT_ERR_NOTEXTTOSEND;
int EIRC_EVENT_ERR_NOTOPLEVEL;
int EIRC_EVENT_ERR_WILDTOPLEVEL;
int EIRC_EVENT_ERR_BADMASK;
int EIRC_EVENT_ERR_UNKNOWNCOMMAND;
int EIRC_EVENT_ERR_NOMOTD;
int EIRC_EVENT_ERR_NOADMININFO;
int EIRC_EVENT_ERR_FILEERROR;
int EIRC_EVENT_ERR_NONICKNAMEGIVEN;
int EIRC_EVENT_ERR_ERRONEUSNICKNAME;
int EIRC_EVENT_ERR_NICKNAMEINUSE;
int EIRC_EVENT_ERR_NICKCOLLISION;
int EIRC_EVENT_ERR_UNAVAILRESOURCE;
int EIRC_EVENT_ERR_USERNOTINCHANNEL;
int EIRC_EVENT_ERR_NOTONCHANNEL;
int EIRC_EVENT_ERR_USERONCHANNEL;
int EIRC_EVENT_ERR_NOLOGIN;
int EIRC_EVENT_ERR_SUMMONDISABLED;
int EIRC_EVENT_ERR_USERSDISABLED;
int EIRC_EVENT_ERR_NOTREGISTERED;
int EIRC_EVENT_ERR_NEEDMOREPARAMS;
int EIRC_EVENT_ERR_ALREADYREGISTRED;
int EIRC_EVENT_ERR_NOPERMFORHOST;
int EIRC_EVENT_ERR_PASSWDMISMATCH;
int EIRC_EVENT_ERR_YOUREBANNEDCREEP;
int EIRC_EVENT_ERR_YOUWILLBEBANNED;
int EIRC_EVENT_ERR_KEYSET;
int EIRC_EVENT_ERR_CHANNELISFULL;
int EIRC_EVENT_ERR_UNKNOWNMODE;
int EIRC_EVENT_ERR_INVITEONLYCHAN;
int EIRC_EVENT_ERR_BANNEDFROMCHAN;
int EIRC_EVENT_ERR_BADCHANNELKEY;
int EIRC_EVENT_ERR_BADCHANMASK;
int EIRC_EVENT_ERR_NOCHANMODES;
int EIRC_EVENT_ERR_BANLISTFULL;
int EIRC_EVENT_ERR_NOPRIVILEGES;
int EIRC_EVENT_ERR_CHANOPRIVSNEEDED;
int EIRC_EVENT_ERR_CANTKILLSERVER;
int EIRC_EVENT_ERR_RESTRICTED;
int EIRC_EVENT_ERR_UNIQOPPRIVSNEEDED;
int EIRC_EVENT_ERR_NOOPERHOST;
int EIRC_EVENT_ERR_UMODEUNKNOWNFLAG;
int EIRC_EVENT_ERR_USERSDONTMATCH;

/**
 * Represent a complete irc session context
 */
struct _Eirc_Session
{
	void *session; /**< TODO: Doc this */
	void *context; /**< TODO: Doc this */
};

/**
 * Triggered when a client successfully connects.
 */
struct _Eirc_Event_Connect
{
	Eirc_Session *session;
};

/**
 * Triggered when someone has joined a channel.
 */
struct _Eirc_Event_Join
{
	Eirc_Session *session;
	char *origin;  /**< The person who joined */
	char *channel; /**< The channel name */
};

/**
 * Triggered when someone (including you) on a channel
 * has changed their name.
 */
struct _Eirc_Event_Nick
{
	Eirc_Session *session;
	char *origin;   /**< The person who changed their nick */
	char *new_nick; /**< The new nick. */
};

/**
 * Triggered when someone on a channel has disconnected
 */
struct _Eirc_Event_Quit
{
	Eirc_Session *session;
	char *origin; /**< The person who disconnected */
	char *reason; /**< OPTIONAL: The reason the person gave */
};

/**
 * Triggered when someone leaves a channel.
 */
struct _Eirc_Event_Part
{
	Eirc_Session *session;
	char *origin;  /**< Who left. Can be compared to your own
											nickname to determine if you were able
											to join a certain channel. */
	char *channel; /**< The channel the event occured on */
	char *reason;  /**< OPTIONAL: The reason the person gave */
};

/**
 * Triggered when a channel's mode has been changed.
 */
struct _Eirc_Event_Mode
{
	Eirc_Session *session;
	char *origin;       /**< Who changed the mode */
	char *channel;      /**< The channel the mode was changed on */
	char *changed_mode; /**< The changed mode, '+t', '-i', etc. */
	char *mode_arg;     /**< OPTIONAL: argument for mode, for example,
	                         the key for a '+k' mode */
};

/**
 * Triggered when a channel's topic is changed
 */
struct _Eirc_Event_Topic
{
	Eirc_Session *session;
	char *origin;  /**< Who changed the topic */
	char *channel; /**< The channel the topic was changed on */
	char *topic;   /**< OPTIONAL: the new topic */
};

/**
 * Triggered when someone in a channel has been kicked. This
 * might include yourself.
 */
struct _Eirc_Event_Kick
{
	Eirc_Session *session;
	char *origin;      /**< Person who did the kick */
	char *channel;     /**< The channel the kick occured in */
	char *nick_kicked; /**< OPTIONAL: nick of kicked person */
	char *kick_reason; /**> OPTIONAL: reason the kicker gave */
};

/**
 * Triggered when someone in a channel speaks.
 */
struct _Eirc_Event_Channel
{
	Eirc_Session *session;
	char *origin;  /**< The person who spoke */
	char *channel; /**< The channel the event was triggered in */
	char *message; /**< OPTIONAL: The message itself */
};

/**
 * Triggered when someone is sending you (and maybe others) 
 * a private message.
 */
struct _Eirc_Event_Privmsg
{
	Eirc_Session *session;
	char *origin;  /**< The person sending the message */
	char *nick;    /**< The nickname the message is sent to. You. */
	char *message; /**< OPTIONAL: The text of the message */
};

/**
 * Triggered when you are send a public or private notice.
 * According to RFC 1459, the only difference between NOTICE
 * and PRIVMSG is that you should NEVER automatically reply to NOTICE
 * messages. Unfortunately, this rule is frequently violated by IRC
 * servers itself - for example, NICKSERV messages require reply, and
 * are NOTICEs.
 */
struct _Eirc_Event_Notice
{
	Eirc_Session *session;
	char *origin;  /**< Person who send the notice */
	char *channel; /**< The channel the notice was send on */
	char *message; /**< OPTIONAL: The text of the message */
};

/**
 * Triggered when you are being invited (permitted entry) to
 * a certain channel with '+i' (invite only) mode set.
 */
struct _Eirc_Event_Invite
{
	Eirc_Session *session;
	char *origin;  /**< The person inviting you */
	char *nick;    /**< Your nick */
	char *channel; /**< The channel you are being invite into */
};

/**
 * Triggered when your user mode has been changed.
 */
struct _Eirc_Event_Umode
{
	Eirc_Session *session;
	char *origin;       /**< Person who changed the channel mode */
	char *changed_mode; /**< The changed mode, '+t', '-i', etc. */
};

/**
 * Triggered when the client receives a ctcp reply.
 */
struct _Eirc_Event_Ctcp_Rep
{
	Eirc_Session *session;
	char *origin;  /**< The person who sent the message */
	char *message; /**< The message with its arguments */
};

/**
 * Triggered when the client receives a ctcp request.
 */
struct _Eirc_Event_Ctcp_Req
{
	Eirc_Session *session;
	char *origin;  /**< The person who sent the message */
	char *message; /**< The message with its arguments */
};

/**
 * Triggered when the client receives the ctcp
 * action message (/me blah). These messages usually looks like:\n
 * [23:32:55] * Tim gonna sleep.
 */
struct _Eirc_Event_Ctcp_Action
{
	Eirc_Session *session;
	char *origin;  /**< The person who send the message */
	char *message; /**< The text of the action message */
};

/**
 * Triggered when the client receives a message of unknown
 * or unhandled type.
 */
struct _Eirc_Event_Unknown
{
	Eirc_Session *session;
};

/**
 * Triggered upon receipt of any numeric response from the
 * server. There is a lot of such responses, see the full list
 * in the rfcnumbers list.
 */
struct _Eirc_Event_Numeric
{
	/* TODO: implement */
	Eirc_Session *session;
	unsigned int event_number; /**< The number of the event */
};

/**
 * Triggered when someone requests a dcc chat from you.
 */
struct _Eirc_Event_Dcc_Chat_Req
{
	/* TODO: implement */
};

/**
 * Triggered when someone wants to send you a file via
 * a dcc send request
 */
struct _Eirc_Event_Dcc_Send_Req
{
	/* TODO: imeplement */	
};

/* Numeric events */

/**
 * Triggered when the server replies with a welcome
 */
struct _Eirc_Event_Rpl_Welcome
{
	char *nick;    /**< Your nickname */
	char *message; /**< The server's welcome message */
};

Eirc_Session *eirc_session_new(void);
void eirc_session_delete(Eirc_Session *s);

int eirc_main(Eirc_Session *s);

int eirc_connect(Eirc_Session *s, const char *server, int port, const char *server_password, const char *nick, const char *username, const char *realname);
void eirc_disconnect(Eirc_Session *s);
int eirc_is_connected(Eirc_Session *s);

int eirc_raw_data_send(Eirc_Session *s, const char *format, ...);

int eirc_command_quit(Eirc_Session *s, const char *reason);
int eirc_command_part(Eirc_Session *s, const char *channel);
int eirc_command_invite(Eirc_Session *s, const char *nick, const char *channel);
int eirc_command_names(Eirc_Session *s, const char *channel);
int eirc_command_list(Eirc_Session *s, const char *channel);
int eirc_command_topic(Eirc_Session *s, const char *channel, const char *topic);
int eirc_command_channel_mode(Eirc_Session *s, const char *channel, const char *mode);
int eirc_command_user_mode(Eirc_Session *s, const char *mode);
int eirc_command_nick(Eirc_Session *s, const char *newnick);
int eirc_command_whois(Eirc_Session *s, const char *nick);
int eirc_command_message(Eirc_Session *s, const char *nick_or_channel, const char *message);
int eirc_command_me(Eirc_Session *s, const char *nick_or_channel, const char *text);
int eirc_command_notice(Eirc_Session *s, const char *nick_or_channel, const char *text);
int eirc_command_kick(Eirc_Session *s, const char *nick, const char *channel, const char *reason);
int eirc_command_ctcp_request(Eirc_Session *s, const char *nick, const char *request);
int eirc_command_ctcp_reply(Eirc_Session *s, const char *nick, const char *reply);

void eirc_nick_strip(const char *origin, char *stripped_nick, int stripped_nick_size);
void eirc_host_strip(const char *origin, char *stripped_host, int stripped_host_size);

int eirc_channel_join(Eirc_Session *s, const char *channel, const char *key);

#endif
