/* Support header copied from emna project (Glade-generated) */

#ifndef SUPPORT_H
#define SUPPORT_H

#include <gtk/gtk.h>

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  define Q_(String) g_strip_context ((String), gettext (String))
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define Q_(String) g_strip_context ((String), (String))
#  define N_(String) (String)
#endif

GtkWidget*  lookup_widget              (GtkWidget       *widget,
                                        const gchar     *widget_name);

void        add_pixmap_directory       (const gchar     *directory);

GtkWidget*  create_pixmap              (GtkWidget       *widget,
                                        const gchar     *filename);

GdkPixbuf*  create_pixbuf              (const gchar     *filename);

void        glade_set_atk_action_description (AtkAction       *action,
                                              const gchar     *action_name,
                                              const gchar     *description);

#endif /* SUPPORT_H */
