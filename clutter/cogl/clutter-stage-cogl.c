/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2007,2008,2009,2010,2011  Intel Corporation.
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

 * Authors:
 *  Matthew Allum
 *  Robert Bragg
 *  Neil Roberts
 *  Emmanuele Bassi
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CLUTTER_ENABLE_EXPERIMENTAL_API

#include "clutter-config.h"

#include "clutter-stage-cogl.h"

#include "clutter-actor-private.h"
#include "clutter-backend-private.h"
#include "clutter-debug.h"
#include "clutter-event.h"
#include "clutter-enum-types.h"
#include "clutter-feature.h"
#include "clutter-main.h"
#include "clutter-private.h"
#include "clutter-profile.h"
#include "clutter-stage-private.h"
#include "clutter-util.h"

static void clutter_stage_window_iface_init (ClutterStageWindowIface *iface);

G_DEFINE_TYPE_WITH_CODE (ClutterStageCogl,
                         _clutter_stage_cogl,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_STAGE_WINDOW,
                                                clutter_stage_window_iface_init));

enum {
  PROP_0,
  PROP_WRAPPER,
  PROP_BACKEND,
  PROP_LAST
};

static void
clutter_stage_cogl_unrealize (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  CLUTTER_NOTE (BACKEND, "Unrealizing Cogl stage [%p]", stage_cogl);

  if (stage_cogl->onscreen != NULL)
    {
      cogl_object_unref (stage_cogl->onscreen);
      stage_cogl->onscreen = NULL;
    }
}

static void
handle_swap_complete_cb (CoglFramebuffer *framebuffer,
                         void *user_data)
{
  ClutterStageCogl *stage_cogl = user_data;

  /* Early versions of the swap_event implementation in Mesa
   * deliver BufferSwapComplete event when not selected for,
   * so if we get a swap event we aren't expecting, just ignore it.
   *
   * https://bugs.freedesktop.org/show_bug.cgi?id=27962
   *
   * FIXME: This issue can be hidden inside Cogl so we shouldn't
   * need to care about this bug here.
   */
  if (stage_cogl->pending_swaps > 0)
    stage_cogl->pending_swaps--;
}

static gboolean
clutter_stage_cogl_realize (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  ClutterBackend *backend;
  CoglFramebuffer *framebuffer;
  GError *error = NULL;
  gfloat width = 800;
  gfloat height = 600;

  CLUTTER_NOTE (BACKEND, "Realizing stage '%s' [%p]",
                G_OBJECT_TYPE_NAME (stage_cogl),
                stage_cogl);

  backend = clutter_get_default_backend ();

  if (stage_cogl->onscreen == NULL)
    {
      stage_cogl->onscreen = cogl_onscreen_new (backend->cogl_context,
						width, height);
    }

  cogl_onscreen_set_swap_throttled (stage_cogl->onscreen,
                                    _clutter_get_sync_to_vblank ());

  framebuffer = COGL_FRAMEBUFFER (stage_cogl->onscreen);
  if (!cogl_framebuffer_allocate (framebuffer, &error))
    {
      g_warning ("Failed to allocate stage: %s", error->message);
      g_error_free (error);
      cogl_object_unref (stage_cogl->onscreen);
      stage_cogl->onscreen = NULL;
      return FALSE;
    }

  /* FIXME: for fullscreen Cogl platforms then the size we gave
   * will be ignored, so we need to make sure the stage size is
   * updated to this size. */

  if (cogl_clutter_winsys_has_feature (COGL_WINSYS_FEATURE_SWAP_BUFFERS_EVENT))
    {
      stage_cogl->swap_callback_id =
        cogl_onscreen_add_swap_buffers_callback (stage_cogl->onscreen,
                                                 handle_swap_complete_cb,
                                                 stage_cogl);
    }

  return TRUE;
}

static int
clutter_stage_cogl_get_pending_swaps (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  return stage_cogl->pending_swaps;
}

static ClutterActor *
clutter_stage_cogl_get_wrapper (ClutterStageWindow *stage_window)
{
  return CLUTTER_ACTOR (CLUTTER_STAGE_COGL (stage_window)->wrapper);
}

static void
clutter_stage_cogl_show (ClutterStageWindow *stage_window,
			 gboolean            do_raise)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  clutter_actor_map (CLUTTER_ACTOR (stage_cogl->wrapper));
}

static void
clutter_stage_cogl_hide (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  clutter_actor_unmap (CLUTTER_ACTOR (stage_cogl->wrapper));
}

