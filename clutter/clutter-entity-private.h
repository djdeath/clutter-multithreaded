#ifndef __CLUTTER_ENTITY_PRIVATE_H__
#define __CLUTTER_ENTITY_PRIVATE_H__

#include "clutter-component.h"
#include "clutter-property.h"
#include "clutter-property-private.h"

#include <clutter/clutter-stage.h>

#define CLUTTER_ENTITY_PROPERTY(entity,prop) \
  ((entity)->properties[CLUTTER_PROP_##prop])

/* It's a bit mental, but that's for your own good. */
#define CLUTTER_ENTITY_VALUE_READ(entity,type,prop)                     \
  (type *) &((clutter_property_get_value_read ((entity)->properties[CLUTTER_PROP_##prop]))->data.v_##type)
#define CLUTTER_ENTITY_VALUE_WRITE(entity,type,prop)                    \
  (type *) &((clutter_property_get_value_write ((entity)->properties[CLUTTER_PROP_##prop]))->data.v_##type)
#define CLUTTER_ENTITY_VALUE_DISPLAY(entity,type,prop)                  \
  (type *) &((clutter_property_get_value_display ((entity)->properties[CLUTTER_PROP_##prop]))->data.v_##type)

struct _ClutterEntity
{
  GInitiallyUnowned parent;

  ClutterEntityPrivate *priv;

  ClutterProperty *properties[CLUTTER_PROP_LAST_PROPERTY];

  ClutterComponent *components[10];
  guint n_components;

  ClutterStage *stage;
};

struct _ClutterEntityClass
{
  GInitiallyUnownedClass parent_class;
};

void clutter_entity_initialize_components (ClutterEntity *entity);

void clutter_entity_draw (ClutterEntity *entity);

#endif /* __CLUTTER_ENTITY_PRIVATE_H__ */
