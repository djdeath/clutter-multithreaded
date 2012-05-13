#include <clutter/clutter.h>

#include <stdlib.h>

#define NB_ENTITIES (20)

static guint pos;

static ClutterEntity *rects[NB_ENTITIES];

static gboolean
timeout_cb (gpointer data)
{
  gint i;
  static gint counter = 0;

  pos = (pos + 1) % 1024;
  for (i = 0; i < NB_ENTITIES; i++)
    {
      if (pos == 0)
        clutter_entity_set_position (rects[i], i, i, 0);
      else
        clutter_entity_move_by (rects[i], 1, 0, 0);
    }

  return TRUE;
}

G_MODULE_EXPORT int
test_entity_main (int argc, char *argv[])
{
  gint i;
  ClutterActor *stage;

  if (clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
    return EXIT_FAILURE;

  stage = clutter_stage_new ();
  clutter_actor_set_size (stage, 1024, 768);
  g_signal_connect (stage, "destroy", G_CALLBACK (clutter_main_quit), NULL);
  clutter_stage_set_title (CLUTTER_STAGE (stage), "Three Flowers in a Vase");
  clutter_stage_set_user_resizable (CLUTTER_STAGE (stage), TRUE);

  /* there are three flowers in a vase */
  for (i = 0; i < NB_ENTITIES; i++)
    {
      rects[i] = clutter_entity_new ();
      clutter_entity_append_component (rects[i], clutter_rectangle_component_new ());
      clutter_stage_append_entity (CLUTTER_STAGE (stage), rects[i]);
      clutter_entity_set_position (rects[i], i, i, 0);
    }

  clutter_actor_show (stage);

  g_timeout_add (20, (GSourceFunc) timeout_cb, NULL);

  clutter_main ();

  return EXIT_SUCCESS;
}

G_MODULE_EXPORT const char *
test_entity_describe (void)
{
  return "Basic example of entity usage.";
}
