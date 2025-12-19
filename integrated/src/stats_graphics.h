/* stats_graphics.h
 * Professional implementation of statistics graphics using Cairo
 * Author: Gym Management System Team
 * Date: 2025-12-17
 */

#ifndef STATS_GRAPHICS_H
#define STATS_GRAPHICS_H

#include <gtk/gtk.h>
#include <cairo.h>

/* Data Structures */
typedef struct {
    char date[20];      /* Date in format YYYY-MM-DD */
    int value;          /* Attendance count or score */
} StatDataPoint;

typedef struct {
    StatDataPoint *points;  /* Array of data points */
    int count;              /* Number of points */
    int max_value;          /* Maximum value for scaling */
    char title[100];        /* Chart title */
} ChartData;

/* Drawing Area Callbacks */
gboolean on_drawing_histogram_expose(GtkWidget *widget, 
                                     GdkEventExpose *event, 
                                     gpointer user_data);

gboolean on_drawing_progress_expose(GtkWidget *widget,
                                    GdkEventExpose *event,
                                    gpointer user_data);

/* Chart Drawing Functions */
void draw_histogram(cairo_t *cr, int width, int height, ChartData *data);
void draw_progress_curve(cairo_t *cr, int width, int height, ChartData *data);

/* Utility Functions */
void draw_chart_axes(cairo_t *cr, int margin, int width, int height);
void draw_chart_title(cairo_t *cr, const char *title, int width);
void draw_value_labels(cairo_t *cr, int max_value, int margin, int height);

/* Data Management */
ChartData* create_sample_histogram_data(void);
ChartData* create_sample_progress_data(void);
void free_chart_data(ChartData *data);

/* Real-time Data Management */
void set_current_stats_data(int classes_attended, int consistency_score);
ChartData* get_current_histogram_data(void);
ChartData* get_current_progress_data(void);
void update_stats_charts(GtkWidget *parent_widget);

#endif /* STATS_GRAPHICS_H */
