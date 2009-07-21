/*
 * eon_animation.c
 *
 *  Created on: 09-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"

#define EON_ANIMATION_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Animation_Private *)((Eon_Animation *)(d))->private)
struct _Eon_Animation_Private
{
	char *name;
	Eon_Clock dur;
	Eon_Calc calc;
	Eon_Trigger begin;
	Eon_Trigger end;
	struct
	{
		Ekeko_Value value;
		Etch_Animation_Keyframe *k;
	} from, to;
	int repeat;

	Etch_Animation *anim;
	/*
	 * TODO
	 * keytimes = list
	 * values = list
	 * "from" and "to" are just a list of two values
	 * keysplines = list of the bezier control points
	 */
};

typedef struct _Eon_Animation_Callback_Container
{
	Eon_Animation *a;
	Eon_Animation_Callback cb;
} Eon_Animation_Callback_Container;

static inline Etch_Animation_Type _calc_to_etch(Eon_Calc c)
{
	switch (c)
	{
		case EON_CALC_LINEAR:
		return ETCH_ANIMATION_LINEAR;
		break;

		case EON_CALC_DISCRETE:
		return ETCH_ANIMATION_DISCRETE;
		break;

		default:
#if EON_ANIMATION_DEBUG
		printf("ERRRRRRRRRRRRRRORRRRRRR\n");
#endif
		break;
	}
	return ETCH_ANIMATION_LINEAR;
}

static inline Eon_Document * _document_get(Ekeko_Object *o)
{
	Eon_Document *doc;

	/* canvas */
	if (ekeko_type_instance_is_of(o, EON_TYPE_CANVAS))
	{
		doc = eon_canvas_document_get((Eon_Canvas *)o);
	}
	/* shape */
	else
	{
		Ekeko_Object *parent;

		parent = ekeko_object_parent_get(o);
		if (!parent) return NULL;
		doc = _document_get(parent);
	}
	return doc;
}

static void _etch_callback(const Etch_Data *curr, const Etch_Data *prev, void *data)
{
	Eon_Animation_Callback_Container *acc = data;
	Eon_Animation *a = acc->a;
	Ekeko_Object *rel;
	Eon_Animation_Private *prv = PRIVATE(a);

#if EON_ANIMATION_DEBUG
	printf("[Eon_Animation] Entering animation callback\n");
#endif
	rel = ekeko_object_parent_get((Ekeko_Object *)a);
	acc->cb(a, prv->name, rel, curr, prev);
}


static void _event_set(Eon_Event_Animation *ev, const Ekeko_Object *o, const char *type, Eon_Event_Animation_State state)
{
	ekeko_event_init((Ekeko_Event *)ev, type, o, EINA_FALSE);
	ev->state = state;
	ekeko_event_dispatch((Ekeko_Event *)ev);
}

static void _etch_start_cb(Etch_Animation *ea, void *data)
{
	Eon_Animation_Callback_Container *acc = data;
	Eon_Event_Animation ev;

	_event_set(&ev, (Ekeko_Object *)acc->a, EON_ANIMATION_START, EON_EVENT_ANIMATION_START);
}

static void _etch_stop_cb(Etch_Animation *ea, void *data)
{
	Eon_Animation_Callback_Container *acc = data;
	Eon_Event_Animation ev;

	_event_set(&ev, (Ekeko_Object *)acc->a, EON_ANIMATION_STOP, EON_EVENT_ANIMATION_STOP);
}

static inline void _value_set(Eon_Animation *a, Ekeko_Value *v, Etch_Animation_Keyframe *k)
{
	if (a->value_set)
		a->value_set(v, k);
}

static inline void _from_set(Eon_Animation *a)
{
	Eon_Animation_Private *prv = PRIVATE(a);

	if (!prv->from.k)
		prv->from.k = etch_animation_keyframe_add(prv->anim);
	_value_set(a, &prv->from.value, prv->from.k);
}

static inline void _to_set(Eon_Animation *a)
{
	Eon_Animation_Private *prv = PRIVATE(a);

	if (!prv->to.k)
		prv->to.k = etch_animation_keyframe_add(prv->anim);
	_value_set(a, &prv->to.value, prv->to.k);
}

