#ifndef _EQU_COMMON_H
#define _EQU_COMMON_H

#define EQUANIME_NAME "equanime"
#define EQUANIME_PORT 0xea

/*
 * TODO
 * - Add a way to send messages from the server to the client that are *not* replies 
 */

typedef int Equ_Common_Id;

/* common data definitions */
typedef struct _Equ_Common_Host
{
	Equ_Common_Id id;
	char *name;
} Equ_Common_Host;

typedef struct _Equ_Common_Controller
{
	Equ_Common_Id id;
	char *name;
} Equ_Common_Controller;

typedef struct _Equ_Layer_Info
{
	Equ_Common_Id id;
	char *name;
} Equ_Layer_Info;

typedef struct _Equ_Layer_Caps
{
	uint32_t fmt_mask;
} Equ_Layer_Caps;

typedef struct _Equ_Layer_Status
{
	int32_t x;
	int32_t y;
	uint32_t w;
	uint32_t h;
	Equ_Format fmt;
} Equ_Layer_Status;

typedef enum
{
	EQU_DATA_HOST,
	EQU_DATA_CONTROLLER,
	EQU_DATA_LAYER,
	EQU_DATA_LAYER_CAPS,
	EQU_DATA_LAYER_STATUS,
	EQU_DATAS
} Equ_Data;

/* protocol structures */
typedef enum
{
	EQU_MSG_NO_REPLY,
	EQU_MSG_REPLY,
} Equ_Message_Reply;

/*
 * this errors are well known error number to retrieve the correct
 * error from the message, it is used only internally, for API errors
 * use TODO
 */
typedef enum
{
	EQU_ERR_NONE,
	EQU_ERR_NEXIST,  /* a request for some element that doesnt exist */
	EQU_ERR_CODEC,   /* bad encoding or decoding */
	EQU_ERR_TIMEOUT, /* timeout error */
	EQU_ERRORS,
} Equ_Error;

/* A reply should always be the same value as the message this reply replies to
 * plus one
 */
typedef enum _Equ_Message_Name
{
	EQU_MSG_NAME_HOSTS_GET,
	EQU_MSG_NAME_HOSTS_GETR,
	EQU_MSG_NAME_CONTROLLERS_GET,
	EQU_MSG_NAME_CONTROLLERS_GETR,
	EQU_MSG_NAME_LAYERS_GET,
	EQU_MSG_NAME_LAYERS_GETR,
	EQU_MSG_NAME_LAYER_CAPS_GET,
	EQU_MSG_NAME_LAYER_CAPS_GETR,
	EQU_MSG_NAME_LAYER_STATUS_GET,
	EQU_MSG_NAME_LAYER_STATUS_GETR,
	EQU_MSG_NAME_SURFACE_GET,
	EQU_MSG_NAME_SURFACE_GETR,
	EQU_MSG_NAME_SURFACE_PUT,
	EQU_MSG_NAME_SURFACE_PUTR,
	EQU_MSG_NAMES
} Equ_Message_Name;

