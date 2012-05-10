/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2009  Intel Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:
 *   Emmanuele Bassi <ebassi@linux.intel.com>
 */

/**
 * SECTION:clutter-animatable
 * @short_description: Interface for animatable classes
 *
 * #ClutterAnimatable is an interface that allows a #GObject class
 * to control how a #ClutterAnimation will animate a property.
 *
 * Each #ClutterAnimatable should implement the
 * <function>animate_property</function> virtual function of the interface
 * to compute the animation state between two values of an interval depending
 * on a progress factor, expressed as a floating point value.
 *
 * If a #ClutterAnimatable is animated by a #ClutterAnimation
 * instance, the #ClutterAnimation will call
 * clutter_animatable_animate_property() passing the name of the
 * currently animated property; the initial and final values of
 * the animation interval; the progress factor. The #ClutterAnimatable
 * implementation should return the computed value for the animated
 * property.
 *
 * #ClutterAnimatable is available since Clutter 1.0
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gobject/gvaluecollector.h>

#include "clutter-animatable.h"
#include "clutter-animatable-private.h"
#include "clutter-interval.h"
#include "clutter-debug.h"
#include "clutter-private.h"

typedef ClutterAnimatableIface  ClutterAnimatableInterface;
G_DEFINE_INTERFACE (ClutterAnimatable, clutter_animatable, G_TYPE_OBJECT);

static GParamSpecPool *animatable_properties_spec_pool = NULL;
static GQuark          quark_real_owner                = 0;

static void
clutter_animatable_default_init (ClutterAnimatableInterface *iface)
{
  quark_real_owner =
    g_quark_from_static_string ("clutter-animatable-real-owner-quark");
  animatable_properties_spec_pool = g_param_spec_pool_new (FALSE);
}

/* TODO_LIONEL: write documentation */
void
clutter_animatable_install_property (GType       owner_type,
                                     GParamSpec *pspec)
{
  g_return_if_fail (owner_type != G_TYPE_INVALID);
  g_return_if_fail (G_IS_PARAM_SPEC (pspec));

  if (g_param_spec_pool_lookup (animatable_properties_spec_pool,
                                pspec->name,
                                owner_type,
                                FALSE))
    {
      g_warning ("%s: class `%s' already contains a style property named `%s'",
                 G_STRLOC,
                 g_type_name (owner_type),
                 pspec->name);
      return;
    }

  g_param_spec_ref_sink (pspec);
  g_param_spec_set_qdata_full (pspec, quark_real_owner,
                               g_strdup (g_type_name (owner_type)),
                               g_free);

  g_param_spec_pool_insert (animatable_properties_spec_pool,
                            pspec,
                            owner_type);
}

void
clutter_animatable_install_properties (GType       owner_type,
                                       guint       n_props,
                                       GParamSpec *pspec[])
{
  gint i;

  g_return_if_fail (owner_type != G_TYPE_INVALID);
  g_return_if_fail (n_props > 1);
  g_return_if_fail (pspec != NULL);

  for (i = 1; i < n_props; i++)
    {
      clutter_animatable_install_property (owner_type, pspec[i]);
      pspec[i]->param_id = i;
    }
}

/**
 * clutter_animatable_find_property:
 * @animatable: a #ClutterAnimatable
 * @property_name: the name of the animatable property to find
 *
 * Finds the #GParamSpec for @property_name
 *
 * Return value: (transfer none): The #GParamSpec for the given property
 *   or %NULL
 *
 * Since: 1.4
 */
GParamSpec *
clutter_animatable_find_property (ClutterAnimatable *animatable,
                                  const gchar       *property_name)
{
  ClutterAnimatableIface *iface;
  GParamSpec *pspec = NULL;

  g_return_val_if_fail (CLUTTER_IS_ANIMATABLE (animatable), NULL);
  g_return_val_if_fail (property_name != NULL, NULL);

  CLUTTER_NOTE (ANIMATION, "Looking for property '%s'", property_name);

  iface = CLUTTER_ANIMATABLE_GET_IFACE (animatable);
  if (iface->find_property != NULL)
    if ((pspec = iface->find_property (animatable, property_name)) != NULL)
      return pspec;

  return (GParamSpec *) g_param_spec_pool_lookup (animatable_properties_spec_pool,
                                                  property_name,
                                                  G_OBJECT_TYPE (animatable),
                                                  TRUE);
}

/**
 * clutter_animatable_get_initial_state:
 * @animatable: a #ClutterAnimatable
 * @property_name: the name of the animatable property to retrieve
 * @value: a #GValue initialized to the type of the property to retrieve
 *
 * Retrieves the current state of @property_name and sets @value with it
 *
 * Since: 1.4
 */
