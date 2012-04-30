#include <stdlib.h>
#include <glib.h>
#include <gmodule.h>
#include <clutter/clutter.h>

G_MODULE_EXPORT int
test_texture_material_main (int argc, char *argv[])
{
  ClutterActor *stage, *box;
  int i;

  if (clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
    return 1;

  stage = clutter_stage_new ();
  clutter_stage_set_title (CLUTTER_STAGE (stage), "Texture Material");
  g_signal_connect (stage, "destroy", G_CALLBACK (clutter_main_quit), NULL);

  box = clutter_actor_new ();
  /* clutter_actor_set_layout_manager (box, clutter_flow_layout_new (CLUTTER_FLOW_HORIZONTAL)); */
  clutter_actor_add_child (stage, box);

  for (i = 0; i < 48; i++)
    {
      ClutterActor *texture = clutter_texture_new ();

      clutter_texture_set_load_data_async (CLUTTER_TEXTURE (texture), TRUE);
      clutter_texture_set_keep_aspect_ratio (CLUTTER_TEXTURE (texture), TRUE);
      clutter_texture_set_from_file (CLUTTER_TEXTURE (texture),
                                     TESTS_DATADIR "/redhand.png",
                                     NULL);
      clutter_actor_set_width (texture, 96);

      clutter_actor_add_child (box, texture);
    }

  clutter_actor_show (stage);

  clutter_main ();

  return EXIT_SUCCESS;
}
