#if !defined(__CLUTTER_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <clutter/clutter.h> can be included directly."
#endif

#ifndef __CLUTTER_PROPERTY_H__
#define __CLUTTER_PROPERTY_H__

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _ClutterValue ClutterValue;
typedef struct _ClutterProperty ClutterProperty;
typedef struct _ClutterPropertyClass ClutterPropertyClass;

#include <clutter/clutter-entity.h>

#define CLUTTER_TYPE_PROPERTY (clutter_property_get_type ())
#define CLUTTER_PROPERTY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_PROPERTY, ClutterProperty))
#define CLUTTER_IS_PROPERTY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_PROPERTY))


GType clutter_property_get_type (void) G_GNUC_CONST;

ClutterProperty *clutter_property_ref (ClutterProperty *property);
void clutter_property_unref (ClutterProperty *property);

const gchar *clutter_property_get_name (ClutterProperty *property);

/* TODO_LIONEL: Really necessary public API? */
ClutterEntity *clutter_property_get_entity (ClutterProperty *property);

G_END_DECLS

#endif /* __CLUTTER_PROPERTY_H__ */
