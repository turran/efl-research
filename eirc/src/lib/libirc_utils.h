#ifndef _LIBIRC_UTILS_H
#define _LIBIRC_UTILS_H

void libirc_add_to_set (int fd, fd_set *set, int * maxfd);
void libirc_dump_data (const char * prefix, const char * buf, unsigned int length);
int libirc_findcrlf (const char * buf, int length);
int libirc_findcrorlf (char * buf, int length);
void libirc_event_ctcp_internal (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

#endif
