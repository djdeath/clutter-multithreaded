#include <clutter/clutter.h>

#include <stdlib.h>

static guint pos;

static gboolean
timeout_cb (ClutterEntity *rect)
{
  pos = (pos + 1) % 1024;
  if (pos == 0)
    clutter_entity_set_position (rect, 0, 0, 0);
  else
    clutter_entity_move_by (rect, 1, 0, 0);

  return TRUE;
}

G_MODULE_EXPORT int
test_entity_main (int argc, char *argv[])
{
  ClutterActor *stage;
  ClutterEntity *rect;

  if (clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
    return EXIT_FAILURE;

  stage = clutter_stage_new ();
  clutter_actor_set_size (stage, 1024, 768);
  g_signal_connect (stage, "destroy", G_CALLBACK (clutter_main_quit), NULL);
  clutter_stage_set_title (CLUTTER_STAGE (stage), "Three Flowers in a Vase");
  clutter_stage_set_user_resizable (CLUTTER_STAGE (stage), TRUE);

  /* there are three flowers in a vase */
  rect = clutter_entity_new ();
  clutter_entity_append_component (rect, clutter_rectangle_component_new ());
  clutter_stage_append_entity (CLUTTER_STAGE (stage), rect);

  clutter_actor_show (stage);

  g_timeout_add (20, (GSourceFunc) timeout_cb, rect);

  clutter_main ();

  return EXIT_SUCCESS;
}

G_MODULE_EXPORT const char *
test_entity_describe (void)
{
  return "Basic example of entity usage.";
}
