#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define COGL_ENABLE_EXPERIMENTAL_API 1
#define CLUTTER_ENABLE_EXPERIMENTAL_API 1

#include "clutter-private.h"
#include "clutter-component.h"
#include "clutter-component-private.h"
#include "clutter-properties-private.h"
#include "clutter-backend.h"

#include <gobject/gvaluecollector.h>

static void
value_component_init (GValue *value)
{
  value->data[0].v_pointer = NULL;
}

static void
value_component_free_value (GValue *value)
{
  if (value->data[0].v_pointer != NULL)
    clutter_component_unref (value->data[0].v_pointer);
}

static void
value_component_copy_value (const GValue *src,
                             GValue       *dst)
{
  if (src->data[0].v_pointer != NULL)
    dst->data[0].v_pointer = clutter_component_ref (src->data[0].v_pointer);
  else
    dst->data[0].v_pointer = NULL;
}

static gpointer
value_component_peek_pointer (const GValue *value)
{
  return value->data[0].v_pointer;
}

static gchar *
value_component_collect_value (GValue      *value,
                              guint        n_collect_values,
                              GTypeCValue *collect_values,
                              guint        collect_flags)
{
  ClutterComponent *component;

  component = collect_values[0].v_pointer;

  if (component == NULL)
    {
      value->data[0].v_pointer = NULL;
      return NULL;
    }

  if (component->parent_instance.g_class == NULL)
    return g_strconcat ("invalid unclassed ClutterComponent pointer for "
                        "value type '",
                        G_VALUE_TYPE_NAME (value),
                        "'",
                        NULL);

  value->data[0].v_pointer = clutter_component_ref (component);

  return NULL;
}

static gchar *
value_component_lcopy_value (const GValue *value,
                              guint         n_collect_values,
                              GTypeCValue  *collect_values,
                              guint         collect_flags)
{
  ClutterComponent **component_p = collect_values[0].v_pointer;

  if (component_p == NULL)
    return g_strconcat ("value location for '",
                        G_VALUE_TYPE_NAME (value),
                        "' passed as NULL",
                        NULL);

  if (value->data[0].v_pointer == NULL)
    *component_p = NULL;
  else if (collect_flags & G_VALUE_NOCOPY_CONTENTS)
    *component_p = value->data[0].v_pointer;
  else
    *component_p = clutter_component_ref (value->data[0].v_pointer);

  return NULL;
}

static void
clutter_component_class_base_init (ClutterComponentClass *klass)
{
}

static void
clutter_component_class_base_finalize (ClutterComponentClass *klass)
{
}

static void
clutter_component_class_init (ClutterComponentClass *klass)
{
}

static void
clutter_component_init (ClutterComponent *self)
{
  self->ref_count = 1;
}

GType
clutter_component_get_type (void)
{
  static volatile gsize component_type_id__volatile = 0;

  if (g_once_init_enter (&component_type_id__volatile))
    {
      static const GTypeFundamentalInfo finfo = {
        (G_TYPE_FLAG_CLASSED |
         G_TYPE_FLAG_INSTANTIATABLE |
         G_TYPE_FLAG_DERIVABLE |
         G_TYPE_FLAG_DEEP_DERIVABLE),
      };

      static const GTypeValueTable value_table = {
        value_component_init,
        value_component_free_value,
        value_component_copy_value,
        value_component_peek_pointer,
        "p",
        value_component_collect_value,
        "p",
        value_component_lcopy_value,
      };

      const GTypeInfo node_info = {
        sizeof (ClutterComponentClass),

        (GBaseInitFunc) clutter_component_class_base_init,
        (GBaseFinalizeFunc) clutter_component_class_base_finalize,
        (GClassInitFunc) clutter_component_class_init,
        (GClassFinalizeFunc) NULL,
        NULL,

        sizeof (ClutterComponent),
        0,
        (GInstanceInitFunc) clutter_component_init,

        &value_table,
      };

      GType component_type_id =
        g_type_register_fundamental (g_type_fundamental_next (),
                                     I_("ClutterComponent"),
                                     &node_info, &finfo,
                                     G_TYPE_FLAG_ABSTRACT);

      g_once_init_leave (&component_type_id__volatile, component_type_id);
    }

  return component_type_id__volatile;
}

/**
 * clutter_component_ref:
 * @component: a #ClutterComponent
 *
 * Acquires a reference on @component.
 *
 * Return value: (transfer full): the #ClutterComponent
 *
 * Since: 1.10
 */
ClutterComponent *
clutter_component_ref (ClutterComponent *component)
{
  g_return_val_if_fail (CLUTTER_IS_COMPONENT (component), NULL);

  g_atomic_int_inc (&component->ref_count);

  return component;
}

/**
 * clutter_component_unref:
 * @component: a #ClutterComponent
 *
 * Releases a reference on @component.
 *
 * Since: 1.10
 */
void
clutter_component_unref (ClutterComponent *component)
{
  g_return_if_fail (CLUTTER_IS_COMPONENT (component));

  if (g_atomic_int_dec_and_test (&component->ref_count))
    {
      /* TODO_LIONEL: free properties */
      g_type_free_instance ((GTypeInstance *) component);
    }
}

/**
 * clutter_component_get_name:
 * @component: A #ClutterComponent
 *
 * Retrieves the name of @component.
 *
 * Return value: the name of the component, or %NULL. The returned
 *   string is owned by the component and should not be modified or
 *   freed.
 */
const gchar *
clutter_component_get_name (ClutterComponent *component)
{
  g_return_val_if_fail (CLUTTER_IS_COMPONENT (component), NULL);

  return component->name;
}

ClutterComponent *
clutter_component_new (GType gtype)
{
  /* g_return_val_if_fail (g_type_is_a (gtype, CLUTTER_TYPE_PAINT_NODE), NULL); */

  return (ClutterComponent *) g_type_create_instance (gtype);
}

/**/

CoglPipeline *_clutter_component_default_color_pipeline = NULL;
CoglPipeline *_clutter_component_default_texture_pipeline = NULL;

/*< private >
 * _clutter_component_init_types:
 *
 * Initializes the required types for ClutterComponent subclasses
 */
void
clutter_component_render_init (void)
{
  CoglContext *ctx;
  CoglColor cogl_color;

  if (G_LIKELY (_clutter_component_default_color_pipeline != NULL))
    return;

  ctx = clutter_backend_get_cogl_context (clutter_get_default_backend ());

  cogl_color_init_from_4f (&cogl_color, 1.0, 1.0, 1.0, 1.0);

  _clutter_component_default_color_pipeline = cogl_pipeline_new (ctx);
  cogl_pipeline_set_color (_clutter_component_default_color_pipeline, &cogl_color);

  _clutter_component_default_texture_pipeline = cogl_pipeline_new (ctx);
  cogl_pipeline_set_layer_null_texture (_clutter_component_default_texture_pipeline,
                                        0,
                                        COGL_TEXTURE_TYPE_2D);
  cogl_pipeline_set_color (_clutter_component_default_texture_pipeline,
                           &cogl_color);
  cogl_pipeline_set_layer_wrap_mode (_clutter_component_default_texture_pipeline,
                                     0,
                                     COGL_PIPELINE_WRAP_MODE_CLAMP_TO_EDGE);
}
