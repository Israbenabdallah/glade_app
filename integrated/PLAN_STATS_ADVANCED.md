# 📊 PLAN D'IMPLÉMENTATION - Member Statistics Advanced

## 🎯 Fonctionnalités à ajouter

### 1. Graphiques Cairo
- **Histogramme** : Fréquentation par période
- **Courbe de progression** : Évolution dans le temps

### 2. Filtres et Recherche
- **Filtre par date** : Sélection période
- **Recherche par coach** : Combo box filtrable

---

## 📐 Architecture proposée

### A. Modifications UI (merged.ui)

```xml
<!-- Ajouter après la section Performance Metrics -->

<!-- Filters Section -->
<object class="GtkFrame" id="frame_filters">
  <property name="label"><b>Filters</b></property>
  <child>
    <object class="GtkTable">
      <!-- Date From -->
      <object class="GtkEntry" id="entry_date_from"/>
      <!-- Date To -->
      <object class="GtkEntry" id="entry_date_to"/>
      <!-- Coach Filter -->
      <object class="GtkComboBox" id="combo_coach_filter"/>
      <!-- Apply Button -->
      <object class="GtkButton" id="button_apply_filter"/>
    </object>
  </child>
</object>

<!-- Charts Section -->
<object class="GtkFrame" id="frame_charts">
  <property name="label"><b>Statistics Charts</b></property>
  <child>
    <object class="GtkVBox">
      <!-- Histogram -->
      <object class="GtkDrawingArea" id="drawing_histogram">
        <property name="height_request">200</property>
      </object>
      <!-- Progress Curve -->
      <object class="GtkDrawingArea" id="drawing_progress">
        <property name="height_request">200</property>
      </object>
    </object>
  </child>
</object>
```

### B. Fichiers à créer

#### 1. `src/stats_graphics.h`
```c
#ifndef STATS_GRAPHICS_H
#define STATS_GRAPHICS_H

#include <gtk/gtk.h>
#include <cairo.h>

// Structures
typedef struct {
    char date[20];
    int attendance;
} AttendanceData;

typedef struct {
    char coach_name[50];
    int count;
} CoachData;

// Fonctions de dessin
gboolean on_drawing_histogram_expose(GtkWidget *widget, 
                                     GdkEventExpose *event, 
                                     gpointer data);

gboolean on_drawing_progress_expose(GtkWidget *widget,
                                    GdkEventExpose *event,
                                    gpointer data);

void draw_histogram(cairo_t *cr, int width, int height, 
                   AttendanceData *data, int count);

void draw_progress_curve(cairo_t *cr, int width, int height,
                        AttendanceData *data, int count);

#endif
```

