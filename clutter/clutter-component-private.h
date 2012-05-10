#ifndef __CLUTTER_COMPONENT_PRIVATE_H__
#define __CLUTTER_COMPONENT_PRIVATE_H__

#include <glib-object.h>

#include <cogl/cogl.h>

#include <clutter/clutter-component.h>
#include <clutter/clutter-entity.h>
#include <clutter/clutter-property.h>

G_BEGIN_DECLS

#define CLUTTER_COMPONENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
   CLUTTER_TYPE_COMPONENT, ClutterComponentClass))
#define CLUTTER_IS_COMPONENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_COMPONENT))
#define CLUTTER_COMPONENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
   CLUTTER_TYPE_COMPONENT, ClutterComponentClass))

typedef void (*ClutterComponentInit) (ClutterComponent *component,
                                      ClutterEntity *entity);
typedef void (*ClutterComponentDraw) (ClutterComponent *component,
                                      ClutterEntity *entity);
typedef void (*ClutterComponentFini) (ClutterComponent *component,
                                      ClutterEntity *entity);

struct _ClutterComponentClass
{
  GTypeClass base_class;

  ClutterComponentInit init;
  ClutterComponentDraw draw;
  ClutterComponentFini fini;
};

struct _ClutterComponent
{
  GTypeInstance parent_instance;

  volatile int ref_count;

  const gchar *name;

  ClutterProperty **properties;
};

extern CoglPipeline *_clutter_component_default_color_pipeline;
extern CoglPipeline *_clutter_component_default_texture_pipeline;

ClutterComponent *clutter_component_new (GType gtype);

void clutter_component_render_init (void);

G_END_DECLS

#endif /* __CLUTTER_COMPONENT_PRIVATE_H__ */