void
clutter_animatable_get_initial_state (ClutterAnimatable *animatable,
                                      const gchar       *property_name,
                                      GValue            *value)
{
  ClutterAnimatableIface *iface;

  g_return_if_fail (CLUTTER_IS_ANIMATABLE (animatable));
  g_return_if_fail (property_name != NULL);

  CLUTTER_NOTE (ANIMATION, "Getting initial state of '%s'", property_name);

  iface = CLUTTER_ANIMATABLE_GET_IFACE (animatable);
  if (iface->get_initial_state != NULL)
    iface->get_initial_state (animatable, property_name, value);
  else
    g_object_get_property (G_OBJECT (animatable), property_name, value);
}

/**
 * clutter_animatable_set_final_state:
 * @animatable: a #ClutterAnimatable
 * @property_name: the name of the animatable property to set
 * @value: the value of the animatable property to set
 *
 * Sets the current state of @property_name to @value
 *
 * Since: 1.4
 */
void
clutter_animatable_set_final_state (ClutterAnimatable *animatable,
                                    const gchar       *property_name,
                                    const GValue      *value)
{
  ClutterAnimatableIface *iface;

  g_return_if_fail (CLUTTER_IS_ANIMATABLE (animatable));
  g_return_if_fail (property_name != NULL);

  CLUTTER_NOTE (ANIMATION, "Setting state of property '%s'", property_name);

  iface = CLUTTER_ANIMATABLE_GET_IFACE (animatable);
  if (iface->set_final_state != NULL)
    iface->set_final_state (animatable, property_name, value);
  else
    g_object_set_property (G_OBJECT (animatable), property_name, value);
}

/**
 * clutter_animatable_interpolate_value:
 * @animatable: a #ClutterAnimatable
 * @property_name: the name of the property to interpolate
 * @interval: a #ClutterInterval with the animation range
 * @progress: the progress to use to interpolate between the
 *   initial and final values of the @interval
 * @value: (out): return location for an initialized #GValue
 *   using the same type of the @interval
 *
 * Asks a #ClutterAnimatable implementation to interpolate a
 * a named property between the initial and final values of
 * a #ClutterInterval, using @progress as the interpolation
 * value, and store the result inside @value.
 *
 * This function should be used for every property animation
 * involving #ClutterAnimatable<!-- -->s.
 *
 * This function replaces clutter_animatable_animate_property().
 *
 * Return value: %TRUE if the interpolation was successful,
 *   and %FALSE otherwise
 *
 * Since: 1.8
 */
gboolean
clutter_animatable_interpolate_value (ClutterAnimatable *animatable,
                                      const gchar       *property_name,
                                      ClutterInterval   *interval,
                                      gdouble            progress,
                                      GValue            *value)
{
  ClutterAnimatableIface *iface;

  g_return_val_if_fail (CLUTTER_IS_ANIMATABLE (animatable), FALSE);
  g_return_val_if_fail (property_name != NULL, FALSE);
  g_return_val_if_fail (CLUTTER_IS_INTERVAL (interval), FALSE);
  g_return_val_if_fail (value != NULL, FALSE);

  CLUTTER_NOTE (ANIMATION, "Interpolating '%s' (progress: %.3f)",
                property_name,
                progress);

  iface = CLUTTER_ANIMATABLE_GET_IFACE (animatable);
  if (iface->interpolate_value != NULL)
    {
      return iface->interpolate_value (animatable, property_name,
                                       interval,
                                       progress,
                                       value);
    }
  else
    return clutter_interval_compute_value (interval, progress, value);
}

/* Animatable types */

static void
value_init_pointer (GValue *value)
{
  value->data[0].v_pointer = NULL;
}

static gpointer
value_peek_pointer (const GValue *value)
{
  return value->data[0].v_pointer;
}

static void
value_copy_int (const GValue *src_value,
                GValue *dest_value)
{
  gint *dst, *src;

  dst = dest_value->data[0].v_pointer;
  src = src_value->data[0].v_pointer;

  *dst = *src;
}

static gchar*
value_collect_int (GValue *value,
                   guint n_collect_values,
                   GTypeCValue *collect_values,
                   guint collect_flags)
{
  gint *dst, *src;

  dst = value->data[0].v_pointer;
  src = collect_values[0].v_pointer;

  *dst = *src;

  return NULL;
}

static gchar*
value_lcopy_int (const GValue *value,
                 guint n_collect_values,
                 GTypeCValue *collect_values,
                 guint collect_flags)
{
  gint *dst = collect_values[0].v_pointer;
  gint *src = value->data[0].v_pointer;

  if (!dst)
    return g_strdup_printf ("value location for `%s' passed as NULL",
                            G_VALUE_TYPE_NAME (value));

  *dst = *src;

  return NULL;
}

