#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "entrain.h"
#include "ui_registry.h"

#define FICHIER_COACHS "coachs.txt"

/* ========================================================= */
/*        FONCTIONS UTILITAIRES SUR LE FORMULAIRE            */
/* ========================================================= */

/* Récupère le texte actif d'un GtkComboBox (compatible GTK2) */
static gchar* get_combo_box_active_text(GtkComboBox *combo_box)
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

/* Ajoute du texte au début d'un GtkComboBox (compatible GTK2) */
static void combo_box_prepend_text(GtkComboBox *combo_box, const gchar *text)
{
    GtkTreeModel *model;
    GtkListStore *store;
    GtkTreeIter iter;
    
    if (!GTK_IS_COMBO_BOX(combo_box) || text == NULL)
        return;
    
    model = gtk_combo_box_get_model(combo_box);
    if (!GTK_IS_LIST_STORE(model))
        return;
    
    store = GTK_LIST_STORE(model);
    gtk_list_store_prepend(store, &iter);
    gtk_list_store_set(store, &iter, 0, text, -1);
}

/* Lit tous les champs de la fenêtre principale et remplit une struct coach */
static coach lire_formulaire(GtkWidget *objet_graphique)
{
    coach c;
    GtkWidget *w;
    const gchar *txt;
    gchar *txt_dyn;

    /* ID */
    w = lookup_widget(objet_graphique, "entry_id");
    txt = gtk_entry_get_text(GTK_ENTRY(w));
    c.id = atoi(txt);

    /* First name -> nom */
    w = lookup_widget(objet_graphique, "entry_first_name");
    txt = gtk_entry_get_text(GTK_ENTRY(w));
    strcpy(c.nom, txt);

    /* Last name -> prenom */
    w = lookup_widget(objet_graphique, "entry_last_name");
    txt = gtk_entry_get_text(GTK_ENTRY(w));
    strcpy(c.prenom, txt);

    /* Genre */
    w = lookup_widget(objet_graphique, "radio_male");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)))
        strcpy(c.genre, "male");
    else
        strcpy(c.genre, "female");

    /* Age */
    w = lookup_widget(objet_graphique, "spin_age");
    c.age = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w));

    /* Speciality (GtkComboBox) */
    w = lookup_widget(objet_graphique, "combo_specialite");
    txt_dyn = get_combo_box_active_text(GTK_COMBO_BOX(w));
    if (txt_dyn == NULL)
        strcpy(c.specialite, "");
    else
    {
        strcpy(c.specialite, txt_dyn);
        g_free(txt_dyn);
    }

    /* Phone */
    w = lookup_widget(objet_graphique, "entry_phone");
    txt = gtk_entry_get_text(GTK_ENTRY(w));
    strcpy(c.phone, txt);

    /* E-mail */
    w = lookup_widget(objet_graphique, "entry_email");
    txt = gtk_entry_get_text(GTK_ENTRY(w));
    strcpy(c.email, txt);

    /* Disponibility */
    w = lookup_widget(objet_graphique, "entry_dispo");
    txt = gtk_entry_get_text(GTK_ENTRY(w));
    strcpy(c.dispo, txt);

    return c;
}

/* Affiche un message dans une popup (GtkMessageDialog) */
static void afficher_message_popup(GtkWidget *parent, GtkMessageType type, const gchar *message)
{
    GtkWidget *dialog;
    GtkWidget *toplevel;
    
    /* Trouver la fenêtre parente */
    toplevel = gtk_widget_get_toplevel(parent);
    if (!GTK_IS_WINDOW(toplevel))
        toplevel = NULL;
    
    /* Créer la boîte de dialogue */
    dialog = gtk_message_dialog_new(
        GTK_WINDOW(toplevel),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        type,
        GTK_BUTTONS_OK,
        "%s",
        message
    );
    
    gtk_window_set_title(GTK_WINDOW(dialog), "Coach Management");
    
    /* Afficher et détruire */
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/* Affiche un coach dans le formulaire de la fenêtre principale */
static void remplir_formulaire(GtkWidget *objet_graphique, coach c)
{
    GtkWidget *w;
    char buf[64];

    /* ID */
    w = lookup_widget(objet_graphique, "entry_id");
    sprintf(buf, "%d", c.id);
    gtk_entry_set_text(GTK_ENTRY(w), buf);

    /* First name (nom) */
    w = lookup_widget(objet_graphique, "entry_first_name");
    gtk_entry_set_text(GTK_ENTRY(w), c.nom);

    /* Last name (prenom) */
    w = lookup_widget(objet_graphique, "entry_last_name");
    gtk_entry_set_text(GTK_ENTRY(w), c.prenom);

    /* Genre */
    w = lookup_widget(objet_graphique, "radio_male");
    if (strcmp(c.genre, "male") == 0)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
    else
    {
        GtkWidget *wf = lookup_widget(objet_graphique, "radio_female");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wf), TRUE);
    }

    /* Age */
    w = lookup_widget(objet_graphique, "spin_age");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), c.age);

    /* Speciality :
       on insère la spécialité dans le combo (pour être sûr qu'elle y est),
       puis on sélectionne la première entrée. */
    w = lookup_widget(objet_graphique, "combo_specialite");
    combo_box_prepend_text(GTK_COMBO_BOX(w), c.specialite);
    gtk_combo_box_set_active(GTK_COMBO_BOX(w), 0);

    /* Phone */
    w = lookup_widget(objet_graphique, "entry_phone");
    gtk_entry_set_text(GTK_ENTRY(w), c.phone);

    /* E-mail */
    w = lookup_widget(objet_graphique, "entry_email");
    gtk_entry_set_text(GTK_ENTRY(w), c.email);

    /* Dispo */
    w = lookup_widget(objet_graphique, "entry_dispo");
    gtk_entry_set_text(GTK_ENTRY(w), c.dispo);
}

