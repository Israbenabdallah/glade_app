/* stats_graphics.c
 * Professional implementation of statistics graphics using Cairo
 * Author: Gym Management System Team
 * Date: 2025-12-17
 */

#include "stats_graphics.h"
#include "support.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* ========== GLOBAL DATA STORAGE ========== */

/* Store current form data for chart generation */
static int g_classes_attended = 0;
static int g_consistency_score = 0;
static int g_use_real_data = 0;  /* Flag: 0=sample, 1=real */

/* ========== REAL-TIME DATA MANAGEMENT ========== */

void set_current_stats_data(int classes_attended, int consistency_score)
{
    g_classes_attended = classes_attended;
    g_consistency_score = consistency_score;
    g_use_real_data = 1;
    
    g_print("Stats data updated: Classes=%d, Consistency=%d%%\n", 
            classes_attended, consistency_score);
}

ChartData* get_current_histogram_data(void)
{
    g_print("DEBUG: get_current_histogram_data - use_real=%d, classes=%d\n", 
            g_use_real_data, g_classes_attended);
    
    if (!g_use_real_data || g_classes_attended <= 0) {
        g_print("DEBUG: Using sample data\n");
        return create_sample_histogram_data();
    }
    
    ChartData *data = (ChartData*)malloc(sizeof(ChartData));
    if (!data) return NULL;
    
    /* Generate histogram based on classes attended */
    data->count = 6;
    data->points = (StatDataPoint*)malloc(sizeof(StatDataPoint) * data->count);
    if (!data->points) {
        free(data);
        return NULL;
    }
    
    g_print("DEBUG: Generating histogram for %d classes\n", g_classes_attended);
    
    /* Simulate weekly distribution from total classes */
    int avg_per_week = g_classes_attended / data->count;
    if (avg_per_week < 1) avg_per_week = 1;
    
    int variation = (avg_per_week > 3) ? (avg_per_week / 3) : 1;
    int remaining = g_classes_attended;
    
    for (int i = 0; i < data->count; i++) {
        sprintf(data->points[i].date, "Week %d", i + 1);
        
        /* Add some variation to make it realistic */
        int offset = (i % 2 == 0) ? variation : -variation;
        data->points[i].value = avg_per_week + offset;
        
        /* Adjust last week to match exact total */
        if (i == data->count - 1) {
            // Calculate sum of previous weeks
            int sum = 0;
            for (int j = 0; j < data->count - 1; j++) {
                sum += data->points[j].value;
            }
            data->points[i].value = g_classes_attended - sum;
        }
        
        /* Keep values positive and sensible */
        if (data->points[i].value < 1) data->points[i].value = 1;
        if (data->points[i].value > g_classes_attended) data->points[i].value = g_classes_attended;
        
        g_print("DEBUG: Week %d = %d classes\n", i+1, data->points[i].value);
    }
    
    /* Find max value */
    data->max_value = 0;
    for (int i = 0; i < data->count; i++) {
        if (data->points[i].value > data->max_value)
            data->max_value = data->points[i].value;
    }
    
    /* Round up max_value for better display */
    if (data->max_value > 0) {
        int round_to = (data->max_value / 5) + 1;
        data->max_value = round_to * 5;
    }
    
    sprintf(data->title, "Weekly Attendance (Total: %d)", g_classes_attended);
    g_print("DEBUG: Histogram created with max=%d\n", data->max_value);
    return data;
}

ChartData* get_current_progress_data(void)
{
    if (!g_use_real_data || g_consistency_score == 0) {
        return create_sample_progress_data();
    }
    
    ChartData *data = (ChartData*)malloc(sizeof(ChartData));
    if (!data) return NULL;
    
    data->count = 8;
    data->points = (StatDataPoint*)malloc(sizeof(StatDataPoint) * data->count);
    if (!data->points) {
        free(data);
        return NULL;
    }
    
    /* Generate progression curve based on consistency score */
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug"};
    int start_score = g_consistency_score - 30;
    if (start_score < 20) start_score = 20;
    
    for (int i = 0; i < data->count; i++) {
        strcpy(data->points[i].date, months[i]);
        
        /* Gradual progression to current score */
        data->points[i].value = start_score + ((g_consistency_score - start_score) * i / 7);
    }
    
    data->max_value = 100;
    sprintf(data->title, "Performance Progress (Current: %d%%)", g_consistency_score);
    return data;
}

