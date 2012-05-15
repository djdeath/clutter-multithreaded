#ifndef __CLUTTER_SETTINGS_PRIVATE_H__
#define __CLUTTER_SETTINGS_PRIVATE_H__

#include <clutter/clutter-backend-private.h>
#include <clutter/clutter-settings.h>

G_BEGIN_DECLS

typedef struct _ClutterSettingsValue ClutterSettingsValue;

struct _ClutterSettingsValue
{
  const gchar *property;
  GValue gvalue;
};

void    _clutter_settings_set_backend           (ClutterSettings *settings,
                                                 ClutterBackend  *backend);
void    _clutter_settings_read_from_key_file    (ClutterSettings *settings,
                                                 GKeyFile        *key_file);

ClutterSettingsValue *_clutter_settings_value_new (void);
void _clutter_settings_queue_update (ClutterSettings *settings, ClutterSettingsValue *value);
void _clutter_settings_queue_process (ClutterSettings *settings);

G_END_DECLS

#endif /* __CLUTTER_SETTINGS_PRIVATE_H__ */
