#include "clutter-component-private.h"
#include "clutter-entity-private.h"
#include "clutter-property-private.h"
#include "clutter-rectangle-component.h"

#include <cogl/cogl.h>

struct _ClutterRectangleComponent
{
  ClutterComponent parent_instance;
};

struct _ClutterRectangleComponentClass
{
  ClutterComponentClass parent_class;
};

G_DEFINE_TYPE (ClutterRectangleComponent,
               clutter_rectangle_component,
               CLUTTER_TYPE_COMPONENT)

/**/

static void
clutter_component_rectangle_render_init (ClutterComponent *component,
                                         ClutterEntity *entity)
{
}

static void
clutter_component_rectangle_render_draw (ClutterComponent *component,
                                         ClutterEntity *entity)
{
  gfloat *width, *height;

  width = CLUTTER_ENTITY_VALUE_DISPLAY (entity, float, width);
  height = CLUTTER_ENTITY_VALUE_DISPLAY (entity, float, height);

  cogl_set_source_color4ub (0xff, 0, 0, 0xff); /* TODO_LIONEL: hardcoded color */
  cogl_rectangle (0, 0, *width, *height);
}

static void
clutter_component_rectangle_render_fini (ClutterComponent *component,
                                         ClutterEntity *entity)
{
}

/**/

static void
clutter_rectangle_component_class_init (ClutterRectangleComponentClass *klass)
{
  ClutterComponentClass *component_class = CLUTTER_COMPONENT_CLASS (klass);

  component_class->init = clutter_component_rectangle_render_init;
  component_class->draw = clutter_component_rectangle_render_draw;
  component_class->fini = clutter_component_rectangle_render_fini;
}

static void
clutter_rectangle_component_init (ClutterRectangleComponent *cnode)
{
}

/**
 * clutter_rectangle_component_new:
 *
 * Creates a new #ClutterRectangleComponent that will paint a solid
 * color rectangle.
 *
 * Return value: (transfer full): the newly created #ClutterComponent.
 *   Use clutter_component_unref() when done
 *
 */
ClutterComponent *
clutter_rectangle_component_new (void)
{
  ClutterComponent *component =
    clutter_component_new (CLUTTER_TYPE_RECTANGLE_COMPONENT);

  component->properties = g_new0 (ClutterProperty *, 3);
  component->properties[0] = clutter_property_float_new (CLUTTER_PROP (width),
                                                         -G_MAXFLOAT,
                                                         G_MAXFLOAT,
                                                         10);
  component->properties[1] = clutter_property_float_new (CLUTTER_PROP (height),
                                                         -G_MAXFLOAT,
                                                         G_MAXFLOAT,
                                                         10);


  return component;
}