#### 2. `src/stats_graphics.c`
```c
#include "stats_graphics.h"
#include <string.h>

// Données de test (à remplacer par vraies données)
static AttendanceData sample_data[] = {
    {"2025-01-01", 15},
    {"2025-01-08", 20},
    {"2025-01-15", 18},
    {"2025-01-22", 25},
    {"2025-01-29", 22}
};

static int sample_count = 5;

gboolean on_drawing_histogram_expose(GtkWidget *widget,
                                     GdkEventExpose *event,
                                     gpointer data)
{
    cairo_t *cr = gdk_cairo_create(widget->window);
    
    // Get widget dimensions
    int width = widget->allocation.width;
    int height = widget->allocation.height;
    
    // Draw histogram
    draw_histogram(cr, width, height, sample_data, sample_count);
    
    cairo_destroy(cr);
    return FALSE;
}

void draw_histogram(cairo_t *cr, int width, int height,
                   AttendanceData *data, int count)
{
    if (count == 0) return;
    
    // White background
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    
    // Find max value
    int max_val = 0;
    for (int i = 0; i < count; i++) {
        if (data[i].attendance > max_val)
            max_val = data[i].attendance;
    }
    
    // Calculate bar dimensions
    int margin = 40;
    int bar_width = (width - 2 * margin) / count;
    int chart_height = height - 2 * margin;
    
    // Draw axes
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, margin, margin);
    cairo_line_to(cr, margin, height - margin);
    cairo_line_to(cr, width - margin, height - margin);
    cairo_stroke(cr);
    
    // Draw bars
    for (int i = 0; i < count; i++) {
        int bar_height = (data[i].attendance * chart_height) / max_val;
        int x = margin + i * bar_width + 5;
        int y = height - margin - bar_height;
        
        // Bar with gradient
        cairo_pattern_t *pat = cairo_pattern_create_linear(0, y, 0, height - margin);
        cairo_pattern_add_color_stop_rgb(pat, 0, 0.2, 0.6, 1.0);
        cairo_pattern_add_color_stop_rgb(pat, 1, 0.1, 0.3, 0.8);
        
        cairo_set_source(cr, pat);
        cairo_rectangle(cr, x, y, bar_width - 10, bar_height);
        cairo_fill(cr);
        cairo_pattern_destroy(pat);
        
        // Value on top
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 12);
        
        char val_str[10];
        sprintf(val_str, "%d", data[i].attendance);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, val_str, &extents);
        cairo_move_to(cr, x + (bar_width - 10 - extents.width) / 2, y - 5);
        cairo_show_text(cr, val_str);
    }
    
    // Title
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, width / 2 - 80, 20);
    cairo_show_text(cr, "Attendance Histogram");
}

// Similar for progress curve...
```

#### 3. Modifications dans `isra_callbacks.c`
```c
// Ajouter callbacks pour filtres
void on_button_apply_filter_clicked(GtkButton *button, gpointer data)
{
    // Récupérer dates
    // Récupérer coach
    // Filtrer données
    // Redessiner graphiques
    gtk_widget_queue_draw(drawing_histogram);
    gtk_widget_queue_draw(drawing_progress);
}
```

---

## 🔧 Étapes d'implémentation

### Phase 1 : Structure de base ✅
1. Créer stats_graphics.h
2. Créer stats_graphics.c avec fonctions de dessin
3. Modifier Makefile pour compiler

### Phase 2 : Intégration UI
1. Modifier merged.ui pour ajouter DrawingArea
2. Ajouter section Filters
3. Connecter signaux expose-event

### Phase 3 : Données et filtres
1. Créer structure de données
2. Implémenter filtres
3. Charger données depuis fichier

### Phase 4 : Graphiques Cairo
1. Implémenter histogramme
2. Implémenter courbe de progression
3. Ajouter légendes et axes

---

## 📊 Exemple de résultat attendu

```
┌─────────────────────────────────────┐
│         Filters                     │
│  From: [2025-01-01] To: [2025-12-31]│
│  Coach: [All ▼]  [Apply Filter]     │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│     Attendance Histogram            │
│  30 ┤        ███                    │
│  25 ┤     ███│██                    │
│  20 ┤  ███│██│██ ███                │
│  15 ┤███│██│██│████│██              │
│   0 └──────────────────────         │
│      Jan Feb Mar Apr May            │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│       Progress Curve                │
│  30 ┤           ╱─╲                 │
│  25 ┤        ╱─╯   ╲                │
│  20 ┤     ╱─╯       ╲─╮             │
│  15 ┤  ╱─╯             ╲            │
│   0 └────────────────────           │
│      Jan Feb Mar Apr May            │
└─────────────────────────────────────┘
```

---

## ⚙️ Dépendances

- **Cairo** : Déjà inclus avec GTK+2
- **GtkDrawingArea** : Widget standard GTK
- Pas de bibliothèque externe nécessaire

---

## 🚀 Prochaines étapes

1. Créer les fichiers sources
2. Modifier l'UI
3. Compiler et tester
4. Affiner les graphiques
5. Ajouter l'interactivité

**Voulez-vous que je commence l'implémentation ?**
