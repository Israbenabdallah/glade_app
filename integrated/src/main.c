/* integrated main: charge les interfaces Glade d'Emna et Isra via GtkBuilder
 * Montre les fenêtres principales pour vérifier l'intégration initiale.
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include "ui_registry.h"

static GtkBuilder *builder_from_file(const char *filename, GError **err)
{
    GtkBuilder *builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, filename, err))
    {
        g_object_unref(builder);
        return NULL;
    }
    return builder;
}

int main(int argc, char *argv[])
{
    /* Set application identity for Window Manager */
    g_set_prgname("integrated_app");
    g_set_application_name("Gym Management System");
    gdk_set_program_class("GymManager");

    gtk_init(&argc, &argv);
    
    /* Load and resize app icon to avoid 'icons too large' warning */
    GdkPixbuf *app_icon = gdk_pixbuf_new_from_file_at_scale("app_icon.png", 64, 64, TRUE, NULL);
    if (app_icon) {
        gtk_window_set_default_icon(app_icon);
        g_object_unref(app_icon);
    }

    GError *err = NULL;

    /* Paths: both Glade files are copied into `integrated/ui/`.
     * Rather than using two separate GtkBuilder instances we create one builder
     * and add both files into it; this merges their definitions into a single
     * runtime builder so callbacks can lookup widgets across both UIs.
     */
    const char *merged = "ui/merged.ui";

    GtkBuilder *builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, merged, &err))
    {
        g_printerr("Failed to load %s: %s\n", merged, err ? err->message : "(no error)");
        g_clear_error(&err);
    }
    else
    {
        ui_registry_add_builder(builder);
        gtk_builder_connect_signals(builder, NULL);

        /* initialize dashboard embedding of pages */

        /* Show only the requested interface. Usage: ./integrated_app [emna|isra|both|stats]
         * - "emna" (default) : show coach management window
         * - "isra" or "member": show member management window
         * - "stats": show member statistics window
         * - "both": show both main windows (backwards compatibility)
         */
        const char *which = (argc > 1) ? argv[1] : "dashboard";
        /* If user explicitly asks for dashboard, show it; otherwise map args to windows */
        if (g_ascii_strcasecmp(which, "dashboard") == 0)
        {
            dashboard_init_dynamic();
        }
        else
        if (g_ascii_strcasecmp(which, "both") == 0)
        {
            GtkWidget *w_coach = GTK_WIDGET(gtk_builder_get_object(builder, "window_coach"));
            if (w_coach)
                gtk_widget_show_all(w_coach);
            GtkWidget *w_member = GTK_WIDGET(gtk_builder_get_object(builder, "Member Management"));
            if (w_member)
                gtk_widget_show_all(w_member);
        }
        else if (g_ascii_strcasecmp(which, "isra") == 0 || g_ascii_strcasecmp(which, "member") == 0 || g_ascii_strcasecmp(which, "members") == 0)
        {
            GtkWidget *w_member = GTK_WIDGET(gtk_builder_get_object(builder, "Member Management"));
            if (w_member)
                gtk_widget_show_all(w_member);
        }
        else if (g_ascii_strcasecmp(which, "stats") == 0 || g_ascii_strcasecmp(which, "memberstats") == 0)
        {
            GtkWidget *w_mstats = GTK_WIDGET(gtk_builder_get_object(builder, "Member Statistics"));
            if (w_mstats)
                gtk_widget_show_all(w_mstats);
        }
        else if (g_ascii_strcasecmp(which, "coachstats") == 0)
        {
            /* Call the callback directly to calculate stats and show window */
            /* Call the callback directly to calculate stats and show window */
            extern void on_dashboard_button_coach_stats_clicked(GtkButton *b, gpointer data);
            on_dashboard_button_coach_stats_clicked(NULL, NULL);
        }
        else
        {
            /* default fallback: show coach UI */
            GtkWidget *w_coach = GTK_WIDGET(gtk_builder_get_object(builder, "window_coach"));
            if (w_coach)
                gtk_widget_show_all(w_coach);
        }
    }

    /* expose a simple lookup_widget function via a static closure visible to this file and future callback units */

    gtk_main();

    if (builder) g_object_unref(builder);

    /* clear registry */
    ui_registry_clear();

    return 0;
}
