#include <stdio.h>
#include <string.h>
#include "entrain.h"

/* Format de ligne dans le fichier :
   id nom prenom genre age specialite phone email dispo
   → Tous les champs SANS espaces (utiliser _ si besoin)
*/

int ajouter_coach(const char *filename, coach c)
{
    FILE *f = fopen(filename, "a");
    if (!f) return 0;

    fprintf(f, "%d %s %s %s %d %s %s %s %s\n",
            c.id, c.nom, c.prenom, c.genre, c.age,
            c.specialite, c.phone, c.email, c.dispo);

    fclose(f);
    return 1;
}

int modifier_coach(const char *filename, int id, coach nouv)
{
    FILE *f = fopen(filename, "r");
    FILE *tmp = fopen("coachs.tmp", "w");
    coach c;
    int trouve = 0;

    if (!f || !tmp)
    {
        if (f) fclose(f);
        if (tmp) fclose(tmp);
        return 0;
    }

    while (fscanf(f, "%d %29s %29s %9s %d %29s %19s %49s %29s",
                  &c.id, c.nom, c.prenom, c.genre, &c.age,
                  c.specialite, c.phone, c.email, c.dispo) == 9)
    {
        if (c.id == id)
        {
            fprintf(tmp, "%d %s %s %s %d %s %s %s %s\n",
                    nouv.id, nouv.nom, nouv.prenom, nouv.genre, nouv.age,
                    nouv.specialite, nouv.phone, nouv.email, nouv.dispo);
            trouve = 1;
        }
        else
        {
            fprintf(tmp, "%d %s %s %s %d %s %s %s %s\n",
                    c.id, c.nom, c.prenom, c.genre, c.age,
                    c.specialite, c.phone, c.email, c.dispo);
        }
    }

    fclose(f);
    fclose(tmp);

    if (trouve)
    {
        remove(filename);
        rename("coachs.tmp", filename);
        return 1;
    }
    else
    {
        remove("coachs.tmp");
        return 0;
    }
}

int supprimer_coach(const char *filename, int id)
{
    FILE *f = fopen(filename, "r");
    FILE *tmp = fopen("coachs.tmp", "w");
    coach c;
    int trouve = 0;

    if (!f || !tmp)
    {
        if (f) fclose(f);
        if (tmp) fclose(tmp);
        return 0;
    }

    while (fscanf(f, "%d %29s %29s %9s %d %29s %19s %49s %29s",
                  &c.id, c.nom, c.prenom, c.genre, &c.age,
                  c.specialite, c.phone, c.email, c.dispo) == 9)
    {
        if (c.id == id)
            trouve = 1;
        else
            fprintf(tmp, "%d %s %s %s %d %s %s %s %s\n",
                    c.id, c.nom, c.prenom, c.genre, c.age,
                    c.specialite, c.phone, c.email, c.dispo);
    }

    fclose(f);
    fclose(tmp);

    if (trouve)
    {
        remove(filename);
        rename("coachs.tmp", filename);
        return 1;
    }
    else
    {
        remove("coachs.tmp");
        return 0;
    }
}

coach chercher_coach(const char *filename, int id)
{
    FILE *f = fopen(filename, "r");
    coach c;

    c.id = -1; /* valeur spéciale = introuvable */

    if (!f) return c;

    while (fscanf(f, "%d %29s %29s %9s %d %29s %19s %49s %29s",
                  &c.id, c.nom, c.prenom, c.genre, &c.age,
                  c.specialite, c.phone, c.email, c.dispo) == 9)
    {
        if (c.id == id)
        {
            fclose(f);
            return c;
        }
    }

    fclose(f);
    c.id = -1;
    return c;
}

void coach_stats(const char *filename, int *nb, float *age_moyen)
{
    FILE *f = fopen(filename, "r");
    coach c;
    int count = 0;
    int somme_age = 0;

    *nb = 0;
    *age_moyen = 0.0;

    if (!f) return;

    while (fscanf(f, "%d %29s %29s %9s %d %29s %19s %49s %29s",
                  &c.id, c.nom, c.prenom, c.genre, &c.age,
                  c.specialite, c.phone, c.email, c.dispo) == 9)
    {
        count++;
        somme_age += c.age;
    }

    fclose(f);

    if (count > 0)
    {
        *nb = count;
        *age_moyen = (float)somme_age / count;
    }
}