void update_stats_charts(GtkWidget *parent_widget)
{
    /* Histogram removed as per request */
    
    GtkWidget *progress = lookup_widget(parent_widget, "drawing_progress");
    if (!progress) {
        extern GtkWidget *ui_get_object_by_name(const char *name);
        progress = ui_get_object_by_name("drawing_progress");
    }
    
    /* Redraw only progress chart */
    if (progress) {
        gtk_widget_queue_draw(progress);
        g_print("Progress chart updated\n");
    }
}

/* ========== DATA MANAGEMENT ========== */

ChartData* create_sample_histogram_data(void)
{
    ChartData *data = (ChartData*)malloc(sizeof(ChartData));
    if (!data) return NULL;
    
    data->count = 6;
    data->points = (StatDataPoint*)malloc(sizeof(StatDataPoint) * data->count);
    if (!data->points) {
        free(data);
        return NULL;
    }
    
    /* Sample data - weekly attendance */
    strcpy(data->points[0].date, "Week 1");
    data->points[0].value = 15;
    
    strcpy(data->points[1].date, "Week 2");
    data->points[1].value = 22;
    
    strcpy(data->points[2].date, "Week 3");
    data->points[2].value = 18;
    
    strcpy(data->points[3].date, "Week 4");
    data->points[3].value = 28;
    
    strcpy(data->points[4].date, "Week 5");
    data->points[4].value = 25;
    
    strcpy(data->points[5].date, "Week 6");
    data->points[5].value = 30;
    
    /* Find max value */
    data->max_value = 0;
    for (int i = 0; i < data->count; i++) {
        if (data->points[i].value > data->max_value)
            data->max_value = data->points[i].value;
    }
    
    strcpy(data->title, "Weekly Attendance");
    return data;
}

ChartData* create_sample_progress_data(void)
{
    ChartData *data = (ChartData*)malloc(sizeof(ChartData));
    if (!data) return NULL;
    
    data->count = 8;
    data->points = (StatDataPoint*)malloc(sizeof(StatDataPoint) * data->count);
    if (!data->points) {
        free(data);
        return NULL;
    }
    
    /* Sample data - performance progression */
    strcpy(data->points[0].date, "Jan");
    data->points[0].value = 60;
    
    strcpy(data->points[1].date, "Feb");
    data->points[1].value = 65;
    
    strcpy(data->points[2].date, "Mar");
    data->points[2].value = 70;
    
    strcpy(data->points[3].date, "Apr");
    data->points[3].value = 68;
    
    strcpy(data->points[4].date, "May");
    data->points[4].value = 75;
    
    strcpy(data->points[5].date, "Jun");
    data->points[5].value = 80;
    
    strcpy(data->points[6].date, "Jul");
    data->points[6].value = 85;
    
    strcpy(data->points[7].date, "Aug");
    data->points[7].value = 90;
    
    /* Find max value */
    data->max_value = 100;  /* Percentage scale */
    
    strcpy(data->title, "Performance Progress (%)");
    return data;
}

void free_chart_data(ChartData *data)
{
    if (data) {
        if (data->points) free(data->points);
        free(data);
    }
}

/* ========== UTILITY DRAWING FUNCTIONS ========== */

void draw_chart_title(cairo_t *cr, const char *title, int width)
{
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "Sans", 
                          CAIRO_FONT_SLANT_NORMAL, 
                          CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    
    cairo_text_extents_t extents;
    cairo_text_extents(cr, title, &extents);
    cairo_move_to(cr, (width - extents.width) / 2, 25);
    cairo_show_text(cr, title);
}

