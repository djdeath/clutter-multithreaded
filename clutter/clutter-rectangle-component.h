#ifndef __CLUTTER_RECTANGLE_COMPONENT_H__
#define __CLUTTER_RECTANGLE_COMPONENT_H__

#include <clutter/clutter-component.h>

#define CLUTTER_TYPE_RECTANGLE_COMPONENT (clutter_rectangle_component_get_type ())
#define CLUTTER_RECTANGLE_COMPONENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                               CLUTTER_TYPE_RECTANGLE_COMPONENT, \
                               ClutterRectangleComponent))
#define CLUTTER_IS_RECTANGLE_COMPONENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                               CLUTTER_TYPE_RECTANGLE_COMPONENT))

/**
 * ClutterRectangleComponent:
 *
 * The <structname>ClutterRectangleComponent</structname> structure is
 * an opaque type whose members cannot be directly accessed.
 *
 */
typedef struct _ClutterRectangleComponent ClutterRectangleComponent;
typedef struct _ClutterRectangleComponentClass ClutterRectangleComponentClass;

GType clutter_rectangle_component_get_type (void) G_GNUC_CONST;

ClutterComponent *clutter_rectangle_component_new (void);

#endif /* __CLUTTER_COMPONENT_RECTANGLE_H__ */
