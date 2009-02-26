/*
 * etk2_animation.c
 *
 *  Created on: 09-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Animation_Private *)((Etk_Animation *)(d))->private)
struct _Etk_Animation_Private
{
	char *name;
	Etk_Clock dur;
	Etk_Calc calc;
	Etk_Trigger begin;
	Etk_Trigger end;
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

static void _animation_coord_callback(const Etch_Data *curr, const Etch_Data *prev, void *data)
{
	Etk_Animation *a = data;
	Ekeko_Object *rel;
	Etk_Animation_Private *prv = PRIVATE(a);
	Etk_Coord coord;
	Ekeko_Value v;

#ifdef ETK_DEBUG
	printf("[Etk_Animation] Entering animation callback\n");
#endif
	rel = ekeko_object_parent_get((Ekeko_Object *)a);
	ekeko_object_property_value_get(rel, prv->name, &v);
#ifdef ETK_DEBUG
	printf("[Etk_Animation] Animation %p %p called %d -> %d\n", a, v.value.pointer_value, prev->data.i32, curr->data.i32);
#endif
	etk_coord_set(&coord, curr->data.i32, ((Etk_Coord *)v.value.pointer_value)->type);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set(rel, prv->name, &v);
}

static inline Etch_Animation_Type _calc_to_etch(Etk_Calc c)
{
	switch (c)
	{
		case ETK_CALC_LINEAR:
		return ETCH_ANIMATION_LINEAR;
		break;

		case ETK_CALC_DISCRETE:
		return ETCH_ANIMATION_DISCRETE;
		break;

		default:
		printf("ERRRRRRRRRRRRRRORRRRRRR\n");
		break;
	}
	return ETCH_ANIMATION_LINEAR;
}

static inline Etk_Document * _document_get(Ekeko_Object *o)
{
	Etk_Document *doc;

	/* canvas */
	if (ekeko_type_instance_is_of(o, ETK_TYPE_CANVAS))
	{
		doc = etk_canvas_document_get((Etk_Canvas *)o);
	}
	/* shape */
	else
	{
		Etk_Canvas *c;

		c = ekeko_object_parent_get(o);
		doc = etk_canvas_document_get(c);
	}
	return doc;
}

static inline void _value_set(Etk_Animation *a, Ekeko_Value *v, Etch_Animation_Keyframe *k)
{
	Etk_Animation_Private *prv = PRIVATE(a);

	if (v->type == ETK_PROPERTY_COORD)
	{
		Etk_Coord *c = v->value.pointer_value;

		etch_animation_keyframe_value_set(k, c->value);
		printf("[Etk_Animation] Setting value to %d\n", c->value);
	}
	else
	{
		printf("ERRRRRRRRRRRRRROOOOOOOOORRRRRR setting wrong animation value %d\n", v->type);
	}
}

static inline void _from_set(Etk_Animation *a)
{
	Etk_Animation_Private *prv = PRIVATE(a);

	if (!prv->from.k)
		prv->from.k = etch_animation_keyframe_add(prv->anim);
	_value_set(a, &prv->from.value, prv->from.k);
}

static inline void _to_set(Etk_Animation *a)
{
	Etk_Animation_Private *prv = PRIVATE(a);

	if (!prv->to.k)
		prv->to.k = etch_animation_keyframe_add(prv->anim);
	_value_set(a, &prv->to.value, prv->to.k);
}

static inline void _duration_set(Etk_Animation *a)
{
	Etk_Animation_Private *prv = PRIVATE(a);
	Etch_Animation_Keyframe *k;
	Eina_Iterator *it;
	Etk_Clock *clock;
	int count;
	int i = 0;

	count = etch_animation_keyframe_count(prv->anim);
	if (!count)
		return;
	clock = &prv->dur;
	printf("[Etk_Animation] Setting new clock to %d %d\n", clock->seconds, clock->micro);
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
}