static void
value_copy_float (const GValue *src_value,
                  GValue *dest_value)
{
  gfloat *dst, *src;

  dst = dest_value->data[0].v_pointer;
  src = src_value->data[0].v_pointer;

  *dst = *src;
}

static gchar*
value_collect_float (GValue *value,
                     guint n_collect_values,
                     GTypeCValue *collect_values,
                     guint collect_flags)
{
  gfloat *dst, *src;

  dst = value->data[0].v_pointer;
  src = collect_values[0].v_pointer;

  *dst = *src;

  return NULL;
}

static gchar*
value_lcopy_float (const GValue *value,
                   guint n_collect_values,
                   GTypeCValue *collect_values,
                   guint collect_flags)
{
  gfloat *float_p = collect_values[0].v_pointer;

  if (!float_p)
    return g_strdup_printf ("value location for `%s' passed as NULL", G_VALUE_TYPE_NAME (value));

  *float_p = value->data[0].v_float;

  return NULL;
}

static void
value_copy_double (const GValue *src_value,
                   GValue *dest_value)
{
  dest_value->data[0].v_double = src_value->data[0].v_double;
}

static gchar*
value_collect_double (GValue *value,
                      guint n_collect_values,
                      GTypeCValue *collect_values,
                      guint collect_flags)
{
  value->data[0].v_double = collect_values[0].v_double;

  return NULL;
}

static gchar*
value_lcopy_double (const GValue *value,
                    guint n_collect_values,
                    GTypeCValue  *collect_values,
                    guint collect_flags)
{
  gdouble *double_p = collect_values[0].v_pointer;

  if (!double_p)
    return g_strdup_printf ("value location for `%s' passed as NULL", G_VALUE_TYPE_NAME (value));

  *double_p = value->data[0].v_double;

  return NULL;
}


void
_clutter_animatable_value_types_init (void)
{
  GTypeInfo info = {
    0,                           /* class_size */
    NULL,                        /* base_init */
    NULL,                        /* base_destroy */
    NULL,                        /* class_init */
    NULL,                        /* class_destroy */
    NULL,                        /* class_data */
    0,                           /* instance_size */
    0,                           /* n_preallocs */
    NULL,                        /* instance_init */
    NULL,                        /* value_table */
  };
  const GTypeFundamentalInfo finfo = { G_TYPE_FLAG_DERIVABLE, };
  GType type;

  /* Int / UInt
   */
  {
    static const GTypeValueTable value_table = {
      value_init_pointer,        /* value_init */
      NULL,                      /* value_free */
      value_copy_int,            /* value_copy */
      value_peek_pointer,        /* value_peek_pointer */
      "p",                       /* collect_format */
      value_collect_int,         /* collect_value */
      "p",                       /* lcopy_format */
      value_lcopy_int,           /* lcopy_value */
    };
    info.value_table = &value_table;
    type = g_type_register_fundamental (G_TYPE_POINTER,
                                        g_intern_static_string ("clutter-animatable-gint"),
                                        &info, &finfo, 0);
    g_assert (type == G_TYPE_POINTER);
    type = g_type_register_fundamental (G_TYPE_POINTER,
                                        g_intern_static_string ("clutter-animatable-guint"),
                                        &info, &finfo, 0);
    g_assert (type == G_TYPE_POINTER);
  }

  /* Float
   */
  {
    static const GTypeValueTable value_table = {
      value_init_pointer,        /* value_init */
      NULL,                      /* value_free */
      value_copy_float,          /* value_copy */
      value_peek_pointer,        /* value_peek_pointer */
      "p",                       /* collect_format */
      value_collect_float,       /* collect_value */
      "p",                       /* lcopy_format */
      value_lcopy_float,         /* lcopy_value */
    };
    info.value_table = &value_table;
    type = g_type_register_fundamental (G_TYPE_POINTER,
                                        g_intern_static_string ("clutter-animatable-gfloat"),
                                        &info, &finfo, 0);
    g_assert (type == G_TYPE_POINTER);
  }

  /* Double
   */
  {
    static const GTypeValueTable value_table = {
      value_init_pointer,        /* value_init */
      NULL,                      /* value_free */
      value_copy_double,         /* value_copy */
      value_peek_pointer,        /* value_peek_pointer */
      "p",                       /* collect_format */
      value_collect_double,      /* collect_value */
      "p",                       /* lcopy_format */
      value_lcopy_double,        /* lcopy_value */
    };
    info.value_table = &value_table;
    type = g_type_register_fundamental (G_TYPE_POINTER,
                                        g_intern_static_string ("clutter-animatable-gdouble"),
                                        &info, &finfo, 0);
    g_assert (type == G_TYPE_POINTER);
  }
}
