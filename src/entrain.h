#ifndef COACH_H_INCLUDED
#define COACH_H_INCLUDED

typedef struct
{
    int  id;
    char nom[30];
    char prenom[30];
    char genre[10];       /* "male" ou "female" */
    int  age;
    char specialite[30];
    char phone[20];
    char email[50];
    char dispo[30];       /* ex: "Monday/8am" */
} coach;

/* CRUD sur fichier texte */
int   ajouter_coach   (const char *filename, coach c);
int   modifier_coach  (const char *filename, int id, coach nouv);
int   supprimer_coach (const char *filename, int id);
coach chercher_coach  (const char *filename, int id);

/* Statistiques simples */
void  coach_stats(const char *filename, int *nb, float *age_moyen);

#endif
