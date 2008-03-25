/* Clutter.
 * An OpenGL based 'interactive canvas' library.
 * Authored By Matthew Allum  <mallum@openedhand.com>
 * Copyright (C) 2006-2007 OpenedHand
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
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "clutter-backend-win32.h"
#include "clutter-stage-win32.h"
#include "clutter-win32.h"

#include "../clutter-main.h"
#include "../clutter-feature.h"
#include "../clutter-color.h"
#include "../clutter-util.h"
#include "../clutter-event.h"
#include "../clutter-enum-types.h"
#include "../clutter-private.h"
#include "../clutter-debug.h"
#include "../clutter-units.h"

#include "cogl.h"

#include <windows.h>

G_DEFINE_TYPE (ClutterStageWin32, clutter_stage_win32, CLUTTER_TYPE_STAGE);

static void
clutter_stage_win32_show (ClutterActor *actor)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (actor);

  if (stage_win32->hwnd)
    ShowWindow (stage_win32->hwnd, SW_SHOW);

  /* chain up */
  CLUTTER_ACTOR_CLASS (clutter_stage_win32_parent_class)->show (actor);
}

static void
clutter_stage_win32_hide (ClutterActor *actor)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (actor);

  if (stage_win32->hwnd)
    ShowWindow (stage_win32->hwnd, SW_HIDE);
  
  /* chain up */
  CLUTTER_ACTOR_CLASS (clutter_stage_win32_parent_class)->hide (actor);
}

static void
clutter_stage_win32_query_coords (ClutterActor        *self,
				  ClutterActorBox     *box)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (self);

  /* If we're in fullscreen mode then return the size of the screen
     instead */
  if ((stage_win32->state & CLUTTER_STAGE_STATE_FULLSCREEN))
    {
      box->x1 = CLUTTER_UNITS_FROM_INT (stage_win32->fullscreen_rect.left);
      box->y1 = CLUTTER_UNITS_FROM_INT (stage_win32->fullscreen_rect.top);
      box->x2 = CLUTTER_UNITS_FROM_INT (stage_win32->fullscreen_rect.right);
      box->y2 = CLUTTER_UNITS_FROM_INT (stage_win32->fullscreen_rect.bottom);
    }
  else
    {
      box->x1 = CLUTTER_UNITS_FROM_INT (stage_win32->win_xpos);
      box->y1 = CLUTTER_UNITS_FROM_INT (stage_win32->win_ypos);
      box->x2 = box->x1 + CLUTTER_UNITS_FROM_INT (stage_win32->win_width);
      box->y2 = box->y1 + CLUTTER_UNITS_FROM_INT (stage_win32->win_height);
    }
}

static void
get_fullscreen_rect (ClutterStageWin32 *stage_win32)
{
  HMONITOR monitor;
  MONITORINFO monitor_info;

  /* If we already have a window then try to use the same monitor that
     is already on */
  if (stage_win32->hwnd)
    monitor = MonitorFromWindow (stage_win32->hwnd, MONITOR_DEFAULTTONEAREST);
  else
    {
      /* Otherwise just guess that they will want the monitor where
	 the cursor is */
      POINT cursor;
      GetCursorPos (&cursor);
      monitor = MonitorFromPoint (cursor, MONITOR_DEFAULTTONEAREST);
    }

  monitor_info.cbSize = sizeof (monitor_info);
  GetMonitorInfoW (monitor, &monitor_info);
  stage_win32->fullscreen_rect = monitor_info.rcMonitor;
}

static void
get_full_window_pos (ClutterStageWin32 *stage_win32,
		     int xpos_in, int ypos_in,
		     int *xpos_out, int *ypos_out)
{
  gboolean resizable
    = clutter_stage_get_user_resizable (CLUTTER_STAGE (stage_win32));
  /* The window position passed to CreateWindow includes the window
     decorations */
  *xpos_out = xpos_in - GetSystemMetrics (resizable ? SM_CXSIZEFRAME
					  : SM_CXFIXEDFRAME);
  *ypos_out = ypos_in - GetSystemMetrics (resizable ? SM_CYSIZEFRAME
					  : SM_CYFIXEDFRAME)
    - GetSystemMetrics (SM_CYCAPTION);
}