/* Vide tous les champs du formulaire */
static void clear_form(GtkWidget *objet_graphique)
{
    GtkWidget *w;
    
    /* Vider les entries */
    w = lookup_widget(objet_graphique, "entry_id");
    gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(objet_graphique, "entry_first_name");
    gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(objet_graphique, "entry_last_name");
    gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(objet_graphique, "entry_phone");
    gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(objet_graphique, "entry_email");
    gtk_entry_set_text(GTK_ENTRY(w), "");
    
    w = lookup_widget(objet_graphique, "entry_dispo");
    gtk_entry_set_text(GTK_ENTRY(w), "");
    
    /* Réinitialiser age */
    w = lookup_widget(objet_graphique, "spin_age");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), 18);
    
    /* Sélectionner male par défaut */
    w = lookup_widget(objet_graphique, "radio_male");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);
    
    /* Réinitialiser combo */
    w = lookup_widget(objet_graphique, "combo_specialite");
    gtk_combo_box_set_active(GTK_COMBO_BOX(w), -1);
}

/* ========================================================= */
/*                      CALLBACKS GTK                        */
/* ========================================================= */

void
on_window_coach_destroy (GtkWidget *widget, gpointer user_data)
{
    (void)widget;
    (void)user_data;
    gtk_main_quit();
}

/* Au moment où la fenêtre principale s'affiche :
   on remplit la combo Speciality */
void
on_window_coach_show (GtkWidget *widget, gpointer user_data)
{
   


}

/* Bouton Add */
void
on_button_add_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    coach c = lire_formulaire(objet_graphique);

    if (ajouter_coach(FICHIER_COACHS, c))
    {
        afficher_message_popup(objet_graphique, GTK_MESSAGE_INFO, "✓ Coach ajouté avec succès !");
        clear_form(objet_graphique);  /* Vider le formulaire pour un nouveau coach */
        
        /* Auto-refresh coach list */
        extern void on_button_display_clicked(GtkButton *button, gpointer user_data);
        on_button_display_clicked(NULL, NULL);
    }
    else
        afficher_message_popup(objet_graphique, GTK_MESSAGE_ERROR, "✗ Erreur lors de l'ajout du coach.");

    (void)user_data;
}

/* Bouton Edit */
void
on_button_edit_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    coach c = lire_formulaire(objet_graphique);

    if (modifier_coach(FICHIER_COACHS, c.id, c))
    {
        afficher_message_popup(objet_graphique, GTK_MESSAGE_INFO, "✓ Coach modifié avec succès !");
        clear_form(objet_graphique);  /* Vider après modification */
        
        /* Auto-refresh coach list */
        extern void on_button_display_clicked(GtkButton *button, gpointer user_data);
        on_button_display_clicked(NULL, NULL);
    }
    else
        afficher_message_popup(objet_graphique, GTK_MESSAGE_WARNING, "⚠ Coach non trouvé.");

    (void)user_data;
}

/* Bouton Delete */
void
on_button_delete_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    GtkWidget *w   = lookup_widget(objet_graphique, "entry_id");
    const gchar *txt = gtk_entry_get_text(GTK_ENTRY(w));
    int id = atoi(txt);

    if (supprimer_coach(FICHIER_COACHS, id))
    {
        afficher_message_popup(objet_graphique, GTK_MESSAGE_INFO, "✓ Coach supprimé avec succès !");
        clear_form(objet_graphique);  /* Vider après suppression */
        
        /* Auto-refresh coach list */
        extern void on_button_display_clicked(GtkButton *button, gpointer user_data);
        on_button_display_clicked(NULL, NULL);
    }
    else
        afficher_message_popup(objet_graphique, GTK_MESSAGE_WARNING, "⚠ Coach non trouvé.");

    (void)user_data;
}

