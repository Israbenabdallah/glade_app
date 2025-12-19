# Projet intégré — Emna + Isra

But: démontrer une intégration initiale en chargeant les interfaces Glade des deux sous-projets
et en lançant une seule application qui montre les fenêtres principales.

Pré-requis:
- GTK+ 2 devel installé (package `libgtk2.0-dev` ou équivalent).

Construction:
```bash
cd integrated
make
```

Exécution:
```bash
./integrated_app
```

Remarques:
- Ce launcher charge les fichiers Glade existants dans `../emna/frontend.glade` et
  `../isra/project4.glade`. Pour un packaging final, copiez/emballez ces fichiers dans
  un sous-dossier `ui/` et modifiez les chemins dans `src/main.c`.
- Les callbacks et la logique métier ne sont pas encore entièrement fusionnés —
  ceci est un premier pas pour vérifier que les deux interfaces peuvent coexister
  dans une seule exécutable. La prochaine étape est d'adapter les callbacks et
  réutiliser le code métier (`support.c`, `entrain.c`, `Member.c`, `memberstats.c`, ...).
