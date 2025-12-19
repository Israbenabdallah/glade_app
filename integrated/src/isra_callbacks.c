#include <gtk/gtk.h>
#include <string.h>
#include "support.h"
#include "callbacks.h"
#include "Member.h"
#include "memberstats.h"
#include "entrain.h"
#include "ui_registry.h"

int selected_gender = 0;   // 0 = Female, 1 = Male

// ======= UTILITY FUNCTIONS =======
/* Vide tous les champs du formulaire Member */
static void clear_member_form(GtkWidget *parent_widget)
{
    GtkWidget *w;
    
    /* Vider les entries */
    w = lookup_widget(parent_widget, "entryIdMember");
    if (w) gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(parent_widget, "entryFNM");
    if (w) gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(parent_widget, "entryLNM");
    if (w) gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(parent_widget, "entryPhone");
    if (w) gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(parent_widget, "entryEmail");
    if (w) gtk_entry_set_text(GTK_ENTRY(w), "");
    
    /* Réinitialiser age */
    w = lookup_widget(parent_widget, "spinAge");
    if (w) gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), 18);
    
    /* Sélectionner Female par défaut */
    w = lookup_widget(parent_widget, "radioFemale");
    if (w) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
    
    /* Réinitialiser combo */
    w = lookup_widget(parent_widget, "comboSub");
    if (w) gtk_combo_box_set_active(GTK_COMBO_BOX(w), 0);
}

/* Récupère le texte actif d'un GtkComboBox (compatible GTK2) */
static gchar* get_combo_box_text(GtkComboBox *combo_box)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *text = NULL;
    
    if (!GTK_IS_COMBO_BOX(combo_box))
        return NULL;
    
    model = gtk_combo_box_get_model(combo_box);
    if (model == NULL)
        return NULL;
    
    if (gtk_combo_box_get_active_iter(combo_box, &iter))
    {
        gtk_tree_model_get(model, &iter, 0, &text, -1);
    }
    
    return text;
}

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

   gchar *sub_text = get_combo_box_text(GTK_COMBO_BOX(sub));
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
    if (r == 1) {
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK, "✓ Member added successfully");
        clear_member_form(GTK_WIDGET(button));  /* Vider le formulaire */
        
        /* Auto-refresh table view */
        extern void on_button_display_members_clicked(GtkButton *b, gpointer user_data);
        on_button_display_members_clicked(NULL, NULL);
    }
    else
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                     GTK_BUTTONS_OK, "✗ Error: cannot add member");

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

    gchar *sub_text = get_combo_box_text(GTK_COMBO_BOX(sub));
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
    if (r == 1) {
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK, "✓ Member updated successfully");
        clear_member_form(GTK_WIDGET(button));  /* Vider le formulaire */
    }
    else
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                     GTK_BUTTONS_OK, "✗ Error: cannot update member");

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
                                     GTK_BUTTONS_OK, "✓ Member deleted successfully");
        clear_member_form(GTK_WIDGET(button));  /* Vider complètement */
    }
    else
        msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                     GTK_BUTTONS_OK, "✗ Error: cannot delete member");

    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}