static void
get_full_window_size (ClutterStageWin32 *stage_win32,
		      int width_in, int height_in,
		      int *width_out, int *height_out)
{
  gboolean resizable
    = clutter_stage_get_user_resizable (CLUTTER_STAGE (stage_win32));
  /* The window size passed to CreateWindow includes the window
     decorations */
  *width_out = width_in + GetSystemMetrics (resizable ? SM_CXSIZEFRAME
					    : SM_CXFIXEDFRAME) * 2;
  *height_out = height_in + GetSystemMetrics (resizable ? SM_CYSIZEFRAME
					      : SM_CYFIXEDFRAME) * 2
    + GetSystemMetrics (SM_CYCAPTION);
}

void
_clutter_stage_win32_get_min_max_info (ClutterStageWin32 *stage_win32,
				       MINMAXINFO *min_max_info)
{
  /* If the window isn't resizable then set the max and min size to
     the current size */
  if (!clutter_stage_get_user_resizable (CLUTTER_STAGE (stage_win32)))
    {
      int full_width, full_height;
      get_full_window_size (stage_win32,
			    stage_win32->win_width, stage_win32->win_height,
			    &full_width, &full_height);
      min_max_info->ptMaxTrackSize.x = full_width;
      min_max_info->ptMinTrackSize.x = full_width;
      min_max_info->ptMaxTrackSize.y = full_height;
      min_max_info->ptMinTrackSize.y = full_height;
    }
}

static void
clutter_stage_win32_request_coords (ClutterActor        *self,
				    ClutterActorBox     *box)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (self);

  gint new_xpos, new_ypos, new_width, new_height;

  new_xpos = CLUTTER_UNITS_TO_INT (MIN (box->x1, box->x2));
  new_ypos = CLUTTER_UNITS_TO_INT (MIN (box->y1, box->y2));
  new_width  = ABS (CLUTTER_UNITS_TO_INT (box->x2 - box->x1));
  new_height = ABS (CLUTTER_UNITS_TO_INT (box->y2 - box->y1)); 

  if ((new_width != stage_win32->win_width
       || new_height != stage_win32->win_height
       || new_xpos != stage_win32->win_xpos
       || new_ypos != stage_win32->win_ypos)
      /* Ignore size requests if we are in full screen mode */
      && (stage_win32->state & CLUTTER_STAGE_STATE_FULLSCREEN) == 0)
    {
      stage_win32->win_xpos = new_xpos;
      stage_win32->win_ypos = new_ypos;
      stage_win32->win_width = new_width;
      stage_win32->win_height = new_height;

      if (stage_win32->hwnd != NULL)
	{
	  int full_xpos, full_ypos, full_width, full_height;

	  get_full_window_pos (stage_win32,
			       new_xpos, new_ypos,
			       &full_xpos, &full_ypos);
	  get_full_window_size (stage_win32,
				new_width, new_height,
				&full_width, &full_height);

	  SetWindowPos (stage_win32->hwnd, NULL,
			full_xpos, full_ypos,
			full_width, full_height,
			SWP_NOZORDER);
	}

      CLUTTER_SET_PRIVATE_FLAGS (self, CLUTTER_ACTOR_SYNC_MATRICES);
    }

  CLUTTER_ACTOR_CLASS (clutter_stage_win32_parent_class)
    ->request_coords (self, box);
}

static void
clutter_stage_win32_set_title (ClutterStage *stage,
			       const gchar  *title)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (stage);
  wchar_t *wtitle;

  /* Empty window titles not allowed, so set it to just a period. */
  if (title == NULL || !title[0])
    title = ".";
  
  wtitle = g_utf8_to_utf16 (title, -1, NULL, NULL, NULL);
  SetWindowTextW (stage_win32->hwnd, wtitle);
  g_free (wtitle);
}

static LONG
get_window_style (ClutterStageWin32 *stage_win32)
{
  /* Fullscreen mode shouldn't have any borders */
  if ((stage_win32->state & CLUTTER_STAGE_STATE_FULLSCREEN))
    return WS_POPUP;
  /* Otherwise it's an overlapped window but if it isn't resizable
     then it shouldn't have a thick frame */
  else if (clutter_stage_get_user_resizable (CLUTTER_STAGE (stage_win32)))
    return WS_OVERLAPPEDWINDOW;
  else
    return WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
}

