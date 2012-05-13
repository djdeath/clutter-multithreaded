#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "clutter-entity-private.h"
#include "clutter-private.h"
#include "clutter-property.h"
#include "clutter-property-private.h"
#include "clutter-properties-private.h"
#include "clutter-stage-private.h"

#include <gobject/gvaluecollector.h>

#if 0
# define DEBUG_ENTITY(args...) g_print (args)
#else
# define DEBUG_ENTITY(args...)
#endif

static void
value_property_init (GValue *value)
{
  value->data[0].v_pointer = NULL;
}

static void
value_property_free_value (GValue *value)
{
  if (value->data[0].v_pointer != NULL)
    clutter_property_unref (value->data[0].v_pointer);
}

static void
value_property_copy_value (const GValue *src,
                             GValue       *dst)
{
  if (src->data[0].v_pointer != NULL)
    dst->data[0].v_pointer = clutter_property_ref (src->data[0].v_pointer);
  else
    dst->data[0].v_pointer = NULL;
}

static gpointer
value_property_peek_pointer (const GValue *value)
{
  return value->data[0].v_pointer;
}

static gchar *
value_property_collect_value (GValue      *value,
                              guint        n_collect_values,
                              GTypeCValue *collect_values,
                              guint        collect_flags)
{
  ClutterProperty *property;

  property = collect_values[0].v_pointer;

  if (property == NULL)
    {
      value->data[0].v_pointer = NULL;
      return NULL;
    }

  if (property->parent_instance.g_class == NULL)
    return g_strconcat ("invalid unclassed ClutterProperty pointer for "
                        "value type '",
                        G_VALUE_TYPE_NAME (value),
                        "'",
                        NULL);

  value->data[0].v_pointer = clutter_property_ref (property);

  return NULL;
}

static gchar *
value_property_lcopy_value (const GValue *value,
                              guint         n_collect_values,
                              GTypeCValue  *collect_values,
                              guint         collect_flags)
{
  ClutterProperty **property_p = collect_values[0].v_pointer;

  if (property_p == NULL)
    return g_strconcat ("value location for '",
                        G_VALUE_TYPE_NAME (value),
                        "' passed as NULL",
                        NULL);

  if (value->data[0].v_pointer == NULL)
    *property_p = NULL;
  else if (collect_flags & G_VALUE_NOCOPY_CONTENTS)
    *property_p = value->data[0].v_pointer;
  else
    *property_p = clutter_property_ref (value->data[0].v_pointer);

  return NULL;
}

static void
clutter_property_class_base_init (ClutterPropertyClass *klass)
{
}

static void
clutter_property_class_base_finalize (ClutterPropertyClass *klass)
{
}

static void
clutter_property_class_init (ClutterPropertyClass *klass)
{
}

static void
clutter_property_init (ClutterProperty *self)
{
  self->ref_count = 1;
}

GType
clutter_property_get_type (void)
{
  static volatile gsize property_type_id__volatile = 0;

  if (g_once_init_enter (&property_type_id__volatile))
    {
      static const GTypeFundamentalInfo finfo = {
        (G_TYPE_FLAG_CLASSED |
         G_TYPE_FLAG_INSTANTIATABLE |
         G_TYPE_FLAG_DERIVABLE |
         G_TYPE_FLAG_DEEP_DERIVABLE),
      };

      static const GTypeValueTable value_table = {
        value_property_init,
        value_property_free_value,
        value_property_copy_value,
        value_property_peek_pointer,
        "p",
        value_property_collect_value,
        "p",
        value_property_lcopy_value,
      };

      const GTypeInfo node_info = {
        sizeof (ClutterPropertyClass),

        (GBaseInitFunc) clutter_property_class_base_init,
        (GBaseFinalizeFunc) clutter_property_class_base_finalize,
        (GClassInitFunc) clutter_property_class_init,
        (GClassFinalizeFunc) NULL,
        NULL,

        sizeof (struct _ClutterPropertyExp),
        0,
        (GInstanceInitFunc) clutter_property_init,

        &value_table,
      };

      GType property_type_id =
        g_type_register_fundamental (g_type_fundamental_next (),
                                     I_("ClutterProperty"),
                                     &node_info, &finfo,
                                     0/* G_TYPE_FLAG_ABSTRACT */);

      g_once_init_leave (&property_type_id__volatile, property_type_id);
    }

  return property_type_id__volatile;
}

/**
 * clutter_property_ref:
 * @property: a #ClutterProperty
 *
 * Acquires a reference on @property.
 *
 * Return value: (transfer full): the #ClutterProperty
 *
 * Since: 1.10
 */
ClutterProperty *
clutter_property_ref (ClutterProperty *property)
{
  g_return_val_if_fail (CLUTTER_IS_PROPERTY (property), NULL);

  g_atomic_int_inc (&property->ref_count);

  return property;
}

