#include <gtk/gtk.h>
#include <string.h>
#include "support.h"
#include "callbacks.h"
#include "Member.h"
#include "memberstats.h"

int selected_gender = 0;   // 0 = Female, 1 = Male

// ======= RADIO BUTTONS =======
void
on_radioFemale_toggled(GtkToggleButton *togglebutton,
                       gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        selected_gender = 0;
    }
}

void
on_radioMale_toggled(GtkToggleButton *togglebutton,
                     gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        selected_gender = 1;
    }
}


// ======= BUTTON "ADD" =======
void
on_buttonAdd_clicked(GtkButton *button,
                     gpointer user_data)
{
    Member m;

    // --- GET WIDGETS ---
    GtkWidget *id    = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    GtkWidget *first = lookup_widget(GTK_WIDGET(button), "entryFNM");
    GtkWidget *last  = lookup_widget(GTK_WIDGET(button), "entryLNM");
    GtkWidget *age   = lookup_widget(GTK_WIDGET(button), "spinAge");
    GtkWidget *phone = lookup_widget(GTK_WIDGET(button), "entryPhone");
    GtkWidget *email = lookup_widget(GTK_WIDGET(button), "entryEmail");
    GtkWidget *sub   = lookup_widget(GTK_WIDGET(button), "comboSub");

    // --- GET VALUES ---
    strcpy(m.id_member, gtk_entry_get_text(GTK_ENTRY(id)));
    strcpy(m.first_name, gtk_entry_get_text(GTK_ENTRY(first)));
    strcpy(m.last_name, gtk_entry_get_text(GTK_ENTRY(last)));

    m.age = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(age));

    m.gender = selected_gender;

    strcpy(m.phone, gtk_entry_get_text(GTK_ENTRY(phone)));
    strcpy(m.email, gtk_entry_get_text(GTK_ENTRY(email)));

   gchar *sub_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(sub));
if (sub_text != NULL) {
    strcpy(m.subscription_type, sub_text);
    g_free(sub_text);
} else {
    strcpy(m.subscription_type, "");   // or default
}


    // Default values
    m.private_coach = 0;
    m.request_show_stats = 0;
    m.join_course = 0;

    // --- SAVE ---
    int r = ajouter_member("members.txt", m);

    // --- POPUP ---
    GtkWidget *msg;
    if (r == 1)
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK, "Member added successfully");
    else
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                     GTK_BUTTONS_OK, "Error: cannot add member");

    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}




// ======= BUTTON "EDIT" =======
void
on_buttonEdit_clicked(GtkButton *button,
                      gpointer user_data)
{
    Member m;
    Member nouv;

    // --- GET WIDGETS ---
    GtkWidget *id    = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    GtkWidget *first = lookup_widget(GTK_WIDGET(button), "entryFNM");
    GtkWidget *last  = lookup_widget(GTK_WIDGET(button), "entryLNM");
    GtkWidget *age   = lookup_widget(GTK_WIDGET(button), "spinAge");
    GtkWidget *phone = lookup_widget(GTK_WIDGET(button), "entryPhone");
    GtkWidget *email = lookup_widget(GTK_WIDGET(button), "entryEmail");
    GtkWidget *sub   = lookup_widget(GTK_WIDGET(button), "comboSub");

    // --- GET ID TO SEARCH ---
    const char *search_id = gtk_entry_get_text(GTK_ENTRY(id));
    
    // --- SEARCH EXISTING MEMBER ---
    m = chercher_member("members.txt", (char*)search_id);
    
    if (strcmp(m.id_member, "") == 0) {
        GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK, "Member not found");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    // --- GET NEW VALUES ---
    strcpy(nouv.id_member, gtk_entry_get_text(GTK_ENTRY(id)));
    strcpy(nouv.first_name, gtk_entry_get_text(GTK_ENTRY(first)));
    strcpy(nouv.last_name, gtk_entry_get_text(GTK_ENTRY(last)));

    nouv.age = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(age));
    nouv.gender = selected_gender;

    strcpy(nouv.phone, gtk_entry_get_text(GTK_ENTRY(phone)));
    strcpy(nouv.email, gtk_entry_get_text(GTK_ENTRY(email)));

    gchar *sub_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(sub));
