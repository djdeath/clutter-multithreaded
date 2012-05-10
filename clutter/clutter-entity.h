#ifndef __CLUTTER_ENTITY_H__
#define __CLUTTER_ENTITY_H__

#include <glib-object.h>

#include <clutter/clutter-component.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_ENTITY clutter_entity_get_type()

#define CLUTTER_ENTITY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CLUTTER_TYPE_ENTITY, ClutterEntity))

#define CLUTTER_ENTITY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CLUTTER_TYPE_ENTITY, ClutterEntityClass))

#define CLUTTER_IS_ENTITY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CLUTTER_TYPE_ENTITY))

#define CLUTTER_IS_ENTITY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CLUTTER_TYPE_ENTITY))

#define CLUTTER_ENTITY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CLUTTER_TYPE_ENTITY, ClutterEntityClass))


typedef struct _ClutterEntity ClutterEntity;
typedef struct _ClutterEntityClass ClutterEntityClass;
typedef struct _ClutterEntityPrivate ClutterEntityPrivate;

#include <clutter/clutter-property.h>

GType clutter_entity_get_type (void) G_GNUC_CONST;

ClutterEntity *clutter_entity_new (void);

const ClutterValue *clutter_entity_get_value_read (ClutterEntity *entity, guint prop_n);
ClutterValue *clutter_entity_get_value_write (ClutterEntity *entity, guint prop_n);

void clutter_entity_append_component (ClutterEntity *entity, ClutterComponent *component);

void clutter_entity_set_position (ClutterEntity *self,
                                  gfloat x,
                                  gfloat y,
                                  gfloat z);
void clutter_entity_get_position (ClutterEntity *self,
                                  gfloat *p_x,
                                  gfloat *p_y,
                                  gfloat *p_z);
void clutter_entity_move_by (ClutterEntity *self,
                             gfloat dx,
                             gfloat dy,
                             gfloat dz);

G_END_DECLS

#endif /* __CLUTTER_ENTITY_H__ */