typedef enum _Equ_Message_Type
{
	EQU_MSG_TYPE_HOSTS_GET         = ((EQU_MSG_NAME_HOSTS_GET << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_HOSTS_GETR        = ((EQU_MSG_NAME_HOSTS_GETR << 1) | EQU_MSG_NO_REPLY),
	EQU_MSG_TYPE_CONTROLLERS_GET   = ((EQU_MSG_NAME_CONTROLLERS_GET << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_CONTROLLERS_GETR  = ((EQU_MSG_NAME_CONTROLLERS_GETR << 1) | EQU_MSG_NO_REPLY),
	EQU_MSG_TYPE_LAYERS_GET        = ((EQU_MSG_NAME_LAYERS_GET << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_LAYERS_GETR       = ((EQU_MSG_NAME_LAYERS_GETR << 1) | EQU_MSG_NO_REPLY),
	EQU_MSG_TYPE_LAYER_CAPS_GET    = ((EQU_MSG_NAME_LAYER_CAPS_GET << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_LAYER_CAPS_GETR   = ((EQU_MSG_NAME_LAYER_CAPS_GETR << 1) | EQU_MSG_NO_REPLY),
	EQU_MSG_TYPE_LAYER_STATUS_GET  = ((EQU_MSG_NAME_LAYER_STATUS_GET << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_LAYER_STATUS_GETR = ((EQU_MSG_NAME_LAYER_STATUS_GETR << 1) | EQU_MSG_NO_REPLY),
	EQU_MSG_TYPE_SURFACE_GET       = ((EQU_MSG_NAME_SURFACE_GET << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_SURFACE_GETR      = ((EQU_MSG_NAME_SURFACE_GETR << 1) | EQU_MSG_NO_REPLY),
} Equ_Message_Type;

/*
 * A message is composed of:
 * +----+------+------+-----------------
 * | id | type | size |
 * +----+------+------+-----------------
 * <-----header------>.<-----body------>
 */

typedef struct _Equ_Message
{
	unsigned int id; /* id of the message */
	Equ_Message_Type type; /* type of message */
	unsigned int size; /* size of the body */
} Equ_Message;

typedef struct _Equ_Message_Hosts_Get
{
} Equ_Message_Hosts_Get;

typedef struct _Equ_Message_Controllers_Get
{
	Equ_Common_Id host_id;
} Equ_Message_Controllers_Get;

typedef struct _Equ_Message_Layers_Get
{
	Equ_Common_Id controller_id;
} Equ_Message_Layers_Get;

typedef struct _Equ_Message_Layer_Caps_Get
{
	Equ_Common_Id layer_id;
} Equ_Message_Layer_Caps_Get;

typedef struct _Equ_Message_Layer_Status_Get
{
	Equ_Common_Id layer_id;
} Equ_Message_Layer_Status_Get;

typedef struct _Equ_Message_Surface_Get
{
	Equ_Common_Id host_id;
	unsigned int w;
	unsigned int h;
	Equ_Format fmt;
	Equ_Surface_Type type;
} Equ_Message_Surface_Get;

typedef struct _Equ_Message_Surface_Put
{
	Equ_Common_Id src;
	Equ_Common_Id dst;
	/* TODO */
} Equ_Message_Surface_Put;

/*
 * A reply is composed of:
 * +----+-------+------+-----------------
 * | id | error | size |
 * +----+-------+------+-----------------
 * <------header------>.<-----body------>
 */

typedef struct _Equ_Reply
{
	unsigned int id; /* id of the message this reply replies to */
	unsigned int error; /* in case of any error set by the daemon */
	unsigned int size; /* size of the body */
} Equ_Reply;

typedef struct _Equ_Reply_Hosts_Get
{
	int hosts_count;
	Equ_Common_Host *hosts;
} Equ_Reply_Hosts_Get;

typedef struct _Equ_Reply_Controllers_Get
{
	int controllers_count;
	Equ_Common_Controller *controllers;
} Equ_Reply_Controllers_Get;

typedef struct _Equ_Reply_Layers_Get
{
	int layers_count;
	Equ_Layer_Info *layers;
} Equ_Reply_Layers_Get;

typedef struct _Equ_Reply_Layer_Caps_Get
{
	Equ_Layer_Caps caps;
} Equ_Reply_Layer_Caps_Get;

typedef struct _Equ_Reply_Layer_Status_Get
{
	Equ_Layer_Status status;
} Equ_Reply_Layer_Status_Get;

typedef struct _Equ_Reply_Surface_Get
{
	Equ_Common_Id id;
} Equ_Reply_Surface_Get;

typedef struct _Equ_Reply_Surface_Put
{
	/* TODO */
} Equ_Reply_Surface_Put;

static inline Equ_Message_Name equ_message_name_get(Equ_Message_Type t)
{
	return (t & ~1) >> 1;
}

static inline Eina_Bool equ_message_reply_has(Equ_Message_Type t)
{
	if (t & EQU_MSG_REPLY)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}

/**
 * Given a message type return the name of the reply. Note that the reply's
 * name is always the message plus one.
 */
static inline Eina_Bool equ_message_reply_name_get(Equ_Message_Type t, Equ_Message_Name *n)
{
	if (equ_message_reply_has(t) == EINA_FALSE)
		return EINA_FALSE;
	*n = equ_message_name_get(t) + 1;
	return EINA_TRUE;
}

void equ_message_init(void);
void equ_message_shutdown(void);

void * equ_message_encode(Equ_Message_Name name, const void *data, int *size);
void * equ_message_decode(Equ_Message_Name name, const void *data, int size);

#endif /*_EQU_COMMON_H*/