static void
clutter_stage_cogl_get_geometry (ClutterStageWindow    *stage_window,
                                 cairo_rectangle_int_t *geometry)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  if (geometry)
    {
      if (stage_cogl->onscreen)
        {
          CoglFramebuffer *framebuffer =
            COGL_FRAMEBUFFER (stage_cogl->onscreen);

          geometry->x = geometry->y = 0;

          geometry->width = cogl_framebuffer_get_width (framebuffer);
          geometry->height = cogl_framebuffer_get_height (framebuffer);
        }
      else
        {
          geometry->x = geometry->y = 0;
          geometry->width = 800;
          geometry->height = 600;
        }
    }
}

static void
clutter_stage_cogl_resize (ClutterStageWindow *stage_window,
                          gint                width,
                          gint                height)
{
}

static gboolean
clutter_stage_cogl_has_redraw_clips (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  /* NB: at the start of each new frame there is an implied clip that
   * clips everything (i.e. nothing would be drawn) so we need to make
   * sure we return True in the un-initialized case here.
   *
   * NB: a clip width of 0 means a full stage redraw has been queued
   * so we effectively don't have any redraw clips in that case.
   */
  if (!stage_cogl->initialized_redraw_clip ||
      (stage_cogl->initialized_redraw_clip &&
       stage_cogl->bounding_redraw_clip.width != 0))
    return TRUE;
  else
    return FALSE;
}

static gboolean
clutter_stage_cogl_ignoring_redraw_clips (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  /* NB: a clip width of 0 means a full stage redraw is required */
  if (stage_cogl->initialized_redraw_clip &&
      stage_cogl->bounding_redraw_clip.width == 0)
    return TRUE;
  else
    return FALSE;
}

/* A redraw clip represents (in stage coordinates) the bounding box of
 * something that needs to be redraw. Typically they are added to the
 * StageWindow as a result of clutter_actor_queue_clipped_redraw() by
 * actors such as ClutterGLXTexturePixmap. All redraw clips are
 * discarded after the next paint.
 *
 * A NULL stage_clip means the whole stage needs to be redrawn.
 *
 * What we do with this information:
 * - we keep track of the bounding box for all redraw clips
 * - when we come to redraw; we scissor the redraw to that box and use
 *   glBlitFramebuffer to present the redraw to the front
 *   buffer.
 */
static void
clutter_stage_cogl_add_redraw_clip (ClutterStageWindow    *stage_window,
                                    cairo_rectangle_int_t *stage_clip)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  /* If we are already forced to do a full stage redraw then bail early */
  if (clutter_stage_cogl_ignoring_redraw_clips (stage_window))
    return;

  /* A NULL stage clip means a full stage redraw has been queued and
   * we keep track of this by setting a zero width
   * stage_cogl->bounding_redraw_clip */
  if (stage_clip == NULL)
    {
      stage_cogl->bounding_redraw_clip.width = 0;
      stage_cogl->initialized_redraw_clip = TRUE;
      return;
    }

  /* Ignore requests to add degenerate/empty clip rectangles */
  if (stage_clip->width == 0 || stage_clip->height == 0)
    return;

  if (!stage_cogl->initialized_redraw_clip)
    {
      stage_cogl->bounding_redraw_clip = *stage_clip;
    }
  else if (stage_cogl->bounding_redraw_clip.width > 0)
    {
      _clutter_util_rectangle_union (&stage_cogl->bounding_redraw_clip,
                                     stage_clip,
                                     &stage_cogl->bounding_redraw_clip);
    }

  stage_cogl->initialized_redraw_clip = TRUE;
}

static gboolean
clutter_stage_cogl_get_redraw_clip_bounds (ClutterStageWindow    *stage_window,
                                           cairo_rectangle_int_t *stage_clip)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  if (stage_cogl->using_clipped_redraw)
    {
      *stage_clip = stage_cogl->bounding_redraw_clip;

      return TRUE;
    }

  return FALSE;
}

