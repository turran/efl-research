#ifndef ESHM_PRIVATE_H_
#define ESHM_PRIVATE_H_

#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "Eet.h"
#include "Ecore.h"
#include "Ecore_Con.h"

#define ESHMD_NAME "eshmd"
#define ESHMD_PORT 0x1b

extern Ecore_Con_Server *eshm_server;
/* protocol structures */
typedef enum
{
	ESHM_MSG_NO_REPLY,
	ESHM_MSG_REPLY,
} Eshm_Message_Reply;

/* 
 * this errors are well known error number to retrieve the correct
 * error from the message, it is used only internally, for API errors
 * use TODO
 */
typedef enum
{
	ESHM_ERR_CODEC,
	ESHM_ERR_TIMEOUT,
	ESHM_ERRORS,
} Eshm_Error;

/* A reply should always be the same value as the message this reply replies to
 * plus one
 */
typedef enum
{
	ESHM_MSG_NAME_SEGMENT_NEW,
	ESHM_MSG_NAME_SEGMENT_NEWR,
	ESHM_MSG_NAME_SEGMENT_LOCK,
	ESHM_MSG_NAME_SEGMENT_UNLOCK,
	ESHM_MSG_NAME_SEGMENT_DELETE,
	ESHM_MSG_NAMES
} Eshm_Message_Name;

typedef enum
{
	ESHM_MSG_TYPE_SEGMENT_NEW    = ((ESHM_MSG_NAME_SEGMENT_NEW << 1) | ESHM_MSG_REPLY),
	ESHM_MSG_TYPE_SEGMENT_NEWR   = ((ESHM_MSG_NAME_SEGMENT_NEWR << 1) | ESHM_MSG_NO_REPLY),
	ESHM_MSG_TYPE_SEGMENT_LOCK   = ((ESHM_MSG_NAME_SEGMENT_LOCK << 1) | ESHM_MSG_REPLY),
	ESHM_MSG_TYPE_SEGMENT_UNLOCK = ((ESHM_MSG_NAME_SEGMENT_UNLOCK << 1) | ESHM_MSG_REPLY),
	ESHM_MSG_TYPE_SEGMENT_DELETE = ((ESHM_MSG_NAME_SEGMENT_DELETE << 1) | ESHM_MSG_NO_REPLY),
} Eshm_Message_Type;


/* 
 * A message is composed of:
 * +----+------+------+-----------------
 * | id | type | size | 
 * +----+------+------+-----------------
 * <-----header------>.<-----body------>
 */

typedef struct _Eshm_Message
{
	unsigned int id; /* id of the message */
	Eshm_Message_Type type; /* type of message */
	unsigned int size; /* size of the body */
} Eshm_Message;

typedef struct _Eshm_Message_Segment_New
{
	const char *id;
	unsigned int size;
} Eshm_Message_Segment_New;

/* 
 * A reply is composed of:
 * +----+-------+------+-----------------
 * | id | error | size | 
 * +----+-------+------+-----------------
 * <------header------>.<-----body------>
 */

typedef struct _Eshm_Reply
{
	unsigned int id; /* id of the message this reply replies to */
	unsigned int error; /* in case of any error set by the daemon */
	unsigned int size; /* size of the body */
} Eshm_Reply;

typedef struct _Eshm_Reply_Segment_New
{
	int shmid;
} Eshm_Reply_Segment_New;


/* eshmd_main.c */
Eshm_Error eshm_server_send(Eshm_Message *m, void *data, double timeout, void **rdata);

/* eshmd_message.c */
inline Eshm_Message_Name eshm_message_name_get(Eshm_Message_Type t);
inline Eina_Bool eshm_message_reply_has(Eshm_Message_Type t);
inline Eina_Bool eshm_message_reply_name_get(Eshm_Message_Type t, Eshm_Message_Name *n);
void eshm_message_init(void);
void eshm_message_shutdown(void);
void * eshm_message_encode(Eshm_Message_Name name, const void *data, int *size);
void * eshm_message_decode(Eshm_Message_Name name, const void *data, int size);
Eshm_Error eshm_message_server_send(Eshm_Message_Type type, void *data, double timeout, void **rdata);

#endif /*ESHM_PRIVATE_H_*/
