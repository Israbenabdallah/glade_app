/* glade_shims.c
 * Small compatibility shims for handlers referenced in the merged Glade file
 * but not implemented elsewhere. These are lightweight and safe; they can
 * be replaced by full implementations when desired.
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>
#include "callbacks.h"
#include "support.h"
#include "stats_graphics.h"

/* Show the Member stats window created by Glade's create_Member_stats(). */
void
on_ButtonStats_clicked(GtkButton *button, gpointer user_data)
{
    (void)button; (void)user_data;
    GtkWidget *w = create_Member_stats();
    if (w) gtk_widget_show_all(w);
}

/* Save statistics to file */
void
on_buttonStatsSave_clicked(GtkButton *button, gpointer user_data)
{
    (void)user_data;
    
    GtkWidget *parent = GTK_WIDGET(button);
    GtkWidget *w;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *text;
    
    // Récupérer les valeurs des champs
    
    // Classes Attended
    w = lookup_widget(parent, "textviewNumClasses");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("textviewNumClasses");
    }
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *num_classes = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    // Favorite Class
    w = lookup_widget(parent, "textviewFavClass");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("textviewFavClass");
    }
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *fav_class = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    // Coach Name
    w = lookup_widget(parent, "comboCoachName");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("comboCoachName");
    }
    gchar *coach_name = NULL;
    if (w && GTK_IS_COMBO_BOX(w)) {
        // Use same method as in isra_callbacks.c
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(w));
        GtkTreeIter iter;
        if (model && gtk_combo_box_get_active_iter(GTK_COMBO_BOX(w), &iter)) {
            gtk_tree_model_get(model, &iter, 0, &coach_name, -1);
        }
    }
    
    // Performance Rating
    const char *rating = "Not Rated";
    w = lookup_widget(parent, "radiobtnExcellent");
    if (w && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)))
        rating = "Excellent";
    else {
        w = lookup_widget(parent, "radiobtnGood");
        if (w && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)))
            rating = "Good";
        else {
            w = lookup_widget(parent, "radiobtnBad");
            if (w && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)))
                rating = "Bad";
        }
    }
    
    // Consistency Score
   w = lookup_widget(parent, "textviewConsistency");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("textviewConsistency");
    }
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *consistency = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    // Sauvegarder dans un fichier
    FILE *file = fopen("member_statistics.txt", "a");
    if (file) {
        // Get current timestamp
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        
        fprintf(file, "=== Member Statistics Saved ===\n");
        fprintf(file, "Date: %02d/%02d/%04d %02d:%02d:%02d\n",
                t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
                t->tm_hour, t->tm_min, t->tm_sec);
        fprintf(file, "Classes Attended: %s\n", num_classes ? num_classes : "N/A");
        fprintf(file, "Favorite Class: %s\n", fav_class ? fav_class : "N/A");
        fprintf(file, "Coach Name: %s\n", coach_name ? coach_name : "N/A");
        fprintf(file, "Performance Rating: %s\n", rating);
        fprintf(file, "Consistency Score: %s\n", consistency ? consistency : "N/A");
        fprintf(file, "================================\n\n");
        fclose(file);
        
        // Show success message
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "✓ Statistics saved successfully!\n\nFile: member_statistics.txt");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        /* Update charts with saved data */
        int classes = num_classes ? atoi(num_classes) : 0;
        int score = consistency ? atoi(consistency) : 0;
        
        /* Remove % sign if present */
        if (score == 0 && consistency) {
            char *pct = strchr(consistency, '%');
            if (pct) *pct = '\0';
            score = atoi(consistency);
        }
        
        extern void set_current_stats_data(int, int);
        extern void update_stats_charts(GtkWidget*);
        extern GtkWidget *ui_get_object_by_name(const char *name);
        
        /* Smart Advice Logic */
        GtkWidget *radio_bad = lookup_widget(parent, "radiobtnBad");
        if (!radio_bad) radio_bad = ui_get_object_by_name("radiobtnBad");
        
        GtkWidget *radio_good = lookup_widget(parent, "radiobtnGood");
        if (!radio_good) radio_good = ui_get_object_by_name("radiobtnGood");
        
        GtkWidget *radio_exc = lookup_widget(parent, "radiobtnExcellent");
        if (!radio_exc) radio_exc = ui_get_object_by_name("radiobtnExcellent");
        
        const char *advice = "Veuillez mettre à jour vos statistiques.";
        
        if (radio_bad && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_bad))) {
            advice = "Conseil : Réservez une séance privée avec votre coach.";
        } else if (radio_good && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_good))) {
            if (score < 50) {
                advice = "Conseil : Essayez de venir à heures fixes.";
            } else {
                advice = "Bonne régularité ! Continuez ainsi.";
            }
        } else if (radio_exc && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_exc))) {
            advice = "Bravo ! Vous êtes prêt pour le niveau supérieur 🚀";
        }
        
        GtkWidget *label_advice = lookup_widget(parent, "label_smart_advice");
        if (!label_advice) label_advice = ui_get_object_by_name("label_smart_advice");
        
        if (label_advice) {
            char markup[512];
            snprintf(markup, sizeof(markup), "<span weight='bold' foreground='#2c3e50'>%s</span>", advice);
            gtk_label_set_markup(GTK_LABEL(label_advice), markup);
        }
        
        set_current_stats_data(classes, score);
        update_stats_charts(parent);
        
    } else {
        // Show error message
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "✗ Error: Unable to save statistics file!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    
    // Free allocated strings
    if (num_classes) g_free(num_classes);
    if (fav_class) g_free(fav_class);
    if (coach_name) g_free(coach_name);
    if (consistency) g_free(consistency);
}

