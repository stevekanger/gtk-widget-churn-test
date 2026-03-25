#pragma once

#include <gtk/gtk.h>

struct _GwcApp {
    GtkApplication parent;

    GtkBuilder *builder;
};

#define GWC_APP_TYPE (gwc_app_get_type())

G_DECLARE_FINAL_TYPE(GwcApp, gwc_app, GWC, APP, GtkApplication)

GwcApp *gwc_app_new(void);