if (sub_text != NULL) {
    strcpy(nouv.subscription_type, sub_text);
    g_free(sub_text);
} else {
    strcpy(nouv.subscription_type, "");
}


    // Keep existing flags
    nouv.private_coach = m.private_coach;
    nouv.request_show_stats = m.request_show_stats;
    nouv.join_course = m.join_course;

    // --- UPDATE ---
    int r = modifier_member("members.txt", (char*)search_id, nouv);

    // --- POPUP ---
    GtkWidget *msg;
    if (r == 1)
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK, "Member updated successfully");
    else
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                     GTK_BUTTONS_OK, "Error: cannot update member");

    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}





// ======= BUTTON "DELETE" =======
void
on_buttonDelete_clicked(GtkButton *button,
                        gpointer user_data)
{
    // --- GET WIDGET ---
    GtkWidget *id = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    const char *member_id = gtk_entry_get_text(GTK_ENTRY(id));

    if (strlen(member_id) == 0) {
        GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK, "Please enter a member ID");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    // --- CONFIRM DELETION ---
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION,
                                              GTK_BUTTONS_YES_NO, "Delete this member?");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response != GTK_RESPONSE_YES)
        return;

    // --- DELETE ---
    int r = supprimer_member("members.txt", (char*)member_id);

    // --- POPUP ---
    GtkWidget *msg;
    if (r == 1) {
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK, "Member deleted successfully");
        // Clear form fields after deletion
        gtk_entry_set_text(GTK_ENTRY(id), "");
        GtkWidget *first = lookup_widget(GTK_WIDGET(button), "entryFNM");
        GtkWidget *last  = lookup_widget(GTK_WIDGET(button), "entryLNM");
        GtkWidget *phone = lookup_widget(GTK_WIDGET(button), "entryPhone");
        GtkWidget *email = lookup_widget(GTK_WIDGET(button), "entryEmail");
        gtk_entry_set_text(GTK_ENTRY(first), "");
        gtk_entry_set_text(GTK_ENTRY(last), "");
        gtk_entry_set_text(GTK_ENTRY(phone), "");
        gtk_entry_set_text(GTK_ENTRY(email), "");
    }
    else
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                     GTK_BUTTONS_OK, "Error: cannot delete member");

    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}


// ======= BUTTON "SEARCH" =======
void
on_buttonSearch_clicked(GtkButton *button,
                        gpointer user_data)
{
    // --- GET WIDGET ---
    GtkWidget *id = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    const char *search_id = gtk_entry_get_text(GTK_ENTRY(id));

    // --- SEARCH MEMBER ---
    Member m = chercher_member("members.txt", (char*)search_id);

    if (strcmp(m.id_member, "") == 0) {
        GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK, "Member not found");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    // --- POPULATE FORM WITH FOUND DATA ---
    GtkWidget *first = lookup_widget(GTK_WIDGET(button), "entryFNM");
    GtkWidget *last  = lookup_widget(GTK_WIDGET(button), "entryLNM");
    GtkWidget *age   = lookup_widget(GTK_WIDGET(button), "spinAge");
    GtkWidget *phone = lookup_widget(GTK_WIDGET(button), "entryPhone");
    GtkWidget *email = lookup_widget(GTK_WIDGET(button), "entryEmail");
    GtkWidget *sub   = lookup_widget(GTK_WIDGET(button), "comboSub");

    gtk_entry_set_text(GTK_ENTRY(first), m.first_name);
    gtk_entry_set_text(GTK_ENTRY(last), m.last_name);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(age), m.age);
    gtk_entry_set_text(GTK_ENTRY(phone), m.phone);
    gtk_entry_set_text(GTK_ENTRY(email), m.email);
    
    gint n_items = gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(sub)), NULL);
    for (gint i = 0; i < n_items; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(sub), 0);
    }
    gtk_combo_box_append_text(GTK_COMBO_BOX(sub), m.subscription_type);
    gtk_combo_box_set_active(GTK_COMBO_BOX(sub), 0);

    // Set gender radio buttons
    if (m.gender == 0) {
        GtkWidget *radio_female = lookup_widget(GTK_WIDGET(button), "radioFemale");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_female), TRUE);
    } else {
        GtkWidget *radio_male = lookup_widget(GTK_WIDGET(button), "radioMale");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_male), TRUE);
    }

    GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK, "Member found and loaded");
    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}