/* Export PDF Report */
void
on_buttonStatsExportPDF_clicked(GtkButton *button, gpointer user_data)
{
    (void)user_data;
    
    GtkWidget *parent = GTK_WIDGET(button);
    GtkWidget *w;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    
    /* Get form data */
    w = lookup_widget(parent, "textviewNumClasses");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("textviewNumClasses");
    }
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *num_classes = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    w = lookup_widget(parent, "textviewFavClass");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("textviewFavClass");
    }
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *fav_class = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    w = lookup_widget(parent, "comboCoachName");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("comboCoachName");
    }
    gchar *coach_name = NULL;
    if (w && GTK_IS_COMBO_BOX(w)) {
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(w));
        GtkTreeIter iter;
        if (model && gtk_combo_box_get_active_iter(GTK_COMBO_BOX(w), &iter)) {
            gtk_tree_model_get(model, &iter, 0, &coach_name, -1);
        }
    }
    
    const char *rating = "Not Rated";
    w = lookup_widget(parent, "radiobtnExcellent");
    if (w && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)))
        rating = "Excellent";
    else {
        w = lookup_widget(parent, "radiobtnGood");
        if (w && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)))
            rating = "Good";
        else {
            w = lookup_widget(parent, "radiobtnBad");
            if (w && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)))
                rating = "Bad";
        }
    }
    
    w = lookup_widget(parent, "textviewConsistency");
    if (!w) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        w = ui_get_object_by_name("textviewConsistency");
    }
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *consistency = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    /* Create PDF filename */
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char pdf_filename[256];
    snprintf(pdf_filename, sizeof(pdf_filename), 
             "member_statistics_%04d%02d%02d_%02d%02d%02d.pdf",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
    
    /* Create PDF surface (A4: 595x842 points) */
    cairo_surface_t *surface = cairo_pdf_surface_create(pdf_filename, 595, 842);
    cairo_t *cr = cairo_create(surface);
    
    /* Page 1: Statistics Summary */
    int y = 50;
    
    /* Title */
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 24);
    cairo_set_source_rgb(cr, 0.1, 0.2, 0.5);
    cairo_move_to(cr, 50, y);
    cairo_show_text(cr, "📊 MEMBER STATISTICS REPORT");
    y += 40;
    
    /* Date */
    cairo_set_font_size(cr, 12);
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    char date_str[100];
    snprintf(date_str, sizeof(date_str), "Generated: %02d/%02d/%04d %02d:%02d",
             t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
    cairo_move_to(cr, 50, y);
    cairo_show_text(cr, date_str);
    y += 40;
    
    /* Separator line */
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, 50, y);
    cairo_line_to(cr, 545, y);
    cairo_stroke(cr);
    y += 30;
    
    /* Data Section */
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_move_to(cr, 50, y);
    cairo_show_text(cr, "Activity Statistics");
    y += 30;
    
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 14);
    
    char info[256];
    snprintf(info, sizeof(info), "Classes Attended: %s", num_classes ? num_classes : "N/A");
    cairo_move_to(cr, 70, y);
    cairo_show_text(cr, info);
    y += 25;
    
    snprintf(info, sizeof(info), "Favorite Class: %s", fav_class ? fav_class : "N/A");
    cairo_move_to(cr, 70, y);
    cairo_show_text(cr, info);
    y += 40;
    
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, 50, y);
    cairo_show_text(cr, "Coach Evaluation");
    y += 30;
    
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 14);
    
    snprintf(info, sizeof(info), "Coach Name: %s", coach_name ? coach_name : "N/A");
    cairo_move_to(cr, 70, y);
    cairo_show_text(cr, info);
    y += 25;
    
    snprintf(info, sizeof(info), "Performance Rating: %s", rating);
    cairo_move_to(cr, 70, y);
    cairo_show_text(cr, info);
    y += 40;
    
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, 50, y);
    cairo_show_text(cr, "Performance Metrics");
    y += 30;
    
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 14);
    
    snprintf(info, sizeof(info), "Consistency Score: %s", consistency ? consistency : "N/A");
    cairo_move_to(cr, 70, y);
    cairo_show_text(cr, info);
    y += 60;
    
    /* Charts Section */
    y = 450;  /* Position for charts */
    
    /* Progress Curve - moved to page 1 to replace histogram */
    cairo_save(cr);
    cairo_translate(cr, 50, y);
    
    ChartData *prog_data = get_current_progress_data();
    if (prog_data && prog_data->points && prog_data->count > 0) {
        g_print("PDF: Drawing progress curve with %d points\n", prog_data->count);
        draw_progress_curve(cr, 495, 300, prog_data);
        free_chart_data(prog_data);
    } else {
        g_print("PDF: Skipping progress curve - invalid data\n");
        if (prog_data) free_chart_data(prog_data);
        
        /* Draw placeholder text */
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 14);
        cairo_move_to(cr, 100, 100);
        cairo_show_text(cr, "No progress data available");
    }
    
    cairo_restore(cr);
    
    /* Note: Histogram removed and pages merged as requested */
    
    /* Footer */
    cairo_set_font_size(cr, 10);
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_move_to(cr, 50, 800);
    cairo_show_text(cr, "Gym Management System - Member Statistics Report");
    
    /* Cleanup */
    cairo_destroy(cr);
    cairo_surface_finish(surface);
    cairo_surface_destroy(surface);
    
    /* Free strings */
    if (num_classes) g_free(num_classes);
    if (fav_class) g_free(fav_class);
    if (coach_name) g_free(coach_name);
    if (consistency) g_free(consistency);
    
    /* Success message */
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "✅ PDF Report Generated!\n\nFile: %s", pdf_filename);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