// ======= BUTTON "SEARCH" (Enhanced: ID or FirstName) =======
void
on_buttonSearch_clicked(GtkButton *button,
                        gpointer user_data)
{
    // --- GET WIDGETS ---
    GtkWidget *id_widget = lookup_widget(GTK_WIDGET(button), "entryIdMember");
    GtkWidget *first_widget = lookup_widget(GTK_WIDGET(button), "entryFNM");
    
    const char *search_id = gtk_entry_get_text(GTK_ENTRY(id_widget));
    const char *search_first = gtk_entry_get_text(GTK_ENTRY(first_widget));
    
    Member m;
    int found = 0;
    
    // --- TRY SEARCH BY ID FIRST (if ID field is not empty) ---
    if (strlen(search_id) > 0) {
        m = chercher_member("members.txt", (char*)search_id);
        if (strcmp(m.id_member, "") != 0) {
            found = 1;
        }
    }
    
    // --- IF NOT FOUND AND FIRSTNAME IS PROVIDED, SEARCH BY FIRSTNAME ---
    if (!found && strlen(search_first) > 0) {
        m = chercher_member_by_firstname("members.txt", (char*)search_first);
        if (strcmp(m.id_member, "") != 0) {
            found = 1;
        }
    }
    
    // --- IF NOT FOUND ---
    if (!found) {
        GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
                                               GTK_BUTTONS_OK, "⚠ Member not found.\nTry searching by ID or First Name.");
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

    gtk_entry_set_text(GTK_ENTRY(id_widget), m.id_member);
    gtk_entry_set_text(GTK_ENTRY(first), m.first_name);
    gtk_entry_set_text(GTK_ENTRY(last), m.last_name);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(age), m.age);
    gtk_entry_set_text(GTK_ENTRY(phone), m.phone);
    gtk_entry_set_text(GTK_ENTRY(email), m.email);
    
    // Set subscription combo by finding the matching item
    if (sub && GTK_IS_COMBO_BOX(sub)) {
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(sub));
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
        gint index = 0;
        
        while (valid) {
            gchar *text = NULL;
            gtk_tree_model_get(model, &iter, 0, &text, -1);
            
            if (text && strcmp(text, m.subscription_type) == 0) {
                gtk_combo_box_set_active(GTK_COMBO_BOX(sub), index);
                g_free(text);
                break;
            }
            
            g_free(text);
            valid = gtk_tree_model_iter_next(model, &iter);
            index++;
        }
    }

    // Set gender radio buttons
    if (m.gender == 0) {
        GtkWidget *radio_female = lookup_widget(GTK_WIDGET(button), "radioFemale");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_female), TRUE);
    } else {
        GtkWidget *radio_male = lookup_widget(GTK_WIDGET(button), "radioMale");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_male), TRUE);
    }

    GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK, "✓ Member found and loaded!");
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
    handle_private_coach_request("activity_log.txt", (char*)member_id);

    GtkWidget *msg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK, "Private coach request submitted");
    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
}



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
    handle_join_course("activity_log.txt", (char*)member_id);

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
    gchar *active_text = get_combo_box_text(combobox);
    if (active_text) {
        /* Subscription type changed - keep age field always enabled */
        GtkWidget *spin = lookup_widget(GTK_WIDGET(combobox), "spinAge");
        if (spin == NULL) {
            /* fallback to global registry */
            extern GtkWidget *ui_get_object_by_name(const char *name);
            spin = ui_get_object_by_name("spinAge");
        }

        /* Always keep the age spin button ENABLED */
        if (spin && GTK_IS_SPIN_BUTTON(spin)) {
            gtk_widget_set_sensitive(spin, TRUE);
        }

        g_free(active_text);
    }
}
// Combo box change
void on_comboCoachName_changed(GtkComboBox *combobox, gpointer user_data)
{
    gchar *active_text = get_combo_box_text(combobox);
    if (active_text) {
        /* Show selected coach in the Member Statistics textview (quick feedback)
         * The real stats view is populated elsewhere when opening stats for a member.
         */
        GtkWidget *tv = lookup_widget(GTK_WIDGET(combobox), "textviewNumClasses");
        if (tv == NULL) {
            extern GtkWidget *ui_get_object_by_name(const char *name);
            tv = ui_get_object_by_name("textviewNumClasses");
        }
        if (tv) {
            gchar buffer[256];
            g_snprintf(buffer, sizeof(buffer), "Selected coach: %s", active_text);
            gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv)), buffer, -1);
        }
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

// ======= WINDOW SHOW INITIALIZATION =======
void
on_Member_Management_show(GtkWidget *widget, gpointer user_data)
{
    // The Subscription Combo is already populated via ListStore in the UI
    // Just set the default selection
    GtkWidget *combo = lookup_widget(widget, "comboSub");
    if (combo && GTK_IS_COMBO_BOX(combo)) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    }
}

