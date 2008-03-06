#ifndef ETCH_H_
#define ETCH_H_

/* for now */
#define EAPI


typedef struct _Etch Etch; /** */
typedef struct _Etch_Object Etch_Object; /** */

enum
{
	ETCH_UINT32,
	ETCH_INT32,
	ETCH_FLOAT,
	ETCH_DOUBLE,
	ETCH_ARGB,
};

enum
{
	ETCH_POSITION_X,
	ETCH_POSITION_Y,
	ETCH_SIZE_H,
	ETCH_SIZE_W,
	/* user defined properties will go from 0x30 to end */
	ETCH_USER 	= 0x30,
};

/* 
 * property format
 * [ type | data type ]
 *    16        16
 * maybe we should use smaller values? do we need properties in this way?
 */

#define ETCH_PROPERTY_TYPE_GET(x) (x >> 16)
#define ETCH_PROPERTY_DATATYPE_GET(x) (x & 0xffff)

enum
{
	ETCH_POSITION_X_UINT32 = (ETCH_POSITION_X << 16) | (ETCH_UINT32),
	ETCH_POSITION_Y_UINT32 = (ETCH_POSITION_Y << 16) | (ETCH_UINT32),
};

typedef void (*Etch_Property_Set)(void *odata, void *pdata);

/**
 * 
 */
typedef struct Etch_Object_Property
{
	int type; 
	Etch_Property_Set set;
} Etch_Object_Property;

/**
 * 
 * a class should define the properties, the name? and the function callbacks
 */
typedef struct _Etch_Object_Class
{
	Etch_Object_Property *props;
} Etch_Object_Class;

EAPI Etch * etch_new(void);
EAPI Etch_Object * etch_object_add(Etch *e, Etch_Object_Class *oc, const char *id, void *data);

#endif /*ETCH_H_*/
