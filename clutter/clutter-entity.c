#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "clutter-component-private.h"
#include "clutter-entity-private.h"
#include "clutter-marshal.h"
#include "clutter-property-private.h"
#include "clutter-private.h"

G_DEFINE_TYPE (ClutterEntity, clutter_entity, G_TYPE_INITIALLY_UNOWNED)

#define ENTITY_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), CLUTTER_TYPE_ENTITY, ClutterEntityPrivate))


#define ARRAY_SIZE(array) (sizeof (array) / sizeof ((array)[0]))

enum
{
  PROPERTY_ADDED,
  PROPERTY_REMOVED,

  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0, };


static void
clutter_entity_get_property (GObject    *object,
                             guint       property_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
clutter_entity_set_property (GObject      *object,
                             guint         property_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
clutter_entity_dispose (GObject *object)
{
  G_OBJECT_CLASS (clutter_entity_parent_class)->dispose (object);
}

static void
clutter_entity_finalize (GObject *object)
{
  G_OBJECT_CLASS (clutter_entity_parent_class)->finalize (object);
}

static void
clutter_entity_class_init (ClutterEntityClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = clutter_entity_get_property;
  object_class->set_property = clutter_entity_set_property;
  object_class->dispose = clutter_entity_dispose;
  object_class->finalize = clutter_entity_finalize;

  signals[PROPERTY_ADDED] =
    g_signal_new (I_("property-added"),
                  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  _clutter_marshal_VOID__BOXED,
                  G_TYPE_NONE, 1,
                  CLUTTER_TYPE_PROPERTY);

  signals[PROPERTY_REMOVED] =
    g_signal_new (I_("property-removed"),
                  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  _clutter_marshal_VOID__BOXED,
                  G_TYPE_NONE, 1,
                  CLUTTER_TYPE_PROPERTY);

}

static void
clutter_entity_init (ClutterEntity *self)
{
  CLUTTER_ENTITY_PROPERTY (self, x) = clutter_property_float_new (CLUTTER_PROP (x),
                                                                  -G_MAXFLOAT,
                                                                  G_MAXFLOAT,
                                                                  0);
  CLUTTER_ENTITY_PROPERTY (self, x)->entity = self;

  CLUTTER_ENTITY_PROPERTY (self, y) = clutter_property_float_new (CLUTTER_PROP (y),
                                                                  -G_MAXFLOAT,
                                                                  G_MAXFLOAT,
                                                                  0);
  CLUTTER_ENTITY_PROPERTY (self, y)->entity = self;

  CLUTTER_ENTITY_PROPERTY (self, z) = clutter_property_float_new (CLUTTER_PROP (z),
                                                                  -G_MAXFLOAT,
                                                                  G_MAXFLOAT,
                                                                  0);
  CLUTTER_ENTITY_PROPERTY (self, z)->entity = self;
}

const ClutterValue *
clutter_entity_get_value_read (ClutterEntity *entity, guint prop_n)
{
  ClutterProperty *prop;

  g_return_val_if_fail (CLUTTER_IS_ENTITY (entity), NULL);
  g_return_val_if_fail (prop_n < CLUTTER_PROP_LAST_PROPERTY, NULL);

  prop = entity->properties[prop_n];

  if (prop == NULL)
    return NULL;

  return clutter_property_get_value_read (prop);
}

ClutterValue *
clutter_entity_get_value_write (ClutterEntity *entity, guint prop_n)
{
  ClutterProperty *prop;

  g_return_val_if_fail (CLUTTER_IS_ENTITY (entity), NULL);
  g_return_val_if_fail (prop_n < CLUTTER_PROP_LAST_PROPERTY, NULL);

  prop = entity->properties[prop_n];

  if (prop == NULL)
    return NULL;

  return clutter_property_get_value_write (prop);
}

void
clutter_entity_initialize_components (ClutterEntity *entity)
{
  gint i;

  for (i = 0; i < ARRAY_SIZE (entity->components); i++)
    {
      ClutterComponent *component = entity->components[i];
      if (component != NULL)
        {
          CLUTTER_COMPONENT_GET_CLASS (component)->init (component, entity);
        }
    }
}

void
clutter_entity_draw (ClutterEntity *self)
{
  gint i;

  for (i = 0; i < ARRAY_SIZE (self->components); i++)
    {
      ClutterComponent *component = self->components[i];

      if (component == NULL)
        break;

      cogl_push_matrix ();

      cogl_translate (*CLUTTER_ENTITY_VALUE_DISPLAY (self, float, x),
                      *CLUTTER_ENTITY_VALUE_DISPLAY (self, float, y),
                      *CLUTTER_ENTITY_VALUE_DISPLAY (self, float, z));

      CLUTTER_COMPONENT_GET_CLASS (component)->draw (component, self);

      cogl_pop_matrix ();
    }
}

/* public API */

/* TODO_LIONEL: documentation FFS! */

ClutterEntity *
clutter_entity_new (void)
{
  return g_object_new (CLUTTER_TYPE_ENTITY, NULL);
}

void
clutter_entity_append_component (ClutterEntity *entity,
                                 ClutterComponent *component)
{
  gint i;

  g_return_if_fail (CLUTTER_IS_ENTITY (entity));
  g_return_if_fail (CLUTTER_IS_COMPONENT (component));

  entity->components[entity->n_components++] = component;

  /* Insert component's properties into entity */
  i = 0;
  while (component->properties[i] != NULL)
    {
      entity->properties[component->properties[i]->prop_n] =
        component->properties[i];
      i++;
    }
}


void
clutter_entity_set_position (ClutterEntity *self,
                             gfloat x,
                             gfloat y,
                             gfloat z)
{
  g_return_if_fail (CLUTTER_IS_ENTITY (self));

  if (*CLUTTER_ENTITY_VALUE_READ (self, float, x) != x)
    *CLUTTER_ENTITY_VALUE_WRITE (self, float, x) = x;

  if (*CLUTTER_ENTITY_VALUE_READ (self, float, y) != y)
    *CLUTTER_ENTITY_VALUE_WRITE (self, float, y) = y;

  if (*CLUTTER_ENTITY_VALUE_READ (self, float, z) != z)
    *CLUTTER_ENTITY_VALUE_WRITE (self, float, z) = z;
}

void
clutter_entity_get_position (ClutterEntity *self,
                             gfloat *p_x,
                             gfloat *p_y,
                             gfloat *p_z)
{
  g_return_if_fail (CLUTTER_IS_ENTITY (self));

  if (p_x)
    *p_x = *CLUTTER_ENTITY_VALUE_READ (self, float, x);
  if (p_y)
    *p_y = *CLUTTER_ENTITY_VALUE_READ (self, float, y);
  if (p_z)
    *p_z = *CLUTTER_ENTITY_VALUE_READ (self, float, z);
}

void
clutter_entity_move_by (ClutterEntity *self,
                        gfloat dx,
                        gfloat dy,
                        gfloat dz)
{
  g_return_if_fail (CLUTTER_IS_ENTITY (self));

  if (dx != 0)
    *CLUTTER_ENTITY_VALUE_WRITE (self, float, x) += dx;

  if (dy != 0)
    *CLUTTER_ENTITY_VALUE_WRITE (self, float, y) += dy;

  if (dz != 0)
    *CLUTTER_ENTITY_VALUE_WRITE (self, float, z) += dz;
}

ClutterStage *
clutter_entity_get_stage (ClutterEntity *entity)
{
  g_return_val_if_fail (CLUTTER_IS_ENTITY (entity), NULL);

  return entity->stage;
}