static inline void _duration_set(Eon_Animation *a)
{
	Eon_Animation_Private *prv = PRIVATE(a);
	Etch_Animation_Keyframe *k;
	Eina_Iterator *it;
	Eon_Clock *clock;
	int count;
	int i = 0;

	count = etch_animation_keyframe_count(prv->anim);
	if (!count)
		return;
	clock = &prv->dur;
#if EON_ANIMATION_DEBUG
	printf("[Eon_Animation] Setting new clock to %d %d\n", clock->seconds, clock->micro);
#endif
#if 0
	/* for each keyframe set the new time */
	it = etch_animation_iterator_get(prv->anim);
	while (eina_iterator_next(it, (void **)&k))
	{
		/* FIXME this function changes the order of the list
		 * so it might not work. fix ETCH!
		 */
		etch_animation_keyframe_time_set(k, (clock->seconds / count) * i, clock->micro);
		i++;
	}
	eina_iterator_free(it);
#endif
	if (!prv->to.k)
		return;
	etch_animation_keyframe_time_set(prv->to.k, clock->seconds, clock->micro);
}

static inline void _property_animate(Eon_Animation *a, Ekeko_Object *parent)
{
	Ekeko_Property *p;
	Eon_Document *doc;
	Eon_Animation_Private *prv = PRIVATE(a);
	Etch *etch;
	Ekeko_Value_Type vtype;
	Etch_Data_Type dtype;
	Eon_Animation_Callback cb = NULL;
	Eon_Animation_Callback_Container *acc;
	Etch_Animation_Type atype;

	/* get the property */
	if (!prv->name)
		return;

	p = ekeko_object_property_get(parent, prv->name);
	if (!p)
		return;

	doc = _document_get(parent);
	if (!doc)
		return;

	etch = eon_document_etch_get(doc);
	vtype = ekeko_property_value_type_get(p);
#ifndef EON_ANIMATION_DEBUG
	printf("[Eon_Animation] Trying to animate property %s of type %d\n", prv->name, vtype);
#endif

	if (!a->callback_set)
		return;
	a->callback_set(vtype, &dtype, &cb);
	if (!cb)
		return;

	acc = malloc(sizeof(Eon_Animation_Callback_Container));
	acc->a = a;
	acc->cb = cb;
	/* TODO check if the animation already has an anim */
	prv->anim = etch_animation_add(etch, dtype, _etch_callback,
			_etch_start_cb, _etch_stop_cb, acc);
	atype = _calc_to_etch(prv->calc);
	etch_animation_repeat_set(prv->anim, prv->repeat);

	/* If we have some trigger event disable the animation for now */
	if (prv->begin.obj)
		etch_animation_disable(prv->anim);
	else
		etch_animation_enable(prv->anim);
	_from_set(a);
	_to_set(a);
	_duration_set(a);
}

static void _trigger_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Eon_Animation_Private *prv = PRIVATE(data);

	printf("[Eon_Animation] Trigger callback %p\n", prv->anim);

	if (!etch_animation_enabled(prv->anim))
	{
		Etch *e;
		unsigned long secs, usecs;

		printf("[Eon_Animation] Begin trigger callback called!\n");
		/* change every keyframe to curr offset */
		e = etch_animation_etch_get(prv->anim);
		etch_timer_get(e, &secs, &usecs);
		etch_animation_offset_add(prv->anim, secs, usecs);
#if 0
		etch_animation_keyframe_time_get(prv->to.k, &secs, &usecs);
		printf("ANIM TIME STOP %ld %ld\n", secs, usecs);
#endif
		/* Enable the animation */
		etch_animation_enable(prv->anim);
	}
	else if (prv->end.obj)
	{
		printf("[Eon_Animation] End trigger callback called!\n");
		etch_animation_disable(prv->anim);
	}
}

static void _prop_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Eon_Animation *a = (Eon_Animation *)o;
	Ekeko_Object *parent;

	if (!(parent = ekeko_object_parent_get(o)))
		return;
	_property_animate(a, parent);
}

static void _dur_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Object *parent;
	Eon_Animation_Private *prv = PRIVATE(o);

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	if (!prv->anim)
		return;
	_duration_set((Eon_Animation *)o);
}