/**
 * clutter_property_unref:
 * @property: a #ClutterProperty
 *
 * Releases a reference on @property.
 *
 * Since: 1.10
 */
void
clutter_property_unref (ClutterProperty *property)
{
  g_return_if_fail (CLUTTER_IS_PROPERTY (property));

  if (g_atomic_int_dec_and_test (&property->ref_count))
    {
      /* TODO_LIONEL: free values */
      g_type_free_instance ((GTypeInstance *) property);
    }
}

/**/

static ClutterValue *
clutter_value_new (ClutterProperty *property)
{
  ClutterValue *value = g_slice_new0 (ClutterValue);

  value->property = property;
  DEBUG_ENTITY ("new value %p\n", value);

  return value;
}

void
clutter_value_free (ClutterValue *value)
{
  DEBUG_ENTITY ("free value %p\n", value);
  value->property->free (value);
  g_slice_free (ClutterValue, value);
}

static void
_value_float_init (ClutterValue *value)
{
  ClutterFloatProperty *prop = (ClutterFloatProperty *) value->property;

  value->data.v_float = prop->default_value;
}

static void
_value_double_init (ClutterValue *value)
{
  ClutterDoubleProperty *prop = (ClutterDoubleProperty *) value->property;

  value->data.v_double = prop->default_value;
}

static void
_value_int_init (ClutterValue *value)
{
  ClutterIntProperty *prop = (ClutterIntProperty *) value->property;

  value->data.v_int = prop->default_value;
}

static void
_value_uint_init (ClutterValue *value)
{
  ClutterUintProperty *prop = (ClutterUintProperty *) value->property;

  value->data.v_uint = prop->default_value;
}

static void
_value_pointer_init (ClutterValue *value)
{
  ClutterPointerProperty *prop = (ClutterPointerProperty *) value->property;

  value->data.v_pointer = prop->default_value;
}

static ClutterValue *
_value_copy (const ClutterValue *value)
{
  ClutterValue *ret = g_slice_dup (ClutterValue, value);
  DEBUG_ENTITY ("copy value %p (from)=%p (prop)=%p/%p\n", ret, value, value->property, ret->property);
  return ret;
}

static void
_value_free (ClutterValue *value)
{
  /* Nothing to free for base types */
}

static void
clutter_property_init_display_value (ClutterProperty *property)
{
  property->display_value = clutter_value_new (property);
  property->init (property->display_value);
  property->capture_value = property->display_value;
}

static ClutterProperty *
clutter_property_new (guint prop_n)
{
  ClutterProperty *prop =
    (ClutterProperty *) g_type_create_instance (CLUTTER_TYPE_PROPERTY);

  prop->prop_n = prop_n;
  DEBUG_ENTITY ("property new %p\n", prop);

  return prop;
}

ClutterProperty *
clutter_property_float_new (guint prop_n,
                            gfloat min_value,
                            gfloat max_value,
                            gfloat default_value)
{
  ClutterFloatProperty *prop;

  g_return_val_if_fail (prop_n < CLUTTER_PROP_LAST_PROPERTY, NULL);
  g_return_val_if_fail (min_value <= max_value, NULL);
  g_return_val_if_fail (default_value <= max_value &&
                        default_value >= min_value, NULL);

  prop = (ClutterFloatProperty *) clutter_property_new (prop_n);
  prop->parent.animatable = TRUE;
  prop->parent.init = _value_float_init;
  prop->parent.copy = _value_copy;
  prop->parent.free = _value_free;

  prop->min_value = min_value;
  prop->max_value = max_value;
  prop->default_value = default_value;

  clutter_property_init_display_value ((ClutterProperty *) prop);

  return (ClutterProperty *) prop;
}

ClutterProperty *
clutter_property_double_new (guint prop_n,
                             gdouble min_value,
                             gdouble max_value,
                             gdouble default_value)
{
  ClutterDoubleProperty *prop;

  g_return_val_if_fail (prop_n < CLUTTER_PROP_LAST_PROPERTY, NULL);
  g_return_val_if_fail (min_value <= max_value, NULL);
  g_return_val_if_fail (default_value <= max_value &&
                        default_value >= min_value, NULL);

  prop = (ClutterDoubleProperty *) clutter_property_new (prop_n);
  prop->parent.animatable = TRUE;
  prop->parent.init = _value_double_init;
  prop->parent.copy = _value_copy;
  prop->parent.free = _value_free;

  prop->min_value = min_value;
  prop->max_value = max_value;
  prop->default_value = default_value;

  clutter_property_init_display_value ((ClutterProperty *) prop);

  return (ClutterProperty *) prop;
}

