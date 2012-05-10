#if !defined(__CLUTTER_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <clutter/clutter.h> can be included directly."
#endif

#ifndef __CLUTTER_COMPONENT_H__
#define __CLUTTER_COMPONENT_H__

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _ClutterComponent ClutterComponent;
typedef struct _ClutterComponentClass ClutterComponentClass;

#define CLUTTER_TYPE_COMPONENT (clutter_component_get_type ())
#define CLUTTER_COMPONENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_COMPONENT, ClutterComponent))
#define CLUTTER_IS_COMPONENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_COMPONENT))

GType clutter_component_get_type (void) G_GNUC_CONST;

ClutterComponent *clutter_component_ref (ClutterComponent *component);
void clutter_component_unref (ClutterComponent *component);

const gchar *clutter_component_get_name (ClutterComponent *component);

G_END_DECLS

#endif /* __CLUTTER_COMPONENT_H__ */