static void _from_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Eon_Animation *a = (Eon_Animation *)o;
	Eon_Animation_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	if (!prv->anim)
		return;
	/* FIXME it is using the curent value! */
	_from_set(a);
}

static void _to_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Eon_Animation *a = (Eon_Animation *)o;
	Eon_Animation_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	if (!prv->anim)
		return;
	/* FIXME it is using the curent value! */
#if 0
	{
		Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
		Eon_Coord *c = em->curr->value.pointer_value;
		printf("TYPE = %d %d\n", em->curr->type, c->value);
		c = em->prev->value.pointer_value;
		printf("PREV = %d\n", c->value);
		c = prv->to.value.value.pointer_value;
		printf("IN = %d\n", c->value);
	}
#endif
	_to_set(a);
}

static void _begin_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Animation_Private *prv = PRIVATE(o);
	Eon_Trigger *t;
	Ekeko_Object *parent;

	/* TODO  we should have a default event that will trigger
	 * the animation start
	 */

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	/* register an event listener to this event */
	t = em->curr->value.pointer_value;
	printf("[Eon_Animation] Begin changed %p %s\n", t->obj, t->event);
	if (!prv->end.obj)
	{
		printf("[Eon_Animation] event listener!!!!!\n");
		/* FIXME change that is not the same! */
		ekeko_event_listener_add(t->obj, t->event, _trigger_cb, EINA_FALSE, o);
	}
	if (!prv->anim)
		return;
	/* disable the animation */
	etch_animation_disable(prv->anim);

}

static void _end_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Animation_Private *prv = PRIVATE(o);
	Eon_Trigger *t;
	Ekeko_Object *parent;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	/* register an event listener to this event */
	t = em->curr->value.pointer_value;
	printf("[Eon_Animation] End changed %p %s\n", t->obj, t->event);
	if (!prv->begin.obj)
	{
		/* FIXME change that is not the same! */
		ekeko_event_listener_add(t->obj, t->event, _trigger_cb, EINA_FALSE, o);
	}
}

static void _repeat_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Animation_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	if (prv->anim)
		etch_animation_repeat_set(prv->anim, em->curr->value.int_value);
}

static void _calc_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Ekeko_Object *parent;
	Eina_Iterator *it;
	Eon_Animation_Private *prv = PRIVATE(o);
	Etch_Animation_Type atype;
	Etch_Animation_Keyframe *k;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	if (!prv->anim)
		return;

	printf("[Eon_Animation] Calc mode changed %d\n", em->curr->value.int_value);
	atype = _calc_to_etch(em->curr->value.int_value);
	/* for each keyframe set the calc mode to calc */
	it = etch_animation_iterator_get(prv->anim);
	while (eina_iterator_next(it, (void **)&k))
	{
		etch_animation_keyframe_type_set(k, atype);
	}
	eina_iterator_free(it);
}

static void _child_append_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Animation *a = (Eon_Animation *)o;
	Eon_Animation_Private *prv = PRIVATE(a);

	if (!prv->name)
		return;
	_property_animate(a, em->related);
}

static void _ctor(void *instance)
{
	Eon_Animation *a;
	Eon_Animation_Private *prv;

	a = (Eon_Animation*) instance;
	a->private = prv = ekeko_type_instance_private_get(eon_animation_type_get(), instance);
	/* default values */
	prv->repeat = 1;
	ekeko_event_listener_add((Ekeko_Object *)a, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_END_CHANGED, _end_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_BEGIN_CHANGED, _begin_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_PROPERTY_CHANGED, _prop_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_DURATION_CHANGED, _dur_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_REPEAT_CHANGED, _repeat_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_FROM_CHANGED, _from_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_TO_CHANGED, _to_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EON_ANIMATION_CALC_CHANGED, _calc_change, EINA_FALSE, NULL);
}

static void _dtor(void *rect)
{

}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Property_Id EON_ANIMATION_END;
Ekeko_Property_Id EON_ANIMATION_BEGIN;
Ekeko_Property_Id EON_ANIMATION_PROPERTY;
Ekeko_Property_Id EON_ANIMATION_DURATION;
Ekeko_Property_Id EON_ANIMATION_REPEAT;
Ekeko_Property_Id EON_ANIMATION_FROM;
Ekeko_Property_Id EON_ANIMATION_TO;
Ekeko_Property_Id EON_ANIMATION_CALC;