static void
clutter_stage_win32_set_user_resize (ClutterStage *stage,
				     gboolean      value)
{
  HWND hwnd = CLUTTER_STAGE_WIN32 (stage)->hwnd;
  LONG old_style = GetWindowLongW (hwnd, GWL_STYLE);

  /* Update the window style but preserve the visibility */
  SetWindowLongW (hwnd, GWL_STYLE,
		  get_window_style (CLUTTER_STAGE_WIN32 (stage))
		  | (old_style & WS_VISIBLE));
}

static void
clutter_stage_win32_set_fullscreen (ClutterStage *stage,
				    gboolean      value)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (stage);
  HWND hwnd = CLUTTER_STAGE_WIN32 (stage)->hwnd;
  LONG old_style = GetWindowLongW (hwnd, GWL_STYLE);
  ClutterStageStateEvent event;

  if (value)
    stage_win32->state |= CLUTTER_STAGE_STATE_FULLSCREEN;
  else
    stage_win32->state &= ~CLUTTER_STAGE_STATE_FULLSCREEN;

  if (hwnd)
    {
      /* Update the window style but preserve the visibility */
      SetWindowLongW (hwnd, GWL_STYLE,
		      get_window_style (stage_win32)
		      | (old_style & WS_VISIBLE));
      /* Update the window size */
      if (value)
	{
	  get_fullscreen_rect (stage_win32);
	  SetWindowPos (hwnd, HWND_TOP,
			stage_win32->fullscreen_rect.left,
			stage_win32->fullscreen_rect.top,
			stage_win32->fullscreen_rect.right
			- stage_win32->fullscreen_rect.left,
			stage_win32->fullscreen_rect.bottom
			- stage_win32->fullscreen_rect.top,
			0);
	}
      else
	{
	  int full_xpos, full_ypos, full_width, full_height;

	  get_full_window_pos (stage_win32,
			       stage_win32->win_xpos,
			       stage_win32->win_ypos,
			       &full_xpos, &full_ypos);
	  get_full_window_size (stage_win32,
				stage_win32->win_width,
				stage_win32->win_height,
				&full_width, &full_height);

	  SetWindowPos (stage_win32->hwnd, NULL,
			full_xpos, full_ypos,
			full_width, full_height,
			SWP_NOZORDER);
	}

      CLUTTER_SET_PRIVATE_FLAGS (stage, CLUTTER_ACTOR_SYNC_MATRICES);
    }

  /* Report the state change */
  memset (&event, 0, sizeof (event));
  event.type = CLUTTER_STAGE_STATE;
  event.new_state = stage_win32->state;
  event.changed_mask = CLUTTER_STAGE_STATE_FULLSCREEN;
  clutter_event_put ((ClutterEvent *) &event);
}

static ATOM
clutter_stage_win32_get_window_class ()
{
  static ATOM klass = 0;

  if (klass == 0)
    {
      WNDCLASSW wndclass;
      memset (&wndclass, 0, sizeof (wndclass));
      wndclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndclass.lpfnWndProc = _clutter_stage_win32_window_proc;
      wndclass.cbWndExtra = sizeof (LONG_PTR);
      wndclass.hInstance = GetModuleHandleW (NULL);
      wndclass.hIcon = LoadIconW (NULL, (LPWSTR) IDI_APPLICATION);
      wndclass.hCursor = LoadCursorW (NULL, (LPWSTR) IDC_ARROW);
      wndclass.hbrBackground = NULL;
      wndclass.lpszMenuName = NULL;
      wndclass.lpszClassName = L"ClutterStageWin32";
      klass = RegisterClassW (&wndclass);
    }

  return klass;
}

static gboolean
clutter_stage_win32_check_gl_version ()
{
  const char *version_string, *major_end, *minor_end;
  int major = 0, minor = 0;
  
  /* Get the OpenGL version number */
  if ((version_string = (const char *) glGetString (GL_VERSION)) == NULL)
    return FALSE;

  /* Extract the major number */
  for (major_end = version_string; *major_end >= '0'
	 && *major_end <= '9'; major_end++)
    major = (major * 10) + *major_end - '0';
  /* If there were no digits or the major number isn't followed by a
     dot then it is invalid */
  if (major_end == version_string || *major_end != '.')
    return FALSE;
  
  /* Extract the minor number */
  for (minor_end = major_end + 1; *minor_end >= '0'
	 && *minor_end <= '9'; minor_end++)
    minor = (minor * 10) + *minor_end - '0';
  /* If there were no digits or there is an unexpected character then
     it is invalid */
  if (minor_end == major_end + 1
      || (*minor_end && *minor_end != ' ' && *minor_end != '.'))
    return FALSE;

  /* Accept OpenGL 1.2 or later */
  return major > 1 || (major == 1 && minor >= 2);
}

