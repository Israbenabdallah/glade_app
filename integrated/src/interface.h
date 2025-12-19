#ifndef INTERFACE_H
#define INTERFACE_H

/* Minimal placeholder for Glade-generated interface declarations.
 * In this integrated project we use GtkBuilder to load the UI, so
 * these create_* functions are not required. This header exists so
 * legacy callback files that include `interface.h` can compile.
 */

#include <gtk/gtk.h>

/* Example prototypes (not implemented here):
 * GtkWidget* create_connexion(void);
 * GtkWidget* create_Espace_Admin(void);
 */

void dashboard_init_dynamic(void);
void on_nav_button_clicked(GtkWidget *widget, gpointer user_data);
void on_treeview_members_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);

#endif /* INTERFACE_H */
