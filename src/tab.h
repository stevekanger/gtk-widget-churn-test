#pragma once
#include <gtk/gtk.h>

struct _GwcTab {
    GtkButton parent_instance;

    int id;
    gchar *name;
    gchar *app_id;
    gchar *ws_name;
    int focused;
};

G_BEGIN_DECLS

#define CUSTOM_TAB_TYPE (gwc_tab_get_type())
G_DECLARE_FINAL_TYPE(GwcTab, gwc_tab, GWC, TAB, GtkButton)

GtkWidget *gwc_tab_new(
    const int id,
    const gchar *name,
    const gchar *app_id,
    const gchar *ws_name,
    int focused
);

GtkWidget *create_control(GtkBuilder *builder);

void update_tabs(GtkWidget *parent);

G_END_DECLS