static void
clutter_stage_win32_realize (ClutterActor *actor)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (actor);
  PIXELFORMATDESCRIPTOR pfd;
  int pf;

  CLUTTER_NOTE (MISC, "Realizing main stage");

  if (stage_win32->hwnd == NULL)
    {
      ATOM window_class = clutter_stage_win32_get_window_class ();
      int win_xpos, win_ypos, win_width, win_height;
      RECT win_rect;
      POINT actual_pos;

      if (window_class == 0)
	{
          g_critical ("Unable to register window class");
          CLUTTER_ACTOR_UNSET_FLAGS (actor, CLUTTER_ACTOR_REALIZED);
          return;
	}

      /* If we're in fullscreen mode then use the fullscreen rect
	 instead */
      if ((stage_win32->state & CLUTTER_STAGE_STATE_FULLSCREEN))
	{
	  get_fullscreen_rect (stage_win32);
	  win_xpos = stage_win32->fullscreen_rect.left;
	  win_ypos = stage_win32->fullscreen_rect.top;
	  win_width = stage_win32->fullscreen_rect.right - win_xpos;
	  win_height = stage_win32->fullscreen_rect.left - win_ypos;
	}
      else
	{
	  if (stage_win32->win_xpos == 0 && stage_win32->win_ypos == 0)
	    win_xpos = win_ypos = CW_USEDEFAULT;
	  else
	    get_full_window_pos (stage_win32,
				 stage_win32->win_xpos, stage_win32->win_ypos,
				 &win_xpos, &win_ypos);
	  get_full_window_size (stage_win32,
				stage_win32->win_width,
				stage_win32->win_height,
				&win_width, &win_height);
	}

      stage_win32->hwnd = CreateWindowW ((LPWSTR) MAKEINTATOM (window_class),
					 L".",
					 get_window_style (stage_win32),
					 win_xpos,
					 win_ypos,
					 win_width,
					 win_height,
					 NULL, NULL,
					 GetModuleHandle (NULL),
					 NULL);

      if (stage_win32->hwnd == NULL)
	{
	  g_critical ("Unable to create stage window");
	  CLUTTER_ACTOR_UNSET_FLAGS (actor, CLUTTER_ACTOR_REALIZED);
	  return;
	}

      /* Get the position in case CW_USEDEFAULT was specified */
      if ((stage_win32->state & CLUTTER_STAGE_STATE_FULLSCREEN) == 0)
	{
	  GetClientRect (stage_win32->hwnd, &win_rect);
	  actual_pos.x = win_rect.left;
	  actual_pos.y = win_rect.top;
	  ClientToScreen (stage_win32->hwnd, &actual_pos);
	  stage_win32->win_xpos = actual_pos.x;
	  stage_win32->win_ypos = actual_pos.y;
	}
      
      /* Store a pointer to the actor in the extra bytes of the window
	 so we can quickly access it in the window procedure */
      SetWindowLongPtrW (stage_win32->hwnd, 0, (LONG_PTR) stage_win32);
    }

  if (stage_win32->gl_context)
    wglDeleteContext (stage_win32->gl_context);

  if (stage_win32->client_dc)
    ReleaseDC (stage_win32->hwnd, stage_win32->client_dc);

  stage_win32->client_dc = GetDC (stage_win32->hwnd);

  memset (&pfd, 0, sizeof (pfd));
  pfd.nSize = sizeof (pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cAlphaBits = 8;
  pfd.cDepthBits = 32;
  pfd.cStencilBits = 8;
  pfd.iLayerType = PFD_MAIN_PLANE;

  if ((pf = ChoosePixelFormat (stage_win32->client_dc, &pfd)) == 0
      || !SetPixelFormat (stage_win32->client_dc, pf, &pfd))
    {
      g_critical ("Unable to find suitable GL pixel format");
      CLUTTER_ACTOR_UNSET_FLAGS (actor, CLUTTER_ACTOR_REALIZED);
      return;
    }

  stage_win32->gl_context = wglCreateContext (stage_win32->client_dc);

  if (stage_win32->gl_context == NULL)
    {
      g_critical ("Unable to create suitable GL context");
      CLUTTER_ACTOR_UNSET_FLAGS (actor, CLUTTER_ACTOR_REALIZED);
      return;
    }

  CLUTTER_NOTE (GL, "wglMakeCurrent");
  wglMakeCurrent (stage_win32->client_dc, stage_win32->gl_context);

  if (!clutter_stage_win32_check_gl_version ())
    {
      g_critical ("OpenGL version number is too low");
      CLUTTER_ACTOR_UNSET_FLAGS (actor, CLUTTER_ACTOR_REALIZED);
      return;
    }
      
  /* Make sure the viewport gets set up correctly */
  CLUTTER_SET_PRIVATE_FLAGS (actor, CLUTTER_ACTOR_SYNC_MATRICES);
}