static inline void _property_animate(Etk_Animation *a, Ekeko_Object *parent)
{
	Property *p;
	Etk_Document *doc;
	Etk_Animation_Private *prv = PRIVATE(a);
	Etch *etch;
	Ekeko_Value_Type vtype;
	Etch_Data_Type dtype;
	Etch_Animation_Callback cb = NULL;
	Etch_Animation_Type atype;

	/* get the property */
	p = ekeko_object_property_get(parent, prv->name);
	if (!p)
		return;
	doc = _document_get(parent);
	etch = etk_document_etch_get(doc);
	vtype = ekeko_property_value_type_get(p);
	printf("[Etk_Animation] Trying to animate property %s of type %d\n", prv->name, vtype);
	switch (vtype)
	{
		case PROPERTY_INT:
		dtype = ETCH_INT32;
		break;

		default:
		if (vtype == ETK_PROPERTY_COORD)
		{
			dtype = ETCH_UINT32;
			cb = _animation_coord_callback;
		}
		break;
	}
	/* TODO check if the animation already has an anim */
	prv->anim = etch_animation_add(etch, dtype, cb, a);
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

static void _trigger_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Animation_Private *prv = PRIVATE(data);

	printf("[Etk_Animation] Trigger callback\n");
	if (!etch_animation_enabled(prv->anim))
	{
		printf("[Etk_Animation] Begin trigger callback called!\n");
		//
		// get the current time
		// get the first keyframe
		// change every keyframe to curr offset
		/* Enable the animation */
		etch_animation_enable(prv->anim);
	}
	else if (prv->end.obj)
	{
		printf("[Etk_Animation] End trigger callback called!\n");
		etch_animation_disable(prv->anim);
	}
}

static void _prop_modify_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Ekeko_Object *parent;
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Animation *a = (Etk_Animation *)o;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	if (!strcmp(em->prop, "name"))
	{
		_property_animate(a, parent);
	}
	else if (!strcmp(em->prop, "dur"))
	{
		Etk_Animation_Private *prv = PRIVATE(o);

		if (!prv->anim)
			return;
		_duration_set((Etk_Animation *)o);
	}
	else if (!strcmp(em->prop, "repeat"))
	{
		Etk_Animation_Private *prv = PRIVATE(o);

		if (prv->anim)
			etch_animation_repeat_set(prv->anim, em->curr->value.int_value);
	}
	else if (!strcmp(em->prop, "calc"))
	{
		Eina_Iterator *it;
		Etk_Animation_Private *prv = PRIVATE(o);
		Etch_Animation_Type atype;
		Etch_Animation_Keyframe *k;

		if (!prv->anim)
			return;
		atype = _calc_to_etch(em->curr->value.int_value);
		/* for each keyframe set the calc mode to calc */
		it = etch_animation_iterator_get(prv->anim);
		while (eina_iterator_next(it, (void **)&k))
		{
			etch_animation_keyframe_type_set(k, atype);
		}
		eina_iterator_free(it);
	}
	else if (!strcmp(em->prop, "begin"))
	{
		Etk_Animation_Private *prv = PRIVATE(o);
		Etk_Trigger *t;

		/* TODO  we should have a default event that will trigger
		 * the animation start
		 */

		/* register an event listener to this event */
		t = em->curr->value.pointer_value;
		if (!prv->end.obj)
		{
			/* FIXME change that is not the same! */
			ekeko_event_listener_add(t->obj, t->event, _trigger_cb, EINA_FALSE, o);
		}
		if (!prv->anim)
			return;
		/* disable the animation */
		etch_animation_disable(prv->anim);
	}
	else if (!strcmp(em->prop, "end"))
	{
		Etk_Animation_Private *prv = PRIVATE(o);
		Etk_Trigger *t;

		/* register an event listener to this event */
		t = em->curr->value.pointer_value;
		if (!prv->begin.obj)
		{
			/* FIXME change that is not the same! */
			ekeko_event_listener_add(t->obj, t->event, _trigger_cb, EINA_FALSE, o);
		}
	}
	else if (!strcmp(em->prop, "from"))
	{
		Etk_Animation_Private *prv = PRIVATE(o);
		printf("%d\n", ((Etk_Coord *)em->curr->value.pointer_value)->value);
		if (!prv->anim)
			return;
		_from_set(a);
	}
	else if (!strcmp(em->prop, "to"))
	{
		Etk_Animation_Private *prv = PRIVATE(o);

		printf("%d\n", ((Etk_Coord *)em->curr->value.pointer_value)->value);
		if (!prv->anim)
			return;
		_to_set(a);
	}
}

static void _child_append_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Animation *a = (Etk_Animation *)o;
	Etk_Animation_Private *prv = PRIVATE(a);

	if (!prv->name)
		return;
	_property_animate(a, em->related);
}

