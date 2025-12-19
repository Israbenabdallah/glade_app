/* Dashboard navigation handlers: show/hide different management windows.
 * The dashboard buttons will open the respective windows.
 */

#include <gtk/gtk.h>
#include <string.h>
#include "ui_registry.h"

static GtkWidget* g_notebook = NULL;

/* Initialize dashboard: Reparent views into the notebook */
void dashboard_init_dynamic(void)
{
    GtkWidget *win_main = ui_get_object_by_name("window_main");
    GtkWidget *notebook = ui_get_object_by_name("notebook_content");
    
    if (!notebook || !win_main) {
        g_warning("Dashboard Notebook or Window not found!");
        return;
    }
    g_notebook = notebook; 

    /* Helper macro to move content */
    /* Remove from dummy parent (window) and add to notebook */
    void move_block(const char* win_id, const char* vbox_id) {
        GtkWidget *win = ui_get_object_by_name(win_id);
        GtkWidget *vbox = ui_get_object_by_name(vbox_id);
        if (win && vbox) {
            gtk_widget_reparent(vbox, notebook);
        } else {
            g_warning("Failed to reparent: %s / %s", win_id, vbox_id);
        }
    }
    
    /* Clear dummy pages from XML */
    int n_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
    for (int i = 0; i < n_pages; i++) {
        gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), 0);
    }

    /* CREATE HOME PAGE (Page 0) */
    GtkWidget *vbox_home = gtk_vbox_new(FALSE, 20);
    gtk_container_set_border_width(GTK_CONTAINER(vbox_home), 40);
    
    GtkWidget *center_box = gtk_vbox_new(FALSE, 20);
    
    /* Load and resize custom app icon to 200x200 to act as a proper logo */
    GtkWidget *img;
    GError *pix_err = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("app_icon.png", 200, 200, TRUE, &pix_err);
    
    if (pixbuf) {
        img = gtk_image_new_from_pixbuf(pixbuf);
        g_object_unref(pixbuf);
    } else {
        /* Fallback to stock home icon if file missing */
        if (pix_err) g_error_free(pix_err);
        img = gtk_image_new_from_stock(GTK_STOCK_HOME, GTK_ICON_SIZE_DIALOG);
    }

    GtkWidget *lbl_welcome = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_welcome), 
        "<span size='30000' weight='bold' foreground='#2c3e50'>Welcome to Smart Gym Manager</span>\n\n"
        "<span size='x-large' foreground='#7f8c8d'>Select a module from the sidebar to begin</span>");
    gtk_label_set_justify(GTK_LABEL(lbl_welcome), GTK_JUSTIFY_CENTER);

    gtk_box_pack_start(GTK_BOX(center_box), img, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(center_box), lbl_welcome, FALSE, FALSE, 0);

    GtkWidget *align = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(align), center_box);
    gtk_box_pack_start(GTK_BOX(vbox_home), align, TRUE, TRUE, 0);
    
    gtk_widget_show_all(vbox_home);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_home, gtk_label_new("Home"));

    /* Move contents in specific order */
    /* Page 0: Member Management */
    move_block("Member Management", "vbox_main");
    
    /* Page 1: Member Stats */
    move_block("Member Statistics", "vbox_stats_main");
    
    /* Page 2: Coach Management */
    move_block("window_coach", "vbox_coach_main");
    
    /* Page 3: Coach Statistics */
    /* Page 3: Coach Statistics */
    move_block("window_stats", "vbox_coach_stats_main");
    
    /* --- INTEGRATE LISTS INTO PAGES --- */
    
    /* 1. Embed Member Cards into Member Management (Page 0) */
    GtkWidget *vbox_member = ui_get_object_by_name("vbox_main");
    GtkWidget *scrol_cards = ui_get_object_by_name("scrol_member_cards");
    
    if (scrol_cards && vbox_member) {
        GtkWidget *parent = gtk_widget_get_parent(scrol_cards);
        if (parent) gtk_container_remove(GTK_CONTAINER(parent), scrol_cards);
        
        GtkWidget *sep = gtk_hseparator_new();
        gtk_box_pack_start(GTK_BOX(vbox_member), sep, FALSE, FALSE, 10);
        
        GtkWidget *lbl = gtk_label_new("<span size='large' weight='bold'>Member Directory</span>");
        gtk_label_set_use_markup(GTK_LABEL(lbl), TRUE);
        gtk_misc_set_alignment(GTK_MISC(lbl), 0, 0.5);
        gtk_box_pack_start(GTK_BOX(vbox_member), lbl, FALSE, FALSE, 5);

        gtk_box_pack_start(GTK_BOX(vbox_member), scrol_cards, TRUE, TRUE, 0);
    }

    /* 2. Embed Coach List into Coach Management (Page 2) */
    GtkWidget *vbox_coach = ui_get_object_by_name("vbox_coach_main");
    GtkWidget *scrol_coach_list = ui_get_object_by_name("scrol_list");
    
    if (scrol_coach_list && vbox_coach) {
        GtkWidget *parent = gtk_widget_get_parent(scrol_coach_list);
        if (parent) gtk_container_remove(GTK_CONTAINER(parent), scrol_coach_list);
        
        GtkWidget *sep = gtk_hseparator_new();
        gtk_box_pack_start(GTK_BOX(vbox_coach), sep, FALSE, FALSE, 10);
        
        GtkWidget *lbl = gtk_label_new("<span size='large' weight='bold'>Coach Directory</span>");
        gtk_label_set_use_markup(GTK_LABEL(lbl), TRUE);
        gtk_misc_set_alignment(GTK_MISC(lbl), 0, 0.5);
        gtk_box_pack_start(GTK_BOX(vbox_coach), lbl, FALSE, FALSE, 5);

        gtk_box_pack_start(GTK_BOX(vbox_coach), scrol_coach_list, TRUE, TRUE, 0);
    }
    
    /* Set custom icon for the main window */
    GdkPixbuf *app_icon = gdk_pixbuf_new_from_file_at_scale("app_icon.png", 128, 128, TRUE, NULL);
    if (app_icon) {
        gtk_window_set_icon(GTK_WINDOW(win_main), app_icon);
        g_object_unref(app_icon);
    }

    /* Show main window maximized */
    gtk_window_maximize(GTK_WINDOW(win_main));
    gtk_widget_show_all(win_main);
}

