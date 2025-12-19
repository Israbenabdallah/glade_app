#ifndef UI_REGISTRY_H
#define UI_REGISTRY_H

#include <gtk/gtk.h>

/* Register a GtkBuilder so that lookup_widget() can find widgets by name */
void ui_registry_add_builder(GtkBuilder *builder);

/* Find a widget by name in any registered builder (returns GtkWidget* or NULL) */
GtkWidget* ui_get_object_by_name(const char *name);

/* Clear registry and unref builders (call at exit) */
void ui_registry_clear(void);

#endif /* UI_REGISTRY_H */