ClutterProperty *
clutter_property_int_new (guint prop_n,
                          gint min_value,
                          gint max_value,
                          gint default_value)
{
  ClutterIntProperty *prop;

  g_return_val_if_fail (prop_n < CLUTTER_PROP_LAST_PROPERTY, NULL);
  g_return_val_if_fail (min_value <= max_value, NULL);
  g_return_val_if_fail (default_value <= max_value &&
                        default_value >= min_value, NULL);

  prop = (ClutterIntProperty *) clutter_property_new (prop_n);
  prop->parent.animatable = TRUE;
  prop->parent.init = _value_int_init;
  prop->parent.copy = _value_copy;
  prop->parent.free = _value_free;

  prop->min_value = min_value;
  prop->max_value = max_value;
  prop->default_value = default_value;

  clutter_property_init_display_value ((ClutterProperty *) prop);

  return (ClutterProperty *) prop;
}

ClutterProperty *
clutter_property_uint_new (guint prop_n,
                           guint min_value,
                           guint max_value,
                           guint default_value)
{
  ClutterUintProperty *prop;

  g_return_val_if_fail (prop_n < CLUTTER_PROP_LAST_PROPERTY, NULL);
  g_return_val_if_fail (min_value <= max_value, NULL);
  g_return_val_if_fail (default_value <= max_value &&
                        default_value >= min_value, NULL);

  prop = (ClutterUintProperty *) clutter_property_new (prop_n);
  prop->parent.animatable = TRUE;
  prop->parent.init = _value_uint_init;
  prop->parent.copy = _value_copy;
  prop->parent.free = _value_free;

  prop->min_value = min_value;
  prop->max_value = max_value;
  prop->default_value = default_value;

  clutter_property_init_display_value ((ClutterProperty *) prop);

  return (ClutterProperty *) prop;
}

ClutterProperty *
clutter_property_pointer_new (guint prop_n,
                              gpointer default_value,
                              ClutterValueCopy copy_cb,
                              ClutterValueFree free_cb)
{
  ClutterPointerProperty *prop;

  g_return_val_if_fail (prop_n < CLUTTER_PROP_LAST_PROPERTY, NULL);

  prop = (ClutterPointerProperty *) clutter_property_new (prop_n);
  prop->parent.animatable = FALSE;
  prop->parent.init = _value_pointer_init;
  prop->parent.copy = copy_cb != NULL ? copy_cb : _value_copy;
  prop->parent.free = free_cb != NULL ? free_cb : _value_free;

  prop->default_value = default_value;

  clutter_property_init_display_value ((ClutterProperty *) prop);

  return (ClutterProperty *) prop;
}

ClutterValue *
clutter_property_get_value_read (ClutterProperty *property)
{
  g_return_val_if_fail (CLUTTER_IS_PROPERTY (property), NULL);

  return property->capture_value;
}

ClutterValue *
clutter_property_get_value_write (ClutterProperty *property)
{
  ClutterValue *value;

  g_return_val_if_fail (CLUTTER_IS_PROPERTY (property), NULL);

  g_assert (property->entity != NULL);

  if (!property->entity->stage)
    {
      /* Special case when the entity hasn't been set on the stage
         yet. */
      if (property->capture_value == property->display_value)
        property->capture_value = property->copy (property->capture_value);
      value = property->capture_value;
    }
  else
    {
      value = property->copy (property->capture_value);
      DEBUG_ENTITY ("\tprop value=%p (prop)=%p (display)=%p\n", value, value->property, value->property->display_value);
      _clutter_stage_queue_value (property->entity->stage, value);
      DEBUG_ENTITY ("\t2prop value=%p (prop)=%p (display)=%p\n", value, value->property, value->property->display_value);
    }

  return value;
}

ClutterValue *
clutter_property_get_value_display (ClutterProperty *property)
{
  /* g_return_val_if_fail (CLUTTER_IS_PROPERTY (property), NULL); */

  /* This should never fail as it should be initialized when creating
     the property. */
  g_assert (property->display_value != NULL);

  return property->display_value;
}

/**
 * clutter_property_get_name:
 * @property: A #ClutterProperty
 *
 * Retrieves the name of @property.
 *
 * Return value: the name of the property, or %NULL. The returned
 *   string is owned by the property and should not be modified or
 *   freed.
 */
const gchar *
clutter_property_get_name (ClutterProperty *property)
{
  g_return_val_if_fail (CLUTTER_IS_PROPERTY (property), NULL);

  return _clutter_property_names[property->prop_n];
}

/**
 * clutter_property_get_entity:
 * @property: A #ClutterProperty
 *
 * Retrieves the #ClutterEntity associated with @property.
 *
 * Return value: (transfer none): the #ClutterEntity associated to
 *   @property, or %NULL. The returned entity is owned by the property
 *   and should not be unreffed.
 */
ClutterEntity *
clutter_property_get_entity (ClutterProperty *property)
{
  g_return_val_if_fail (CLUTTER_IS_PROPERTY (property), NULL);

  return property->entity;
}