void on_nav_button_clicked(GtkWidget *widget, gpointer user_data)
{
    /* Use direct pointer comparison which is safer than string names in GTK2 */
    
    if (!g_notebook) {
        /* Try to recover notebook if NULL (should adhere to init) */
        g_notebook = ui_get_object_by_name("notebook_content");
        if (!g_notebook) return;
    }

    /* Get the button objects from the registry to compare addresses */
    GtkWidget *btn_home = ui_get_object_by_name("btn_nav_home");
    GtkWidget *btn_mm = ui_get_object_by_name("btn_nav_member_mgmt");
    GtkWidget *btn_ms = ui_get_object_by_name("btn_nav_member_stats");
    GtkWidget *btn_cm = ui_get_object_by_name("btn_nav_coach_mgmt");
    GtkWidget *btn_cs = ui_get_object_by_name("btn_nav_coach_stats");

    int page = -1;

    if (widget == btn_home) {
        page = 0;
    } else if (widget == btn_mm) {
        page = 1;
    } else if (widget == btn_ms) {
        page = 2;
    } else if (widget == btn_cm) {
        page = 3;
    } else if (widget == btn_cs) {
        page = 4;
    }

    if (page != -1) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(g_notebook), page);
        
        /* Refresh Member List (Page 1 now) */
        if (page == 1) {
              extern void on_button_display_members_clicked(GtkButton *b, gpointer user_data);
              on_button_display_members_clicked(NULL, NULL);
        }
        
        /* Refresh Member Statistics Combo if selected (Page 2) */
        if (page == 2) {
             extern void on_Member_Statistics_show(GtkWidget *widget, gpointer user_data);
             on_Member_Statistics_show(NULL, NULL);
        }
        
        /* Refresh Coach List (Page 3) */
        if (page == 3) {
             extern void on_button_display_clicked(GtkButton *button, gpointer user_data);
             on_button_display_clicked(NULL, NULL);
        }
        
        /* Refresh Coach Statistics if selected (Page 4) */
        if (page == 4) {
             extern void on_dashboard_button_coach_stats_clicked(GtkButton *b, gpointer user_data);
             on_dashboard_button_coach_stats_clicked(NULL, NULL);
        }
    }
}

// Keep old handlers empty or redirections just in case to avoid link errors
void on_dashboard_button_coach_clicked(GtkButton *b, gpointer user_data) {}
void on_dashboard_button_members_clicked(GtkButton *b, gpointer user_data) {}
void on_dashboard_button_member_stats_clicked(GtkButton *b, gpointer user_data) {}
void on_dashboard_quit_clicked(GtkButton *b, gpointer user_data) { gtk_main_quit(); }
