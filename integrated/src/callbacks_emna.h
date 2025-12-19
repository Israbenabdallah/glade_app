#ifndef CALLBACKS_EMNA_H
#define CALLBACKS_EMNA_H

#include <gtk/gtk.h>

/* ================= Fenêtre principale ================= */
void
on_window_coach_destroy        (GtkWidget *widget,
                                gpointer   user_data);

void
on_window_coach_show           (GtkWidget *widget,
                                gpointer   user_data);

/* ============== Boutons Coach Management =============== */
void
on_button_add_clicked          (GtkButton *button,
                                gpointer   user_data);

void
on_button_edit_clicked         (GtkButton *button,
                                gpointer   user_data);

void
on_button_delete_clicked       (GtkButton *button,
                                gpointer   user_data);

void
on_button_search_clicked       (GtkButton *button,
                                gpointer   user_data);

void
on_button_view_stats_clicked   (GtkButton *button,
                                gpointer   user_data);

/* ============== Autres signaux optionnels ============== */
void
on_combo_specialite_changed    (GtkComboBox *combobox,
                                gpointer     user_data);

void
on_radio_male_toggled          (GtkToggleButton *togglebutton,
                                gpointer         user_data);

void
on_radio_female_toggled        (GtkToggleButton *togglebutton,
                                gpointer         user_data);

#endif /* CALLBACKS_H */
