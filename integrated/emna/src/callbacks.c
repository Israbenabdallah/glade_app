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

#define FICHIER_COACHS "coachs.txt"

/* ========================================================= */
/*        FONCTIONS UTILITAIRES SUR LE FORMULAIRE            */
/* ========================================================= */

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

    /* Speciality (GtkComboBoxText) */
    w = lookup_widget(objet_graphique, "combo_specialite");
    txt_dyn = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(w));
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
    gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(w), c.specialite);
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
    GtkWidget *msg = lookup_widget(objet_graphique, "label_message");

    if (ajouter_coach(FICHIER_COACHS, c))
        gtk_label_set_text(GTK_LABEL(msg), "Ajout avec succès");
    else
        gtk_label_set_text(GTK_LABEL(msg), "Erreur fichier (ajout)");

    (void)user_data;
}

/* Bouton Edit */
void
on_button_edit_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    coach c = lire_formulaire(objet_graphique);
    GtkWidget *msg = lookup_widget(objet_graphique, "label_message");

    if (modifier_coach(FICHIER_COACHS, c.id, c))
        gtk_label_set_text(GTK_LABEL(msg), "Modification avec succès");
    else
        gtk_label_set_text(GTK_LABEL(msg), "Coach non trouvé");

    (void)user_data;
}

/* Bouton Delete */
void
on_button_delete_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    GtkWidget *w   = lookup_widget(objet_graphique, "entry_id");
    GtkWidget *msg = lookup_widget(objet_graphique, "label_message");
    const gchar *txt = gtk_entry_get_text(GTK_ENTRY(w));
    int id = atoi(txt);

    if (supprimer_coach(FICHIER_COACHS, id))
        gtk_label_set_text(GTK_LABEL(msg), "Suppression avec succès");
    else
        gtk_label_set_text(GTK_LABEL(msg), "Coach non trouvé");

    (void)user_data;
}

/* Bouton Search */
void
on_button_search_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    GtkWidget *w   = lookup_widget(objet_graphique, "entry_id");
    GtkWidget *msg = lookup_widget(objet_graphique, "label_message");
    const gchar *txt = gtk_entry_get_text(GTK_ENTRY(w));
    int id = atoi(txt);

    coach c = chercher_coach(FICHIER_COACHS, id);

    if (c.id != -1)
    {
        remplir_formulaire(objet_graphique, c);
        gtk_label_set_text(GTK_LABEL(msg), "Coach trouvé");
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(msg), "Coach introuvable");
    }

    (void)user_data;
}

/* Bouton View stats : affiche la fenêtre window_stats */
void
on_button_view_stats_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *objet_graphique = GTK_WIDGET(button);
    int nb;
    float age_moyen;
    char buf[64];

    coach_stats(FICHIER_COACHS, &nb, &age_moyen);

    GtkWidget *label_nb  = lookup_widget(objet_graphique, "label_nb_coachs");
    GtkWidget *label_age = lookup_widget(objet_graphique, "label_age_moyen");
    GtkWidget *win_stats = lookup_widget(objet_graphique, "window_stats");

    sprintf(buf, "%d", nb);
    gtk_label_set_text(GTK_LABEL(label_nb), buf);

    sprintf(buf, "%.2f", age_moyen);
    gtk_label_set_text(GTK_LABEL(label_age), buf);

    gtk_widget_show(win_stats);

    (void)user_data;
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


