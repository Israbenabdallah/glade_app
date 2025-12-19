# 🚫 EXPORT PDF - FONCTIONNALITÉ DÉSACTIVÉE

## ⚠️ Changement de Statut
À la demande de l'utilisateur, et suite à des instabilités techniques persistantes (Segmentation Faults liés au driver graphique PDF Cairo), la fonctionnalité **Export PDF** a été **retirée de l'application**.

## 📝 État Actuel de l'Interface
- **Member Statistics** :
  - ✅ Saisie des données
  - ✅ Sauvegarde (Save Statistics)
  - ✅ Graphiques (Histogramme et Progression) affichés à l'écran (si activés)
  - ❌ **Bouton Export PDF : SUPPRIMÉ**

## 🔧 Note Technique
Le code de génération PDF (`glade_shims.c`) est toujours présent dans les sources (pour référence future ou réactivation éventuelle après correction des dépendances), mais n'est plus accessible depuis l'interface utilisateur, garantissant ainsi la stabilité de l'application.

---
**L'application est maintenant stable et sans risque de crash lié au PDF.**