static void _ctor(void *instance)
{
	Etk_Animation *a;
	Etk_Animation_Private *prv;

	a = (Etk_Animation*) instance;
	a->private = prv = ekeko_type_instance_private_get(etk_animation_type_get(), instance);
	/* default values */
	prv->repeat = 1;
	ekeko_event_listener_add((Ekeko_Object *)a, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)a, EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE, NULL);
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
EAPI Ekeko_Type *etk_animation_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_ANIMATION, sizeof(Etk_Animation),
				sizeof(Etk_Animation_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
		TYPE_PROP_SINGLE_ADD(type, "name", PROPERTY_STRING, OFFSET(Etk_Animation_Private, name));
		TYPE_PROP_SINGLE_ADD(type, "dur", ETK_PROPERTY_CLOCK, OFFSET(Etk_Animation_Private, dur));
		TYPE_PROP_SINGLE_ADD(type, "repeat", PROPERTY_INT, OFFSET(Etk_Animation_Private, repeat));
		TYPE_PROP_SINGLE_ADD(type, "calc", PROPERTY_INT, OFFSET(Etk_Animation_Private, calc));
		TYPE_PROP_SINGLE_ADD(type, "begin", ETK_PROPERTY_TRIGGER, OFFSET(Etk_Animation_Private, begin));
		TYPE_PROP_SINGLE_ADD(type, "from", PROPERTY_VALUE, OFFSET(Etk_Animation_Private, from.value));
		TYPE_PROP_SINGLE_ADD(type, "to", PROPERTY_VALUE, OFFSET(Etk_Animation_Private, to.value));
	}

	return type;
}

EAPI Etk_Animation * etk_animation_new(void)
{
	Etk_Animation *a;

	a = ekeko_type_instance_new(etk_animation_type_get());

	return a;
}

EAPI void etk_animation_property_set(Etk_Animation *a, const char *prop)
{
	Ekeko_Value val;

	ekeko_value_str_from(&val, prop);
	ekeko_object_property_value_set((Ekeko_Object *)a, "name", &val);
}

/*
 * Whenever the event is triggered the animation should start relative to the begin
 * attribute
 */
EAPI void etk_animation_begin_set(Etk_Animation *a, Ekeko_Object *o, char *event)
{
	Ekeko_Value val;
	Etk_Trigger trigger;

	trigger.event = event;
	trigger.obj = o;
	etk_value_trigger_from(&val, &trigger);
	ekeko_object_property_value_set((Ekeko_Object *)a, "begin", &val);
}

EAPI void etk_animation_end_set(Etk_Animation *a, Ekeko_Object *o, const char *event)
{
	Ekeko_Value val;
	Etk_Trigger trigger;

	trigger.event = (char *)event;
	trigger.obj = o;
	etk_value_trigger_from(&val, &trigger);
	ekeko_object_property_value_set((Ekeko_Object *)a, "end", &val);
}

EAPI void etk_animation_from_set(Etk_Animation *a, Ekeko_Value *from)
{
	ekeko_object_property_value_set((Ekeko_Object *)a, "from", from);
}

EAPI void etk_animation_to_set(Etk_Animation *a, Ekeko_Value *to)
{
	ekeko_object_property_value_set((Ekeko_Object *)a, "to", to);
}

/* Instead of setting from/to you can specify a list of values */
EAPI void etk_animation_values_set(Etk_Animation *a, ...)
{

}

EAPI void etk_animation_repeat_set(Etk_Animation *a, int repeat)
{
	Ekeko_Value val;

	ekeko_value_int_from(&val, repeat);
	ekeko_object_property_value_set((Ekeko_Object *)a, "repeat", &val);
}

EAPI void etk_animation_duration_set(Etk_Animation *a, Etk_Clock *dur)
{
	Ekeko_Value val;

	etk_value_clock_from(&val, dur);
	ekeko_object_property_value_set((Ekeko_Object *)a, "dur", &val);
}

EAPI void etk_animation_calc_set(Etk_Animation *a, Etk_Calc calc)
{
	Ekeko_Value val;

	ekeko_value_int_from(&val, calc);
	ekeko_object_property_value_set((Ekeko_Object *)a, "calc", &val);
}

EAPI void etk_animation_value_append(Etk_Animation *a, void *val)
{

}

EAPI void etk_animation_time_append(Etk_Animation *a, void *val)
{

}

EAPI void etk_animation_cp_append(Etk_Animation *a, void *val)
{

}