void draw_chart_axes(cairo_t *cr, int margin, int width, int height)
{
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2);
    
    /* Y-axis */
    cairo_move_to(cr, margin, margin);
    cairo_line_to(cr, margin, height - margin);
    
    /* X-axis */
    cairo_line_to(cr, width - margin, height - margin);
    cairo_stroke(cr);
}

void draw_value_labels(cairo_t *cr, int max_value, int margin, int height)
{
    if (max_value <= 0) {
        max_value = 10;  // Default safe value
    }
    
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_select_font_face(cr, "Sans",
                          CAIRO_FONT_SLANT_NORMAL,
                          CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 10);
    
    int chart_height = height - 2 * margin;
    int step = max_value / 5;
    if (step < 1) step = 1;
    
    for (int i = 0; i <= 5; i++) {
        int value = i * step;
        char label[20];
        sprintf(label, "%d", value);
        
        cairo_text_extents_t extents;
        cairo_text_extents(cr, label, &extents);
        
        int y = height - margin - (i * chart_height / 5);
        cairo_move_to(cr, margin - extents.width - 10, y + 4);
        cairo_show_text(cr, label);
        
        /* Grid line */
        cairo_set_source_rgba(cr, 0.8, 0.8, 0.8, 0.5);
        cairo_set_line_width(cr, 1);
        cairo_move_to(cr, margin, y);
        cairo_line_to(cr, margin + 10, y);
        cairo_stroke(cr);
        cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    }
}

/* ========== HISTOGRAM DRAWING ========== */

void draw_histogram(cairo_t *cr, int width, int height, ChartData *data)
{
    if (!data || data->count == 0) return;
    
    /* Safety check for max_value */
    if (data->max_value <= 0) {
        data->max_value = 10;  // Default safe value
    }
    
    /* White background */
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
    
    /* Draw title */
    draw_chart_title(cr, data->title, width);
    
    /* Margins */
    int margin = 50;
    int chart_height = height - 2 * margin;
    int chart_width = width - 2 * margin;
    int bar_width = chart_width / data->count;
    
    /* Draw axes */
    draw_chart_axes(cr, margin, width, height);
    draw_value_labels(cr, data->max_value, margin, height);
    
    /* Draw bars */
    cairo_select_font_face(cr, "Sans",
                          CAIRO_FONT_SLANT_NORMAL,
                          CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 10);
    
    for (int i = 0; i < data->count; i++) {
        int bar_height = (data->points[i].value * chart_height) / data->max_value;
        int x = margin + i * bar_width + bar_width * 0.1;
        int y = height - margin - bar_height;
        int bar_w = bar_width * 0.8;
        
        /* Bar with gradient (professional look) */
        cairo_pattern_t *pat = cairo_pattern_create_linear(0, y, 0, height - margin);
        cairo_pattern_add_color_stop_rgb(pat, 0, 0.3, 0.7, 1.0);  /* Blue gradient */
        cairo_pattern_add_color_stop_rgb(pat, 1, 0.15, 0.4, 0.8);
        
        cairo_rectangle(cr, x, y, bar_w, bar_height);
        cairo_set_source(cr, pat);
        cairo_fill_preserve(cr);
        
        /* Border */
        cairo_set_source_rgb(cr, 0.1, 0.3, 0.6);
        cairo_set_line_width(cr, 1.5);
        cairo_stroke(cr);
        cairo_pattern_destroy(pat);
        
        /* Value on top of bar */
        char val_str[20];
        sprintf(val_str, "%d", data->points[i].value);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, val_str, &extents);
        
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_move_to(cr, x + (bar_w - extents.width) / 2, y - 8);
        cairo_show_text(cr, val_str);
        
        /* Label below (date/period) */
        cairo_text_extents(cr, data->points[i].date, &extents);
        cairo_move_to(cr, x + (bar_w - extents.width) / 2, height - margin + 20);
        cairo_show_text(cr, data->points[i].date);
    }
}

