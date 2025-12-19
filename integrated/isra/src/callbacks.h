#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

// ---- Radio buttons ----
void on_radioFemale_toggled (GtkToggleButton *togglebutton, gpointer user_data);
void on_radioMale_toggled (GtkToggleButton *togglebutton, gpointer user_data);

// ---- Button Add/Edit/Delete/Search ----
void on_buttonAdd_clicked    (GtkButton *button, gpointer user_data);
void on_buttonEdit_clicked   (GtkButton *button, gpointer user_data);
void on_buttonDelete_clicked (GtkButton *button, gpointer user_data);
void on_buttonSearch_clicked (GtkButton *button, gpointer user_data);

// ---- Private Coach Request ----
void on_buttonPCR_clicked       (GtkButton *button, gpointer user_data);
void on_buttonPrivateCoach_clicked (GtkButton *button, gpointer user_data);

// ---- Show Stats ----
void on_ButtonStats_clicked    (GtkButton *button, gpointer user_data);

// ---- Join Course ----
void on_buttonJoinCourse_clicked (GtkButton *button, gpointer user_data);
void on_buttonJC_clicked         (GtkButton *button, gpointer user_data);

// ---- Combo boxes ----
void on_comboSub_changed       (GtkComboBox *combobox, gpointer user_data);
void on_comboCoachName_changed (GtkComboBox *combobox, gpointer user_data);

// ---- Member Stats interface ----
void on_buttonStatsSubmit_clicked (GtkButton *button, gpointer user_data);
void on_buttonStatsCancel_clicked (GtkButton *button, gpointer user_data);

// ---- Private Coach interface ----
void on_buttonCoachSubmit_clicked (GtkButton *button, gpointer user_data);
void on_buttonCoachCancel_clicked (GtkButton *button, gpointer user_data);

// ---- Coach performance radio buttons ----
void on_radiobtnBad_toggled       (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobtnGood_toggled      (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobtnExcellent_toggled (GtkToggleButton *togglebutton, gpointer user_data);

// ---- Functions used in callbacks ----
void handle_private_coach_request(const char *filename, char *member_id);
void handle_join_course(const char *filename, char *member_id);
void handle_show_stats(const char *filename, char *member_id);
GtkWidget* create_Member_stats(void);
void on_buttonStatsSave_clicked(GtkButton *button, gpointer user_data);

char* get_textview_text(GtkWidget *tv);


#endif // CALLBACKS_H





