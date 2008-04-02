#ifndef ETCH_H_
#define ETCH_H_

/**
 * @mainpage Etch
 * @section intro Introduction
 * Etch is a library that ... 
 * 
 * @file
 * @brief Etch API
 * @defgroup Etch_Group API
 * @{
 * 
 * TODO normalize the nomenclature for the properties on all the code:
 * property is composed by its type and its data type
 * TODO add the ability to make an object's property relative to other
 */

/* for now */
#define EAPI
/**
 * 
 */
typedef enum
{
	ETCH_UINT32,
	ETCH_INT32,
	ETCH_FLOAT,
	ETCH_DOUBLE,
	ETCH_ARGB,
	ETCH_DATATYPES,
} Etch_Property_Data;
/**
 * 
 */
typedef enum
{
	ETCH_POSITION_X,
	ETCH_POSITION_Y,
	ETCH_SIZE_H,
	ETCH_SIZE_W,
	/* user defined properties will go from 0x30 to end */
	ETCH_USER 	= 0x30,
} Etch_Property_Type;
/**
 * Useful macro to get the property type
 */
#define ETCH_PROPERTY_TYPE_GET(x) (x >> 16)
/**
 * Useful macro to get the property data type
 */
#define ETCH_PROPERTY_DATATYPE_GET(x) (x & 0xffff)
/** 
 * property format
 * [ type | data type ]
 *    16        16
 * maybe we should use smaller values? do we need properties in this way?
 */
typedef enum
{
	ETCH_POSITION_X_UINT32 = (ETCH_POSITION_X << 16) | (ETCH_UINT32),
	ETCH_POSITION_Y_UINT32 = (ETCH_POSITION_Y << 16) | (ETCH_UINT32),
} Etch_Property;

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
 * @defgroup Etch_Core_Group Core
 * @{
 */
typedef struct _Etch Etch; /**< Etch Opaque Handler */
EAPI Etch * etch_new(void);
EAPI void etch_free(Etch *e);
EAPI void etch_timer_fps_set(Etch *e, unsigned int fps);
EAPI unsigned int etch_timer_fps_get(Etch *e);
EAPI void etch_timer_tick(Etch *e);
EAPI int etch_timer_has_end(Etch *e);
EAPI void etch_timer_goto(Etch *e, unsigned long frame);
/** 
 * @}
 * @defgroup Etch_Animations_Group Animations
 * @{
 */
typedef struct _Etch_Animation Etch_Animation; /**< Animation Opaque Handler */
typedef struct _Etch_Animation_Keyframe Etch_Animation_Keyframe; /**< Animation Keyframe Opaque Handler */

typedef enum
{
	ETCH_ANIMATION_LINEAR,
	ETCH_ANIMATION_COSIN,
	ETCH_ANIMATION_QUADRATIC,
	ETCH_ANIMATION_CUBIC,
	ETCH_ANIMATION_TYPES
} Etch_Animation_Type;

EAPI Etch_Animation * etch_animation_new(int dtype);
EAPI void etch_animation_free(Etch_Animation *a);
EAPI Etch_Animation_Keyframe * etch_animation_keyframe_add(Etch_Animation *a);
EAPI void etch_animation_keyframe_del(Etch_Animation *a, Etch_Animation_Keyframe *m);
EAPI void etch_animation_keyframe_type_set(Etch_Animation_Keyframe *m, Etch_Animation_Type t);
EAPI Etch_Animation_Type etch_animation_keyframe_type_get(Etch_Animation_Keyframe *m);
EAPI void etch_animation_keyframe_time_set(Etch_Animation_Keyframe *m, unsigned long secs, unsigned long usecs);
EAPI void etch_animation_keyframe_value_set(Etch_Animation_Keyframe *m, ...);
EAPI void etch_animation_keyframe_value_get(Etch_Animation_Keyframe *m, ...);
/** 
 * @}
 * @defgroup Etch_Objects_Group Objects
 * @{
 */

typedef struct _Etch_Object Etch_Object; /**< Etch Object Opaque Handler */

/**
 * A class should define the properties, the name? and the function callbacks
 */
typedef struct _Etch_Object_Class
{
	Etch_Object_Property *props;
} Etch_Object_Class;

EAPI Etch_Object * etch_object_add(Etch *e, Etch_Object_Class *oc, const char *id, void *data);
EAPI void etch_object_delete(Etch_Object *o);
/* TODO This functions are really needed to be exported? if so, on the get we must pass
 * the void * to not make the user modify the value ? or maybe return a const void *?
 * for internal usage we can make it return the offset directly */
EAPI void etch_object_property_set(Etch_Object *eo, int prop, void *data);
EAPI void etch_object_property_get(Etch_Object *eo, int prop, void *data);
EAPI void etch_object_animation_set(Etch_Object *eo, int prop, Etch_Animation *a);
/** 
 * @}
 * @}
 */
#endif /*ETCH_H_*/
