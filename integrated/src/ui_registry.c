#include "ui_registry.h"
#include <glib.h>

/* Simple registry storing GtkBuilder* in a GList */
static GList *builders = NULL;

void ui_registry_add_builder(GtkBuilder *builder)
{
    if (builder)
    {
        builders = g_list_append(builders, g_object_ref(builder));
    }
}

GtkWidget* ui_get_object_by_name(const char *name)
{
    GList *l;
    for (l = builders; l != NULL; l = l->next)
    {
        GtkBuilder *b = GTK_BUILDER(l->data);
        GObject *obj = gtk_builder_get_object(b, name);
        if (obj)
            return GTK_WIDGET(obj);
    }
    return NULL;
}

void ui_registry_clear(void)
{
    GList *l;
    for (l = builders; l != NULL; l = l->next)
    {
        g_object_unref(l->data);
    }
    g_list_free(builders);
    builders = NULL;
}