/* XXX: This is basically identical to clutter_stage_glx_redraw */
static void
clutter_stage_cogl_redraw (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);
  gboolean may_use_clipped_redraw;
  gboolean use_clipped_redraw;
  gboolean can_blit_sub_buffer;
  ClutterActor *wrapper;

  CLUTTER_STATIC_TIMER (painting_timer,
                        "Redrawing", /* parent */
                        "Painting actors",
                        "The time spent painting actors",
                        0 /* no application private data */);
  CLUTTER_STATIC_TIMER (swapbuffers_timer,
                        "Redrawing", /* parent */
                        "SwapBuffers",
                        "The time spent blocked by SwapBuffers",
                        0 /* no application private data */);
  CLUTTER_STATIC_TIMER (blit_sub_buffer_timer,
                        "Redrawing", /* parent */
                        "blit_sub_buffer",
                        "The time spent in blit_sub_buffer",
                        0 /* no application private data */);

  wrapper = CLUTTER_ACTOR (stage_cogl->wrapper);

  if (!stage_cogl->onscreen)
    return;

  CLUTTER_TIMER_START (_clutter_uprof_context, painting_timer);

  can_blit_sub_buffer =
    cogl_clutter_winsys_has_feature (COGL_WINSYS_FEATURE_SWAP_REGION);

  may_use_clipped_redraw = FALSE;
  if (_clutter_stage_window_can_clip_redraws (stage_window) &&
      can_blit_sub_buffer &&
      /* NB: a zero width redraw clip == full stage redraw */
      stage_cogl->bounding_redraw_clip.width != 0 &&
      /* some drivers struggle to get going and produce some junk
       * frames when starting up... */
      stage_cogl->frame_count > 3)
    {
      may_use_clipped_redraw = TRUE;
    }

  if (may_use_clipped_redraw &&
      G_LIKELY (!(clutter_paint_debug_flags &
                  CLUTTER_DEBUG_DISABLE_CLIPPED_REDRAWS)))
    use_clipped_redraw = TRUE;
  else
    use_clipped_redraw = FALSE;

  if (use_clipped_redraw)
    {
      CLUTTER_NOTE (CLIPPING,
                    "Stage clip pushed: x=%d, y=%d, width=%d, height=%d\n",
                    stage_cogl->bounding_redraw_clip.x,
                    stage_cogl->bounding_redraw_clip.y,
                    stage_cogl->bounding_redraw_clip.width,
                    stage_cogl->bounding_redraw_clip.height);

      stage_cogl->using_clipped_redraw = TRUE;

      cogl_clip_push_window_rectangle (stage_cogl->bounding_redraw_clip.x,
                                       stage_cogl->bounding_redraw_clip.y,
                                       stage_cogl->bounding_redraw_clip.width,
                                       stage_cogl->bounding_redraw_clip.height);
      _clutter_stage_do_paint (CLUTTER_STAGE (wrapper),
                               &stage_cogl->bounding_redraw_clip);
      cogl_clip_pop ();

      stage_cogl->using_clipped_redraw = FALSE;
    }
  else
    {
      CLUTTER_NOTE (CLIPPING, "Unclipped stage paint\n");

      /* If we are trying to debug redraw issues then we want to pass
       * the bounding_redraw_clip so it can be visualized */
      if (G_UNLIKELY (clutter_paint_debug_flags &
                      CLUTTER_DEBUG_DISABLE_CLIPPED_REDRAWS) &&
          may_use_clipped_redraw)
        {
          _clutter_stage_do_paint (CLUTTER_STAGE (wrapper),
                                   &stage_cogl->bounding_redraw_clip);
        }
      else
        _clutter_stage_do_paint (CLUTTER_STAGE (wrapper), NULL);
    }

  if (may_use_clipped_redraw &&
      G_UNLIKELY ((clutter_paint_debug_flags & CLUTTER_DEBUG_REDRAWS)))
    {
      CoglContext *ctx =
        clutter_backend_get_cogl_context (clutter_get_default_backend ());
      static CoglPipeline *outline = NULL;
      cairo_rectangle_int_t *clip = &stage_cogl->bounding_redraw_clip;
      ClutterActor *actor = CLUTTER_ACTOR (wrapper);
      CoglHandle vbo;
      float x_1 = clip->x;
      float x_2 = clip->x + clip->width;
      float y_1 = clip->y;
      float y_2 = clip->y + clip->height;
      float quad[8] = {
        x_1, y_1,
        x_2, y_1,
        x_2, y_2,
        x_1, y_2
      };
      CoglMatrix modelview;

      if (outline == NULL)
        {
          outline = cogl_pipeline_new (ctx);
          cogl_pipeline_set_color4ub (outline, 0xff, 0x00, 0x00, 0xff);
        }

      vbo = cogl_vertex_buffer_new (4);
      cogl_vertex_buffer_add (vbo,
                              "gl_Vertex",
                              2, /* n_components */
                              COGL_ATTRIBUTE_TYPE_FLOAT,
                              FALSE, /* normalized */
                              0, /* stride */
                              quad);
      cogl_vertex_buffer_submit (vbo);

      cogl_push_matrix ();
      cogl_matrix_init_identity (&modelview);
      _clutter_actor_apply_modelview_transform (actor, &modelview);
      cogl_set_modelview_matrix (&modelview);
      cogl_set_source (outline);
      cogl_vertex_buffer_draw (vbo, COGL_VERTICES_MODE_LINE_LOOP,
                               0 , 4);
      cogl_pop_matrix ();
      cogl_object_unref (vbo);
    }

  CLUTTER_TIMER_STOP (_clutter_uprof_context, painting_timer);

  /* push on the screen */
  if (use_clipped_redraw)
    {
      cairo_rectangle_int_t *clip = &stage_cogl->bounding_redraw_clip;
      int copy_area[4];

      /* XXX: It seems there will be a race here in that the stage
       * window may be resized before the cogl_onscreen_swap_region
       * is handled and so we may copy the wrong region. I can't
       * really see how we can handle this with the current state of X
       * but at least in this case a full redraw should be queued by
       * the resize anyway so it should only exhibit temporary
       * artefacts.
       */

      copy_area[0] = clip->x;
      copy_area[1] = clip->y;
      copy_area[2] = clip->width;
      copy_area[3] = clip->height;

      CLUTTER_NOTE (BACKEND,
                    "cogl_onscreen_swap_region (onscreen: %p, "
                                                "x: %d, y: %d, "
                                                "width: %d, height: %d)",
                    stage_cogl->onscreen,
                    copy_area[0], copy_area[1], copy_area[2], copy_area[3]);


      CLUTTER_TIMER_START (_clutter_uprof_context, blit_sub_buffer_timer);

      cogl_onscreen_swap_region (stage_cogl->onscreen, copy_area, 1);

      CLUTTER_TIMER_STOP (_clutter_uprof_context, blit_sub_buffer_timer);
    }
  else
    {
      CLUTTER_NOTE (BACKEND, "cogl_onscreen_swap_buffers (onscreen: %p)",
                    stage_cogl->onscreen);

      /* If we have swap buffer events then cogl_onscreen_swap_buffers
       * will return immediately and we need to track that there is a
       * swap in progress... */
      if (clutter_feature_available (CLUTTER_FEATURE_SWAP_EVENTS))
        stage_cogl->pending_swaps++;

      CLUTTER_TIMER_START (_clutter_uprof_context, swapbuffers_timer);
      cogl_onscreen_swap_buffers (stage_cogl->onscreen);
      CLUTTER_TIMER_STOP (_clutter_uprof_context, swapbuffers_timer);
    }

  /* reset the redraw clipping for the next paint... */
  stage_cogl->initialized_redraw_clip = FALSE;

  stage_cogl->frame_count++;
}

