#pragma once
#include <gtk/gtk.h>

struct _GwcTab {
    GtkWidget parent_instance;

    GtkWidget *label;
    int id;
    int focused;
    gchar *name
};

G_BEGIN_DECLS

#define GWC_TAB_TYPE (gwc_tab_get_type())
G_DECLARE_FINAL_TYPE(GwcTab, gwc_tab, GWC, TAB, GtkWidget)

GtkWidget *gwc_tab_new(const int id, int focused, gchar *name);

GtkWidget *create_control(GtkBuilder *builder);

void update_tabs(GtkWidget *parent);

G_END_DECLS
