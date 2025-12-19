#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrain.h"

#define FICHIER "coachs.txt"

int main()
{
    int choix;
    coach c;
    int id;
    int nb;
    float age_moyen;

    do {
        printf("\n=== MENU BACKEND COACH ===\n");
        printf("1. Ajouter coach\n");
        printf("2. Modifier coach\n");
        printf("3. Supprimer coach\n");
        printf("4. Chercher coach\n");
        printf("5. Statistiques\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            printf("ID : ");
            scanf("%d", &c.id);

            printf("Nom : ");
            scanf("%s", c.nom);

            printf("Prenom : ");
            scanf("%s", c.prenom);

            printf("Genre (male/female) : ");
            scanf("%s", c.genre);

            printf("Age : ");
            scanf("%d", &c.age);

            printf("Specialite : ");
            scanf("%s", c.specialite);

            printf("Phone : ");
            scanf("%s", c.phone);

            printf("Email : ");
            scanf("%s", c.email);

            printf("Dispo : ");
            scanf("%s", c.dispo);

            if (ajouter_coach(FICHIER, c))
                printf("Ajout OK\n");
            else
                printf("Erreur ajout\n");
            break;

        case 2:
            printf("ID à modifier : ");
            scanf("%d", &id);

            printf("Nouveaux infos\n");

            printf("Nom : ");
            scanf("%s", c.nom);

            printf("Prenom : ");
            scanf("%s", c.prenom);

            printf("Genre : ");
            scanf("%s", c.genre);

            printf("Age : ");
            scanf("%d", &c.age);

            printf("Specialite : ");
            scanf("%s", c.specialite);

            printf("Phone : ");
            scanf("%s", c.phone);

            printf("Email : ");
            scanf("%s", c.email);

            printf("Dispo : ");
            scanf("%s", c.dispo);

            c.id = id;

            if (modifier_coach(FICHIER, id, c))
                printf("Modification OK\n");
            else
                printf("Coach non trouvé\n");
            break;

        case 3:
            printf("ID à supprimer : ");
            scanf("%d", &id);

            if (supprimer_coach(FICHIER, id))
                printf("Suppression OK\n");
            else
                printf("Coach non trouvé\n");
            break;

        case 4:
            printf("ID à chercher : ");
            scanf("%d", &id);

            c = chercher_coach(FICHIER, id);
            if (c.id != -1)
            {
                printf("Trouvé : %d %s %s %s %d %s %s %s %s\n",
                       c.id, c.nom, c.prenom, c.genre, c.age,
                       c.specialite, c.phone, c.email, c.dispo);
            }
            else
            {
                printf("Coach introuvable\n");
            }
            break;

        case 5:
            coach_stats(FICHIER, &nb, &age_moyen);
            printf("Nombre de coachs : %d\n", nb);
            printf("Age moyen : %.2f\n", age_moyen);
            break;
        }

    } while (choix != 0);

    return 0;
}