EAPI Ekeko_Type *eon_animation_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_ANIMATION, sizeof(Eon_Animation),
				sizeof(Eon_Animation_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
		EON_ANIMATION_PROPERTY = TYPE_PROP_SINGLE_ADD(type, "name", PROPERTY_STRING, OFFSET(Eon_Animation_Private, name));
		EON_ANIMATION_DURATION= TYPE_PROP_SINGLE_ADD(type, "dur", EON_PROPERTY_CLOCK, OFFSET(Eon_Animation_Private, dur));
		EON_ANIMATION_REPEAT = TYPE_PROP_SINGLE_ADD(type, "repeat", PROPERTY_INT, OFFSET(Eon_Animation_Private, repeat));
		EON_ANIMATION_CALC = TYPE_PROP_SINGLE_ADD(type, "calc", PROPERTY_INT, OFFSET(Eon_Animation_Private, calc));
		EON_ANIMATION_BEGIN = TYPE_PROP_SINGLE_ADD(type, "begin", EON_PROPERTY_TRIGGER, OFFSET(Eon_Animation_Private, begin));
		EON_ANIMATION_FROM = TYPE_PROP_SINGLE_ADD(type, "from", PROPERTY_VALUE, OFFSET(Eon_Animation_Private, from.value));
		EON_ANIMATION_TO = TYPE_PROP_SINGLE_ADD(type, "to", PROPERTY_VALUE, OFFSET(Eon_Animation_Private, to.value));
	}

	return type;
}

EAPI void eon_animation_property_set(Eon_Animation *a, const char *prop)
{
	Ekeko_Value val;

	ekeko_value_str_from(&val, prop);
	ekeko_object_property_value_set((Ekeko_Object *)a, "name", &val);
}

/*
 * Whenever the event is triggered the animation should start relative to the begin
 * attribute
 */
EAPI void eon_animation_begin_set(Eon_Animation *a, Ekeko_Object *o, char *event)
{
	Ekeko_Value val;
	Eon_Trigger trigger;

	trigger.event = event;
	trigger.obj = o;
	eon_value_trigger_from(&val, &trigger);
	ekeko_object_property_value_set((Ekeko_Object *)a, "begin", &val);
}

EAPI void eon_animation_end_set(Eon_Animation *a, Ekeko_Object *o, const char *event)
{
	Ekeko_Value val;
	Eon_Trigger trigger;

	trigger.event = (char *)event;
	trigger.obj = o;
	eon_value_trigger_from(&val, &trigger);
	ekeko_object_property_value_set((Ekeko_Object *)a, "end", &val);
}

EAPI void eon_animation_from_set(Eon_Animation *a, Ekeko_Value *from)
{
	ekeko_object_property_value_set((Ekeko_Object *)a, "from", from);
}

EAPI void eon_animation_to_set(Eon_Animation *a, Ekeko_Value *to)
{
	ekeko_object_property_value_set((Ekeko_Object *)a, "to", to);
}

/* Instead of setting from/to you can specify a list of values */
EAPI void eon_animation_values_set(Eon_Animation *a, ...)
{

}

EAPI void eon_animation_repeat_set(Eon_Animation *a, int repeat)
{
	Ekeko_Value val;

	ekeko_value_int_from(&val, repeat);
	ekeko_object_property_value_set((Ekeko_Object *)a, "repeat", &val);
}

EAPI void eon_animation_duration_set(Eon_Animation *a, Eon_Clock *dur)
{
	Ekeko_Value val;

	eon_value_clock_from(&val, dur);
	ekeko_object_property_value_set((Ekeko_Object *)a, "dur", &val);
}

EAPI void eon_animation_calc_set(Eon_Animation *a, Eon_Calc calc)
{
	Ekeko_Value val;

	ekeko_value_int_from(&val, calc);
	ekeko_object_property_value_set((Ekeko_Object *)a, "calc", &val);
}

EAPI void eon_animation_value_append(Eon_Animation *a, void *val)
{

}

EAPI void eon_animation_time_append(Eon_Animation *a, void *val)
{

}

EAPI void eon_animation_cp_append(Eon_Animation *a, void *val)
{

}
