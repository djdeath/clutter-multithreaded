#ifndef __CLUTTER_PROPERTY_PRIVATE_H__
#define __CLUTTER_PROPERTY_PRIVATE_H__

#include <glib-object.h>
#include <clutter/clutter-property.h>

#include "clutter-properties-private.h"

G_BEGIN_DECLS

#define CLUTTER_PROP(name) (CLUTTER_PROP_##name)

#define CLUTTER_PROPERTY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_PROPERTY, ClutterPropertyClass))
#define CLUTTER_IS_PROPERTY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_PROPERTY))
#define CLUTTER_PROPERTY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_PROPERTY, ClutterPropertyClass))

typedef struct _ClutterFloatProperty ClutterFloatProperty;
typedef struct _ClutterDoubleProperty ClutterDoubleProperty;
typedef struct _ClutterIntProperty ClutterIntProperty;
typedef struct _ClutterUintProperty ClutterUintProperty;
typedef struct _ClutterPointerProperty ClutterPointerProperty;

struct _ClutterValue
{
  ClutterProperty *property;

  union {
    gint	v_int;
    guint	v_uint;
    glong	v_long;
    gulong	v_ulong;
    gint64      v_int64;
    guint64     v_uint64;
    gfloat	v_float;
    gdouble	v_double;
    gpointer	v_pointer;
  } data;
};

typedef void (*ClutterValueInit) (ClutterValue *value);
typedef ClutterValue *(*ClutterValueCopy) (const ClutterValue *value);
typedef void (*ClutterValueFree) (ClutterValue *value);

struct _ClutterPropertyClass
{
  GTypeClass base_class;
};

struct _ClutterProperty
{
  GTypeInstance parent_instance;

  ClutterEntity *entity;

  guint prop_n;
  gboolean animatable;

  volatile int ref_count;

  /* TODO_LIONEL: move this to a const structure outside */
  ClutterValueInit init;
  ClutterValueCopy copy;
  ClutterValueFree free;

  ClutterValue *capture_value;
  ClutterValue *display_value;
};

struct _ClutterPropertyExp
{
  ClutterProperty parent;

  guint __padding[10]; /* Keep the padding large enough to embed
                          "subclasses" */
};

/**/

struct _ClutterFloatProperty
{
  ClutterProperty parent;

  gfloat min_value;
  gfloat max_value;
  gfloat default_value;
};

struct _ClutterDoubleProperty
{
  ClutterProperty parent;

  gdouble min_value;
  gdouble max_value;
  gdouble default_value;
};

struct _ClutterIntProperty
{
  ClutterProperty parent;

  gint min_value;
  gint max_value;
  gint default_value;
};

struct _ClutterUintProperty
{
  ClutterProperty parent;

  guint min_value;
  guint max_value;
  guint default_value;
};

struct _ClutterPointerProperty
{
  ClutterProperty parent;

  gpointer default_value;
};

void clutter_value_free (ClutterValue *value);

ClutterProperty *clutter_property_float_new (guint prop_n,
                                             gfloat min_value,
                                             gfloat max_value,
                                             gfloat default_value);
ClutterProperty *clutter_property_double_new (guint prop_n,
                                              gdouble min_value,
                                              gdouble max_value,
                                              gdouble default_value);
ClutterProperty *clutter_property_int_new (guint prop_n,
                                           gint min_value,
                                           gint max_value,
                                           gint default_value);
ClutterProperty *clutter_property_uint_new (guint prop_n,
                                            guint min_value,
                                            guint max_value,
                                            guint default_value);
ClutterProperty *clutter_property_pointer_new (guint prop_n,
                                               gpointer default_value,
                                               ClutterValueCopy copy_cb,
                                               ClutterValueFree free_cb);

ClutterValue *clutter_property_get_value_read (ClutterProperty *property);
ClutterValue *clutter_property_get_value_write (ClutterProperty *property);
ClutterValue *clutter_property_get_value_display (ClutterProperty *property);

G_END_DECLS

#endif /* __CLUTTER_PROPERTY_PRIVATE_H__ */
