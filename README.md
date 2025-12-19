# Documentation Technique : Gestion des Membres avec GtkTreeView

## 1. Introduction au TreeView (Modèle MVC)

Ce projet met en œuvre le widget **GtkTreeView** de la bibliothèque GTK+ 2 pour la gestion des membres. Cette approche repose sur la séparation entre le stockage des données et leur présentation.

### 1.1 Architecture
L'architecture utilisée suit le modèle **MVC (Modèle-Vue-Contrôleur)** adapté à GTK :
*   **Modèle (Model)** : `GtkListStore`. C'est la structure en mémoire qui stocke les données (ID, Nom, Age, etc.).
*   **Vue (View)** : `GtkTreeView`. C'est le composant graphique qui affiche les données du modèle sous forme de tableau.
*   **Données Persistantes** : Le fichier `members.txt` sert de base de données physique.

### 1.2 Composants implémentés
*   **GtkListStore** : Défini dans `ui/merged.ui` (`liststore_members`), il contient 8 colonnes (ID, Prénom, Nom, Age, Genre, Tél, Email, Abonnement).
*   **GtkTreeView** : Défini dans `ui/merged.ui` (`treeview_members`), il affiche ces colonnes via des **GtkCellRendererText**.

---

## 2. Fonctionnalités du Module Membres

Le module de gestion des membres permet :
1.  **Affichage** : Lecture du fichier `members.txt` et remplissage du `GtkListStore`.
2.  **Affichage hiérarchique** : Présentation claire via un tableau multi-colonnes.
3.  **Suppression** : Action déclenchée par un **double-clic** (`row-activated`) sur une ligne, suivie d'une confirmation et d'un rafraîchissement automatique.

---

## 3. Guide d'Installation et d'Exécution

### Pré-requis
*   Environnement Linux avec **GCC** et **Make**.
*   Bibliothèques de développement **GTK+ 2.0** (`libgtk2.0-dev`).

### Procédure
1.  Ouvrez un terminal dans le dossier `integrated`.
2.  Compilez le projet :
    ```bash
    make
    ```
3.  Lancez l'application :
    ```bash
    ./integrated_app
    ```
4.  Dans l'application :
    *   Cliquez sur **Member Management**.
    *   Cliquez sur **Display Members** pour charger la liste.
    *   **Double-cliquez** sur un membre pour le supprimer.

---

## 4. Description des Fichiers Sources (`src/`)

### Backend (Logique Métier & Données)
*   **`Member.c / .h`** : Contient défini la structure `Member` et les fonctions de gestion de fichier (`ajouter_member`, `supprimer_member`, `rechercher_member`).
*   **`members.txt`** : Fichier texte stockant les données des membres (format pipe `|`).

### Frontend (Interface & Callbacks)
*   **`main.c`** : Point d'entrée du programme. Initialise GTK et le Dashboard.
*   **`dashboard.c`** : Gère l'architecture "fenêtre unique". Il charge l'interface Glade et intègre les différents modules (Membres, Coachs) dans des onglets (`GtkNotebook`).
*   **`isra_callbacks.c`** : Contient la logique spécifique à la gestion des membres :
    *   `on_button_display_members_clicked` : Lit `members.txt` et remplit le `GtkListStore`.
    *   `on_treeview_members_row_activated` : Gère la suppression lors du double-clic.
*   **`callbacks.c`** : Contient les callbacks généraux et ceux liés à la gestion des Coachs.
*   **`ui_registry.c`** : Utilitaire pour récupérer facilement les widgets (ex: `ui_get_object_by_name`) depuis n'importe quel fichier source.

### Interface Graphique
*   **`../ui/merged.ui`** : Fichier XML généré par Glade décrivant toute l'interface graphique (Fenêtres, TreeViews, Boutons, Modèles).

### Autres Modules
*   **`entrain.c`** : Gestion des séances d'entraînement.
*   **`stats_graphics.c`** : Génération des graphiques statistiques (Cairo).
*   **`glade_shims.c`** : Fonctions utilitaires pour compatibilité et fonctionnalités avancées (ex: export PDF).

---

## 5. Conclusion

Ce projet illustre l'intégration complète d'un système de gestion (CRUD) avec une interface graphique moderne en C/GTK, utilisant des composants avancés comme **GtkTreeView** pour une expérience utilisateur optimale (sélection, tri, colonnes).
