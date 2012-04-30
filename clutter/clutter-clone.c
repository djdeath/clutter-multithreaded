/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2008  Intel Corporation.
 *
 * Authored By: Robert Bragg <robert@linux.intel.com>
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
 */

/**
 * SECTION:clutter-clone
 * @short_description: An actor that displays a clone of a source actor
 *
 * #ClutterClone is a #ClutterActor which draws with the paint
 * function of another actor, scaled to fit its own allocation.
 *
 * #ClutterClone can be used to efficiently clone any other actor.
 *
 * <note><para>This is different from clutter_texture_new_from_actor()
 * which requires support for FBOs in the underlying GL
 * implementation.</para></note>
 *
 * #ClutterClone is available since Clutter 1.0
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "clutter-actor-private.h"
#include "clutter-clone.h"
#include "clutter-debug.h"
#include "clutter-main.h"
#include "clutter-paint-volume-private.h"
#include "clutter-private.h"

#include "cogl/cogl.h"

G_DEFINE_TYPE (ClutterClone, clutter_clone, CLUTTER_TYPE_ACTOR);

enum
{
  PROP_0,

  PROP_SOURCE,

  PROP_LAST
};

static GParamSpec *obj_props[PROP_LAST];

#define CLUTTER_CLONE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_CLONE, ClutterClonePrivate))

struct _ClutterClonePrivate
{
  ClutterActor *clone_source;
};

static void clutter_clone_set_source_internal (ClutterClone *clone,
					       ClutterActor *source);

static void
clutter_clone_apply_transform (ClutterActor *self, CoglMatrix *matrix)
{
  ClutterClonePrivate *priv = CLUTTER_CLONE (self)->priv;
  ClutterGeometry geom;
  ClutterGeometry source_geom;
  gfloat x_scale;
  gfloat y_scale;

  /* First chain up and apply all the standard ClutterActor
   * transformations... */
  CLUTTER_ACTOR_CLASS (clutter_clone_parent_class)->apply_transform (self,
                                                                     matrix);

  /* if we don't have a source, nothing else to do */
  if (priv->clone_source == NULL)
    return;

  /* get our allocated size */
  clutter_actor_get_allocation_geometry (self, &geom);

  /* and get the allocated size of the source */
  clutter_actor_get_allocation_geometry (priv->clone_source, &source_geom);

  /* We need to scale what the clone-source actor paints to fill our own
   * allocation...
   */
  x_scale = (gfloat) geom.width  / source_geom.width;
  y_scale = (gfloat) geom.height / source_geom.height;

  cogl_matrix_scale (matrix, x_scale, y_scale, x_scale);
}

static void
clutter_clone_paint (ClutterActor *actor)
{
  ClutterClone *self = CLUTTER_CLONE (actor);
  ClutterClonePrivate *priv = self->priv;
  gboolean was_unmapped = FALSE;

  if (priv->clone_source == NULL)
    return;

  CLUTTER_NOTE (PAINT, "painting clone actor '%s'",
                _clutter_actor_get_debug_name (actor));

  /* The final bits of magic:
   * - We need to override the paint opacity of the actor with our own
   *   opacity.
   * - We need to inform the actor that it's in a clone paint (for the function
   *   clutter_actor_is_in_clone_paint())
   * - We need to stop clutter_actor_paint applying the model view matrix of
   *   the clone source actor.
   */
  _clutter_actor_set_in_clone_paint (priv->clone_source, TRUE);
  _clutter_actor_set_opacity_override (priv->clone_source,
                                       clutter_actor_get_paint_opacity (actor));
  _clutter_actor_set_enable_model_view_transform (priv->clone_source, FALSE);

  if (!CLUTTER_ACTOR_IS_MAPPED (priv->clone_source))
    {
      _clutter_actor_set_enable_paint_unmapped (priv->clone_source, TRUE);
      was_unmapped = TRUE;
    }

  _clutter_actor_push_clone_paint ();
  clutter_actor_paint (priv->clone_source);
  _clutter_actor_pop_clone_paint ();

  if (was_unmapped)
    _clutter_actor_set_enable_paint_unmapped (priv->clone_source, FALSE);

  _clutter_actor_set_enable_model_view_transform (priv->clone_source, TRUE);
  _clutter_actor_set_opacity_override (priv->clone_source, -1);
  _clutter_actor_set_in_clone_paint (priv->clone_source, FALSE);
}

static gboolean
clutter_clone_get_paint_volume (ClutterActor       *actor,
                                ClutterPaintVolume *volume)
{
  ClutterClonePrivate *priv = CLUTTER_CLONE (actor)->priv;
  const ClutterPaintVolume *source_volume;

  /* if the source is not set the paint volume is defined to be empty */
  if (priv->clone_source == NULL)
    return TRUE;

  /* query the volume of the source actor and simply masquarade it as
   * the clones volume... */
  source_volume = clutter_actor_get_paint_volume (priv->clone_source);
  if (source_volume == NULL)
    return FALSE;

  _clutter_paint_volume_set_from_volume (volume, source_volume);
  _clutter_paint_volume_set_reference_actor (volume, actor);

  return TRUE;
}

