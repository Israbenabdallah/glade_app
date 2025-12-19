/* Unified callbacks.h for integrated project
 * Declares callbacks from both the ISRA (members) and EMNA (coachs) UIs
 */
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

/* ------------------ EMNA (coach) callbacks ------------------ */
void on_window_coach_destroy        (GtkWidget *widget, gpointer user_data);
void on_window_coach_show           (GtkWidget *widget, gpointer user_data);

void on_button_add_clicked          (GtkButton *button, gpointer user_data);
void on_button_edit_clicked         (GtkButton *button, gpointer user_data);
void on_button_delete_clicked       (GtkButton *button, gpointer user_data);
void on_button_search_clicked       (GtkButton *button, gpointer user_data);
void on_dashboard_button_coach_stats_clicked (GtkButton *button, gpointer user_data);

void on_combo_specialite_changed    (GtkComboBox *combobox, gpointer user_data);
void on_radio_male_toggled          (GtkToggleButton *togglebutton, gpointer user_data);
void on_radio_female_toggled        (GtkToggleButton *togglebutton, gpointer user_data);

/* ------------------ ISRA (member) callbacks ----------------- */
void on_radioFemale_toggled (GtkToggleButton *togglebutton, gpointer user_data);
void on_radioMale_toggled   (GtkToggleButton *togglebutton, gpointer user_data);

void on_buttonAdd_clicked    (GtkButton *button, gpointer user_data);
void on_buttonEdit_clicked   (GtkButton *button, gpointer user_data);
void on_buttonDelete_clicked (GtkButton *button, gpointer user_data);
void on_buttonSearch_clicked (GtkButton *button, gpointer user_data);

void on_buttonPCR_clicked       (GtkButton *button, gpointer user_data);
void on_buttonPrivateCoach_clicked (GtkButton *button, gpointer user_data);
void on_ButtonStats_clicked    (GtkButton *button, gpointer user_data);
void on_buttonJoinCourse_clicked (GtkButton *button, gpointer user_data);
void on_buttonJC_clicked         (GtkButton *button, gpointer user_data);

void on_comboSub_changed       (GtkComboBox *combobox, gpointer user_data);
void on_comboCoachName_changed (GtkComboBox *combobox, gpointer user_data);

void on_Member_Management_show (GtkWidget *widget, gpointer user_data);
void on_Member_Statistics_show (GtkWidget *widget, gpointer user_data);

void on_buttonStatsSubmit_clicked (GtkButton *button, gpointer user_data);
void on_buttonStatsCancel_clicked (GtkButton *button, gpointer user_data);

void on_buttonCoachSubmit_clicked (GtkButton *button, gpointer user_data);
void on_buttonCoachCancel_clicked (GtkButton *button, gpointer user_data);

void on_radiobtnBad_toggled       (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobtnGood_toggled      (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobtnExcellent_toggled (GtkToggleButton *togglebutton, gpointer user_data);

/* ------------------ Shared utilities used by callbacks ---------- */
void handle_private_coach_request(const char *filename, char *member_id);
void handle_join_course(const char *filename, char *member_id);
void handle_show_stats(const char *filename, char *member_id);
GtkWidget* create_Member_stats(void);
void on_buttonStatsSave_clicked(GtkButton *button, gpointer user_data);
void on_buttonStatsExportPDF_clicked(GtkButton *button, gpointer user_data);

char* get_textview_text(GtkWidget *tv);

#endif /* CALLBACKS_H */
void on_Member_Statistics_show(GtkWidget *widget, gpointer user_data);
void on_button_display_clicked(GtkButton *button, gpointer user_data);
void on_button_display_members_clicked(GtkButton *button, gpointer user_data);