static void
clutter_stage_win32_unrealize (ClutterActor *actor)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (actor);

  wglMakeCurrent (NULL, NULL);

  if (stage_win32->gl_context != NULL)
    {
      wglDeleteContext (stage_win32->gl_context);
      stage_win32->gl_context = NULL;
    }
  
  if (stage_win32->client_dc)
    {
      ReleaseDC (stage_win32->hwnd, stage_win32->client_dc);
      stage_win32->client_dc = NULL;
    }

  if (stage_win32->hwnd)
    {
      DestroyWindow (stage_win32->hwnd);
      stage_win32->hwnd = NULL;
    }
}

static void
clutter_stage_win32_dispose (GObject *gobject)
{
  ClutterStageWin32 *stage_win32 = CLUTTER_STAGE_WIN32 (gobject);

  if (stage_win32->hwnd)
    clutter_actor_unrealize (CLUTTER_ACTOR (gobject));

  G_OBJECT_CLASS (clutter_stage_win32_parent_class)->dispose (gobject);
}

static void
clutter_stage_win32_class_init (ClutterStageWin32Class *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);
  ClutterStageClass *stage_class = CLUTTER_STAGE_CLASS (klass);

  gobject_class->dispose = clutter_stage_win32_dispose;
  
  actor_class->show = clutter_stage_win32_show;
  actor_class->hide = clutter_stage_win32_hide;
  actor_class->request_coords = clutter_stage_win32_request_coords;
  actor_class->query_coords = clutter_stage_win32_query_coords;
  actor_class->realize = clutter_stage_win32_realize;
  actor_class->unrealize = clutter_stage_win32_unrealize;
  
  stage_class->set_title = clutter_stage_win32_set_title;
  stage_class->set_user_resize = clutter_stage_win32_set_user_resize;
  stage_class->set_fullscreen = clutter_stage_win32_set_fullscreen;
}

static void
clutter_stage_win32_init (ClutterStageWin32 *stage)
{
  stage->hwnd = NULL;
  stage->client_dc = NULL;
  stage->gl_context = NULL;
  stage->win_xpos = 0;
  stage->win_ypos = 0;
  stage->win_width = 640;
  stage->win_height = 480;
  stage->backend = NULL;
  stage->scroll_pos = 0;
}

/**
 * clutter_win32_get_stage_window:
 * @stage: a #ClutterStage
 *
 * Gets the stages window handle
 *
 * Return value: An HWND for the stage window.
 *
 * Since: 0.8
 */
HWND
clutter_win32_get_stage_window (ClutterStage *stage)
{
  g_return_val_if_fail (CLUTTER_IS_STAGE_WIN32 (stage), NULL);

  return CLUTTER_STAGE_WIN32 (stage)->hwnd;
}

void
clutter_stage_win32_map (ClutterStageWin32 *stage_win32)
{
  CLUTTER_ACTOR_SET_FLAGS (stage_win32, CLUTTER_ACTOR_MAPPED);

  clutter_actor_queue_redraw (CLUTTER_ACTOR (stage_win32));
}

void
clutter_stage_win32_unmap (ClutterStageWin32 *stage_win32)
{
  CLUTTER_ACTOR_UNSET_FLAGS (stage_win32, CLUTTER_ACTOR_MAPPED);
}