/* Bouton Search */
void
on_button_search_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    GtkWidget *w   = lookup_widget(objet_graphique, "entry_id");
    const gchar *txt = gtk_entry_get_text(GTK_ENTRY(w));
    int id = atoi(txt);

    coach c = chercher_coach(FICHIER_COACHS, id);

    if (c.id != -1)
    {
        remplir_formulaire(objet_graphique, c);
        afficher_message_popup(objet_graphique, GTK_MESSAGE_INFO, "✓ Coach trouvé !");
    }
    else
    {
        afficher_message_popup(objet_graphique, GTK_MESSAGE_WARNING, "⚠ Coach introuvable.");
    }

    (void)user_data;
}

/* Bouton View stats : affiche la fenêtre window_stats */
/* Bouton View stats : affiche la fenêtre window_stats */
void
on_dashboard_button_coach_stats_clicked (GtkButton *button, gpointer user_data)
{
    (void)button; (void)user_data;
    
    int nb;
    float age_moyen;
    char buf[64];

    /* Calculate stats */
    coach_stats("coachs.txt", &nb, &age_moyen); /* Assuming filename, defined as FICHIER_COACHS in header */

    /* Get Window and Widgets using global lookup */
    extern GtkWidget *ui_get_object_by_name(const char *name);
    
    GtkWidget *win_stats = ui_get_object_by_name("window_stats");
    GtkWidget *lbl_nb = ui_get_object_by_name("label_val_nb_coachs"); /* New ID for Label */
    GtkWidget *lbl_age = ui_get_object_by_name("label_val_age_moyen"); /* New ID for Label */

    if (lbl_nb) {
        char markup[128];
        snprintf(markup, sizeof(markup), "<span size='30000' weight='bold' foreground='#2c3e50'>%d</span>", nb);
        gtk_label_set_markup(GTK_LABEL(lbl_nb), markup);
    }

    if (lbl_age) {
        char markup[128];
        snprintf(markup, sizeof(markup), "<span size='30000' weight='bold' foreground='#e67e22'>%.1f</span>", age_moyen);
        gtk_label_set_markup(GTK_LABEL(lbl_age), markup);
    }

    if (win_stats) {
        /* gtk_widget_show_all(win_stats); // Removed: Embedded in dashboard */
    } else {
        g_printerr("Error: window_stats not found!\n");
    }
}

/* Signaux optionnels (non utilisés actuellement) */

void
on_combo_specialite_changed (GtkComboBox *combobox, gpointer user_data)
{
    (void)combobox;
    (void)user_data;
}

void
on_radio_male_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
    (void)togglebutton;
    (void)user_data;
}

void
on_radio_female_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
    (void)togglebutton;
    (void)user_data;
}




/* Button Display All Coaches: Opens the list window and loads data */
void on_button_display_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *win = ui_get_object_by_name("window_coach_list");
    GObject *store_obj = (GObject*)ui_get_object_by_name("liststore_coach_list");
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *f;
    coach c;

    if (!win) {
        /* Fallback if window not found (should be in merged.ui) */
        g_warning("Window 'window_coach_list' not found!");
        return;
    }
    
    if (!store_obj) {
        g_warning("ListStore 'liststore_coach_list' not found!");
        return;
    }
    store = GTK_LIST_STORE(store_obj);

    /* Clear existing */
    gtk_list_store_clear(store);

    /* Load from file */
    f = fopen(FICHIER_COACHS, "r");
    if (f)
    {
         while (fscanf(f, "%d %29s %29s %9s %d %29s %19s %49s %29s",
                      &c.id, c.nom, c.prenom, c.genre, &c.age,
                      c.specialite, c.phone, c.email, c.dispo) == 9)
        {
             gtk_list_store_append(store, &iter);
             /* Columns: 0=ID, 1=Nom, 2=Prenom, 3=Age, 4=Spec, 5=Phone, 6=Email, 7=Dispo */
             gtk_list_store_set(store, &iter, 
                                0, c.id,
                                1, c.nom,
                                2, c.prenom,
                                3, c.age,
                                4, c.specialite,
                                5, c.phone,
                                6, c.email,
                                7, c.dispo,
                                -1);
        }
        fclose(f);
    }
    
    /* gtk_widget_show_all(win); // Removed: embedded in dashboard */
}