static CoglFramebuffer *
clutter_stage_cogl_get_active_framebuffer (ClutterStageWindow *stage_window)
{
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_window);

  return COGL_FRAMEBUFFER (stage_cogl->onscreen);
}

static void
clutter_stage_window_iface_init (ClutterStageWindowIface *iface)
{
  iface->realize = clutter_stage_cogl_realize;
  iface->unrealize = clutter_stage_cogl_unrealize;
  iface->get_wrapper = clutter_stage_cogl_get_wrapper;
  iface->get_geometry = clutter_stage_cogl_get_geometry;
  iface->resize = clutter_stage_cogl_resize;
  iface->show = clutter_stage_cogl_show;
  iface->hide = clutter_stage_cogl_hide;
  iface->get_pending_swaps = clutter_stage_cogl_get_pending_swaps;
  iface->add_redraw_clip = clutter_stage_cogl_add_redraw_clip;
  iface->has_redraw_clips = clutter_stage_cogl_has_redraw_clips;
  iface->ignoring_redraw_clips = clutter_stage_cogl_ignoring_redraw_clips;
  iface->get_redraw_clip_bounds = clutter_stage_cogl_get_redraw_clip_bounds;
  iface->redraw = clutter_stage_cogl_redraw;
  iface->get_active_framebuffer = clutter_stage_cogl_get_active_framebuffer;
}

static void
clutter_stage_cogl_set_property (GObject      *gobject,
				 guint         prop_id,
				 const GValue *value,
				 GParamSpec   *pspec)
{
  ClutterStageCogl *self = CLUTTER_STAGE_COGL (gobject);

  switch (prop_id)
    {
    case PROP_WRAPPER:
      self->wrapper = g_value_get_object (value);
      break;

    case PROP_BACKEND:
      self->backend = g_value_get_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
    }
}

static void
_clutter_stage_cogl_class_init (ClutterStageCoglClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = clutter_stage_cogl_set_property;

  g_object_class_override_property (gobject_class, PROP_WRAPPER, "wrapper");
  g_object_class_override_property (gobject_class, PROP_BACKEND, "backend");
}

static void
_clutter_stage_cogl_init (ClutterStageCogl *stage)
{
}