void
on_Member_Statistics_show(GtkWidget *widget, gpointer user_data)
{
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *f;
    coach c;
    char fullname[100];
    
    /* 1. Get the ListStore via global lookup or from widget if possible.
       Since we are in isra_callbacks, we might not have easy access to the liststore
       if it's not a child of the widget passed. But usually it's global.
       Use ui_get_object_by_name for safety.
    */
    GObject *obj_store = (GObject*)ui_get_object_by_name("liststore_coaches");
    if (!obj_store) return;
    
    store = GTK_LIST_STORE(obj_store);
    
    /* 2. Clear existing items */
    gtk_list_store_clear(store);
    
    /* 3. Read coaches from file */
    f = fopen("coachs.txt", "r");
    if (f)
    {
        while (fscanf(f, "%d %29s %29s %9s %d %29s %19s %49s %29s",
                      &c.id, c.nom, c.prenom, c.genre, &c.age,
                      c.specialite, c.phone, c.email, c.dispo) == 9)
        {
             /* Format name: First Last */
             snprintf(fullname, sizeof(fullname), "%s %s", c.prenom, c.nom);
             
             gtk_list_store_append(store, &iter);
             gtk_list_store_set(store, &iter, 0, fullname, -1);
        }
        fclose(f);
    }
    
    /* 4. Set active to 0 if list not empty */
    GtkWidget *combo = ui_get_object_by_name("comboCoachName");
    if (!combo) combo = lookup_widget(widget, "comboCoachName"); /* Fallback if widget provided */
    
    if (combo && GTK_IS_COMBO_BOX(combo)) {
        if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter))
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    }
}


/* Callback for the delete button inside the card */
#if 0
/* Callback for the delete button inside the card */
void on_card_delete_clicked(GtkButton *btn, gpointer user_data)
{
    const char *id = (const char *)g_object_get_data(G_OBJECT(btn), "member_id");
    if (!id) return;

    /* Confirm deletion */
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, 
                                               GTK_BUTTONS_YES_NO, 
                                               "Are you sure you want to delete member ID %s?", id);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_YES)
    {
        /* Perform deletion */
        supprimer_member("members.txt", (char*)id);
        
        /* Refresh the view by calling the display function */
        /* Note: we pass NULL because the function handles looking up widgets globally */
        on_button_display_members_clicked(NULL, NULL);
    }
}

/* Helper to create a styled card for a member */
static GtkWidget* create_member_card(Member m)
{
    /* ... (Code omitted for brevity, logic replaced by TreeView) ... */
    return NULL; 
}
#endif

/* Double-click handler for deletion */
void on_treeview_members_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id;
    gchar *fname, *lname;

    model = gtk_tree_view_get_model(tree_view);

    if (gtk_tree_model_get_iter(model, &iter, path))
    {
        /* Get data from selected row */
        gtk_tree_model_get(model, &iter, 0, &id, 1, &fname, 2, &lname, -1);
        
        /* Confirmation Dialog */
        /* Use main window as parent if possible, but NULL works */
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, 
                                                   GTK_BUTTONS_YES_NO, 
                                                   "Delete Member?\n\nID: %s\nName: %s %s", id, fname, lname);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES)
        {
            /* Delete from file */
             /* extern void supprimer_member(char * filename, char * id); removed - already in header as int */
             supprimer_member("members.txt", id);
             
             /* Refresh View */
             on_button_display_members_clicked(NULL, NULL);
        }
        
        g_free(id);
        g_free(fname);
        g_free(lname);
    }
}

void on_button_display_members_clicked(GtkButton *button, gpointer user_data)
{
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *f;
    Member m;
    char line[512];
    
    /* Get the ListStore object */
    GObject *obj_store = (GObject *)ui_get_object_by_name("liststore_members");
    if (!obj_store) {
        g_warning("ListStore 'liststore_members' not found");
        return;
    }
    store = GTK_LIST_STORE(obj_store);
    
    /* Clear existing items */
    gtk_list_store_clear(store);
    
    /* Read members from file */
    f = fopen("members.txt", "r");
    if (f)
    {
        while (fgets(line, sizeof(line), f))
        {
            if (sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%[^|]|%[^|]|%d|%d|%d",
                   m.id_member, m.first_name, m.last_name, &m.age, &m.gender,
                   m.phone, m.email, m.subscription_type,
                   &m.private_coach, &m.request_show_stats, &m.join_course) >= 8)
            {
                /* Gender display conversion */
                const char *gender_str = (m.gender == 0) ? "Female" : "Male";
                if (strlen(m.subscription_type) == 0) strcpy(m.subscription_type, "Standard");

                /* Append to ListStore */
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 
                                   0, m.id_member,
                                   1, m.first_name,
                                   2, m.last_name,
                                   3, m.age,
                                   4, gender_str,
                                   5, m.phone,
                                   6, m.email,
                                   7, m.subscription_type,
                                   -1);
            }
        }
        fclose(f);
    }
}