static gboolean
clutter_clone_has_overlaps (ClutterActor *actor)
{
  ClutterClonePrivate *priv = CLUTTER_CLONE (actor)->priv;

  /* The clone has overlaps iff the source has overlaps */

  if (priv->clone_source == NULL)
    return FALSE;

  return clutter_actor_has_overlaps (priv->clone_source);
}

static void
clutter_clone_set_property (GObject      *gobject,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  ClutterClone *self = CLUTTER_CLONE (gobject);

  switch (prop_id)
    {
    case PROP_SOURCE:
      clutter_clone_set_source (self, g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
  }
}

static void
clutter_clone_get_property (GObject    *gobject,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  ClutterClonePrivate *priv = CLUTTER_CLONE (gobject)->priv;

  switch (prop_id)
    {
    case PROP_SOURCE:
      g_value_set_object (value, priv->clone_source);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
    }
}

static void
clutter_clone_dispose (GObject *gobject)
{
  clutter_clone_set_source_internal (CLUTTER_CLONE (gobject), NULL);

  G_OBJECT_CLASS (clutter_clone_parent_class)->dispose (gobject);
}

static void
clutter_clone_class_init (ClutterCloneClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

  g_type_class_add_private (gobject_class, sizeof (ClutterClonePrivate));

  actor_class->apply_transform = clutter_clone_apply_transform;
  actor_class->paint = clutter_clone_paint;
  actor_class->get_paint_volume = clutter_clone_get_paint_volume;
  actor_class->has_overlaps = clutter_clone_has_overlaps;

  gobject_class->dispose = clutter_clone_dispose;
  gobject_class->set_property = clutter_clone_set_property;
  gobject_class->get_property = clutter_clone_get_property;

  /**
   * ClutterClone:source:
   *
   * This property specifies the source actor being cloned.
   *
   * Since: 1.0
   */
  obj_props[PROP_SOURCE] =
    g_param_spec_object ("source",
                         P_("Source"),
                         P_("Specifies the actor to be cloned"),
                         CLUTTER_TYPE_ACTOR,
                         G_PARAM_CONSTRUCT |
                         CLUTTER_PARAM_READWRITE);

  g_object_class_install_properties (gobject_class, PROP_LAST, obj_props);
}

static void
clutter_clone_init (ClutterClone *self)
{
  ClutterClonePrivate *priv;

  self->priv = priv = CLUTTER_CLONE_GET_PRIVATE (self);

  priv->clone_source = NULL;
}

/**
 * clutter_clone_new:
 * @source: a #ClutterActor, or %NULL
 *
 * Creates a new #ClutterActor which clones @source/
 *
 * Return value: the newly created #ClutterClone
 *
 * Since: 1.0
 */
ClutterActor *
clutter_clone_new (ClutterActor *source)
{
  return g_object_new (CLUTTER_TYPE_CLONE, "source", source,  NULL);
}

static void
clone_source_queue_redraw_cb (ClutterActor *source,
			      ClutterActor *origin,
			      ClutterClone *self)
{
  clutter_actor_queue_redraw (CLUTTER_ACTOR (self));
}

static void
clutter_clone_set_source_internal (ClutterClone *self,
				   ClutterActor *source)
{
  ClutterClonePrivate *priv = self->priv;

  if (priv->clone_source != NULL)
    {
      g_signal_handlers_disconnect_by_func (priv->clone_source,
                                            G_CALLBACK (clone_source_queue_redraw_cb),
					    self);
      g_object_unref (priv->clone_source);
      priv->clone_source = NULL;
    }

  if (source != NULL)
    {
      priv->clone_source = g_object_ref (source);
      g_signal_connect (priv->clone_source, "queue-redraw",
			G_CALLBACK (clone_source_queue_redraw_cb), self);
    }

  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_SOURCE]);

  clutter_actor_queue_redraw (CLUTTER_ACTOR (self));
}

/**
 * clutter_clone_set_source:
 * @self: a #ClutterClone
 * @source: (allow-none): a #ClutterActor, or %NULL
 *
 * Sets @source as the source actor to be cloned by @self.
 *
 * Since: 1.0
 */
void
clutter_clone_set_source (ClutterClone *self,
                          ClutterActor *source)
{
  g_return_if_fail (CLUTTER_IS_CLONE (self));
  g_return_if_fail (source == NULL || CLUTTER_IS_ACTOR (source));

  clutter_clone_set_source_internal (self, source);
  clutter_actor_queue_redraw (CLUTTER_ACTOR (self));
}

/**
 * clutter_clone_get_source:
 * @self: a #ClutterClone
 *
 * Retrieves the source #ClutterActor being cloned by @self.
 *
 * Return value: (transfer none): the actor source for the clone
 *
 * Since: 1.0
 */
ClutterActor *
clutter_clone_get_source (ClutterClone *self)
{
  g_return_val_if_fail (CLUTTER_IS_CLONE (self), NULL);

  return self->priv->clone_source;
}