// ======= BUTTON "PRIVATE COACH REQUEST" =======
void
on_buttonPCR_clicked(GtkButton *button,
                     gpointer user_data)
{
    // --- GET MEMBER ID ---
    GtkWidget *id = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    const char *member_id = gtk_entry_get_text(GTK_ENTRY(id));

    if (strlen(member_id) == 0) {
        GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK, "Please select a member first");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    // --- RECORD EVENT ---
    handle_private_coach_request("members.txt", (char*)member_id);

    GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK, "Private coach request submitted");
    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}


// ======= BUTTON "SHOW STATS" =======
//void
//on_ButtonStats_clicked(GtkButton *button,
                       //gpointer user_data)
/*{
    // --- GET MEMBER ID ---
    GtkWidget *id = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    const char *member_id = gtk_entry_get_text(GTK_ENTRY(id));

    if (strlen(member_id) == 0) {
        GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK, "Please select a member first");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    // --- RECORD EVENT ---
    handle_show_stats("members.txt", (char*)member_id);

    GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK, "Member stats will be displayed");
    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}*/


// ======= BUTTON "JOIN COURSE" =======
void
on_buttonJoinCourse_clicked(GtkButton *button,
                            gpointer user_data)
{
    // --- GET MEMBER ID ---
    GtkWidget *id = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    const char *member_id = gtk_entry_get_text(GTK_ENTRY(id));

    if (strlen(member_id) == 0) {
        GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK, "Please select a member first");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    // --- RECORD EVENT ---
    handle_join_course("members.txt", (char*)member_id);

    GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK, "Member enrolled in course");
    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}

void
on_buttonJC_clicked(GtkButton *button,
                    gpointer user_data)
{
    on_buttonJoinCourse_clicked(button, user_data);
}

// ======= COMBO BOX SUBSCRIPTION CHANGE HANDLER =======
void
on_comboSub_changed(GtkComboBox *combobox,
                    gpointer user_data)
{
    gchar *active_text = gtk_combo_box_get_active_text(combobox);
    if (active_text) {
        // Subscription type changed - can add logic here if needed
        g_free(active_text);
    }
}
// Combo box change
void on_comboCoachName_changed(GtkComboBox *combobox, gpointer user_data)
{
    gchar *active_text = gtk_combo_box_get_active_text(combobox);
    if (active_text) {
        // Add logic here if needed
        g_free(active_text);
    }
}

// Radio buttons for performance
void on_radiobtnBad_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Add logic if needed
}

void on_radiobtnGood_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Add logic if needed
}

void on_radiobtnExcellent_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Add logic if needed
}



// ================= Member Stats | callbacks.c ===================


char* get_textview_text(GtkWidget *tv)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
    GtkTextIter start, end;

    gtk_text_buffer_get_bounds(buffer, &start, &end);

    // gtk_text_buffer_get_text() alloue une nouvelle chaîne → à free() après usage
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}


// === GLOBAL POINTERS TO WIDGETS ===
GtkWidget *tv_num_attended;
GtkWidget *tv_fav_class;
GtkWidget *cb_coach_name;
GtkWidget *rb_bad;
GtkWidget *rb_good;
GtkWidget *rb_excellent;
GtkWidget *tv_consistency;