/* ========== PROGRESS CURVE DRAWING ========== */

void draw_progress_curve(cairo_t *cr, int width, int height, ChartData *data)
{
    if (!data || data->count == 0) return;
    
    /* Safety check for max_value */
    if (data->max_value <= 0) {
        data->max_value = 100;  // Default for percentage
    }
    
    /* Light gray background */
    /* Light gray background */
    cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
    
    /* Draw title */
    draw_chart_title(cr, data->title, width);
    
    /* Margins */
    int margin = 50;
    int chart_height = height - 2 * margin;
    int chart_width = width - 2 * margin;
    
    /* Draw axes */
    draw_chart_axes(cr, margin, width, height);
    draw_value_labels(cr, data->max_value, margin, height);
    
    /* Draw grid */
    cairo_set_source_rgba(cr, 0.8, 0.8, 0.8, 0.3);
    cairo_set_line_width(cr, 1);
    for (int i = 1; i < 5; i++) {
        int y = margin + (i * chart_height / 5);
        cairo_move_to(cr, margin, y);
        cairo_line_to(cr, width - margin, y);
    }
    cairo_stroke(cr);
    
    /* Draw curve */
    if (data->count > 1) {
        cairo_set_line_width(cr, 3);
        cairo_set_source_rgb(cr, 0.2, 0.8, 0.3);  /* Green */
        
        /* Start path */
        int x0 = margin;
        int y0 = height - margin - (data->points[0].value * chart_height / data->max_value);
        cairo_move_to(cr, x0, y0);
        
        /* Draw smooth curve through points */
        for (int i = 1; i < data->count; i++) {
            int x = margin + (i * chart_width / (data->count - 1));
            int y = height - margin - (data->points[i].value * chart_height / data->max_value);
            cairo_line_to(cr, x, y);
        }
        cairo_stroke(cr);
        
        /* Draw data points */
        for (int i = 0; i < data->count; i++) {
            int x = margin + (i * chart_width / (data->count - 1));
            int y = height - margin - (data->points[i].value * chart_height / data->max_value);
            
            /* Point circle */
            cairo_arc(cr, x, y, 5, 0, 2 * M_PI);
            cairo_set_source_rgb(cr, 1, 1, 1);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, 0.2, 0.8, 0.3);
            cairo_set_line_width(cr, 2);
            cairo_stroke(cr);
            
            /* Label */
            cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
            cairo_select_font_face(cr, "Sans",
                                  CAIRO_FONT_SLANT_NORMAL,
                                  CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, 10);
            
            cairo_text_extents_t extents;
            cairo_text_extents(cr, data->points[i].date, &extents);
            cairo_move_to(cr, x - extents.width / 2, height - margin + 20);
            cairo_show_text(cr, data->points[i].date);
        }
    }
}

/* ========== GTK CALLBACKS ========== */

gboolean on_drawing_histogram_expose(GtkWidget *widget,
                                     GdkEventExpose *event,
                                     gpointer user_data)
{
    (void)event;
    (void)user_data;
    
    cairo_t *cr = gdk_cairo_create(widget->window);
    
    int width = widget->allocation.width;
    int height = widget->allocation.height;
    
    /* Use real data if available, otherwise sample data */
    ChartData *data = get_current_histogram_data();
    if (data) {
        draw_histogram(cr, width, height, data);
        free_chart_data(data);
    }
    
    cairo_destroy(cr);
    return FALSE;
}

gboolean on_drawing_progress_expose(GtkWidget *widget,
                                    GdkEventExpose *event,
                                    gpointer user_data)
{
    (void)event;
    (void)user_data;
    
    cairo_t *cr = gdk_cairo_create(widget->window);
    
    int width = widget->allocation.width;
    int height = widget->allocation.height;
    
    /* Use real data if available, otherwise sample data */
    ChartData *data = get_current_progress_data();
    if (data) {
        draw_progress_curve(cr, width, height, data);
        free_chart_data(data);
    }
    
    cairo_destroy(cr);
    return FALSE;
}