// === Function to fill the stats interface ===
void load_stats_into_widgets(MemberStats stats)
{
    // Convert integers to strings
    char buffer[50];
    
    // Number of classes attended
    sprintf(buffer, "%d", stats.num_classes_attended);
    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_num_attended)),
        buffer,
        -1
    );

    // Favorite class
    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_fav_class)),
        stats.favorite_class,
        -1
    );

    // Coach name (combobox)
  GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(cb_coach_name));
GtkTreeIter iter;
gboolean valid;
gchar *coach_name;

valid = gtk_tree_model_get_iter_first(model, &iter);
int index = 0;
int found_index = 0;

while (valid) {
    gtk_tree_model_get(model, &iter, 0, &coach_name, -1);
    if (strcmp(coach_name, stats.coach_name) == 0) {
        found_index = index;
        g_free(coach_name);
        break;
    }
    g_free(coach_name);
    valid = gtk_tree_model_iter_next(model, &iter);
    index++;
}

gtk_combo_box_set_active(GTK_COMBO_BOX(cb_coach_name), found_index);


    // Coach performance (radio buttons)
    if(stats.performance == 1)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_bad), TRUE);
    else if(stats.performance == 2)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_good), TRUE);
    else if(stats.performance == 3)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_excellent), TRUE);

    // Consistency score
    sprintf(buffer, "%d / 10", stats.consistency_score);
    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_consistency)),
        buffer,
        -1
    );
}


// === When clicking "Show stats" in Member Management ===
void on_ButtonStats_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *stats_window = create_Member_stats();  // glade window
    gtk_widget_show_all(stats_window);

    // Retrieve the widgets in this window
    tv_num_attended = lookup_widget(stats_window, "textviewNumClasses");
    tv_fav_class    = lookup_widget(stats_window, "textviewFavClass");
    cb_coach_name   = lookup_widget(stats_window, "comboCoachName");

    rb_bad          = lookup_widget(stats_window, "radiobtnBad");
    rb_good         = lookup_widget(stats_window, "radiobtnGood");
    rb_excellent    = lookup_widget(stats_window, "radiobtnExcellent");

    tv_consistency  = lookup_widget(stats_window, "textviewConsistency");

    // Load the stats of the current logged/selected member
    GtkWidget *id_widget = lookup_widget(GTK_WIDGET(button), "entryIdMember");
const char *current_member_id = gtk_entry_get_text(GTK_ENTRY(id_widget));

MemberStats ms = load_member_stats("memberstats.txt", (char*)current_member_id);


    load_stats_into_widgets(ms);
} void on_buttonStatsSave_clicked(GtkButton *button, gpointer user_data)
{
    const char *num_attended_str = get_textview_text(tv_num_attended);
    const char *fav_class       = get_textview_text(tv_fav_class);
    const char *coach_name      = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(cb_coach_name));
    const char *consistency_str = get_textview_text(tv_consistency);

    int num_attended = atoi(num_attended_str);
    int consistency  = atoi(consistency_str);

    int performance;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(rb_bad)))
        performance = 1;
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(rb_good)))
        performance = 2;
    else
        performance = 3;

    GtkWidget *id_widget = lookup_widget(GTK_WIDGET(button), "entryIdStats");
    const char *member_id = gtk_entry_get_text(GTK_ENTRY(id_widget));

    MemberStats ms = load_member_stats("memberstats.txt", (char*)member_id);

    ms.num_classes_attended = num_attended;
    strcpy(ms.favorite_class, fav_class);
    strcpy(ms.coach_name, coach_name);
    ms.performance = performance;
    ms.consistency_score = consistency;

    save_member_stats("memberstats.txt", member_id, ms);

    g_print("Stats updated for member %s\n", member_id);
}





